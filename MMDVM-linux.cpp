/*
 *   Copyright (C) 2015,2016 by Jonathan Naylor G4KLX
 *   Copyright (C) 2016 by Mathis Schmieder DB9MAT
 *   Copyright (C) 2016 by Colin Durbridge G4EML
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Config.h"
#if defined(LINUX_SDR)

#include "Globals.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


// Global variables
MMDVM_STATE m_modemState = STATE_IDLE;

bool m_dstarEnable = true;
bool m_dmrEnable   = true;
bool m_ysfEnable   = true;
bool m_p25Enable   = true;

bool m_duplex = true;

bool m_tx = false;

uint32_t m_sampleCount = 0U;
bool    m_sampleInsert = false;

CDStarRX   dstarRX;
CDStarTX   dstarTX;

CDMRIdleRX dmrIdleRX;
CDMRRX     dmrRX;
CDMRTX     dmrTX;

CDMRDMORX  dmrDMORX;
CDMRDMOTX  dmrDMOTX;

CYSFRX     ysfRX;
CYSFTX     ysfTX;

CP25RX     p25RX;
CP25TX     p25TX;

CCalDStarRX calDStarRX;
CCalDStarTX calDStarTX;
CCalDMR     calDMR;

CCWIdTX cwIdTX;

CSerialPort serial;
CIO io;

int serialfd;

void setup()
{
  // Prepare socket
  serialfd = posix_openpt(O_RDWR  | O_NOCTTY);
  if(grantpt(serialfd) == -1) {
    perror("grantpt");
    close(serialfd); exit(1);
  }
  if(unlockpt(serialfd)==-1) {
    perror("unlockpt");
    close(serialfd); exit(1);
  }
  char *p = ptsname(serialfd);
  if(p==NULL) {
    perror("ptsname");
    close(serialfd); exit(1);
  }
  unlink("/tmp/mmdvm-tty");
  symlink(p, "/tmp/mmdvm-tty");  
  serial.start();

  // prepare stdin
  int fd = STDIN_FILENO;
  /* Save the current flags */
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
      return 0;

  int blocking = 0;
  if (blocking)
      flags &= ~O_NONBLOCK;
  else
      flags |= O_NONBLOCK;
  int res = fcntl(fd, F_SETFL, flags);
}

#define READSIZE 256
bool handleinput()
{
  int r;
  uint8_t buf[READSIZE];
  struct pollfd fds[]= { {STDIO_FILENO, POLLIN } };
  r = poll(fds, 1, 0);
  if(r<0) { perror("poll failed\n"); return false; }
  if(fds[0].revents & POLLHUP) {
     printf("poll: has hup! %d\n", fds[0].revents);
     return false;
  }
  if(fds[0].revents & POLLIN) {
    printf("poll: has data! %d\n", fds[0].revents);
    int res = read(STDIO_FILENO, buf, READSIZE);
    if(res>0) {
       int i;
       for(i=0; i<res; i+=4) { 
 	   // INPUT: float; OUTPUT: unsigned int 0..4096
	    float in = *(float *)(buf+i);
	    // clamp
	    if(in<-1) in=-1;
	    if(in>1) in=1;
	    uint16_t sample = (in+1.0)*2048;
            m_rxBuffer.put(sample, MARK_NONE);
       }
    }
  }
}

void loop()
{
  // Take data from stdio to rx queue
  //TODO  24kHz sampling rate
  handleinput();

  // handle rs232 inout (commands)
  serial.process();
 
  // Process rx queue data 
  io.process();

  // The following is for transmitting
  if (m_dstarEnable && m_modemState == STATE_DSTAR)
    dstarTX.process();

  if (m_dmrEnable && m_modemState == STATE_DMR) {
    if (m_duplex)
      dmrTX.process();
    else
      dmrDMOTX.process();
  }

  if (m_ysfEnable && m_modemState == STATE_YSF)
    ysfTX.process();

  if (m_p25Enable && m_modemState == STATE_P25)
    p25TX.process();

  if (m_modemState == STATE_DSTARCAL)
    calDStarTX.process();

  if (m_modemState == STATE_DMRCAL)
    calDMR.process();

	if (m_modemState == STATE_IDLE)
		cwIdTX.process();
}

int main()
{
  setup();

  for (;;)
    loop();
}

#endif

