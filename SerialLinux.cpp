/*
 *   Copyright (C) 2016 by Hans P. Reiser DL9RDZ
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
#include "Globals.h"

#include "SerialPort.h"

#if defined(LINUX_SDR)
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

extern int serialfd;
extern void resetSerial();

void CSerialPort::beginInt(uint8_t n, int speed)
{
/*
  switch (n) {
    case 1U:
      Serial.begin(speed);
      break;
    case 2U:
      Serial2.begin(speed);
      break;
    case 3U:
      Serial3.begin(speed);
      break;
    default:
      break;
  }
*/
}

int CSerialPort::availableInt(uint8_t n)
{
  int r;
  struct pollfd fds[]= { {serialfd, POLLIN | POLLHUP | POLLRDNORM} };
  switch (n) {
    case 1U:
      r = poll(fds, 1, 0);
      if(r<0) { perror("poll failed\n"); return false; }
      if(fds[0].revents & POLLHUP) {
	printf("poll: has hup! %d\n", fds[0].revents);
	resetSerial();
      }
      if(fds[0].revents & POLLIN) {
	printf("poll: has data! %d\n", fds[0].revents);
        return true;
      }
      return false;
    default:
      return false;
  }
}

uint8_t CSerialPort::readInt(uint8_t n)
{
  uint8_t ret;
  int res;
 
  switch (n) {
    case 1U:
      res = read(serialfd, &ret, 1);
      printf("reading data! got %d (%d)\n", res, ret);
      if(res==1) return ret;
      if(res<0) perror("read failed\n");
      return 0U;
    default:
      return 0U;
  }
}

void CSerialPort::writeInt(uint8_t n, const uint8_t* data, uint16_t length, bool flush)
{
  switch (n) {
    case 1U:
      write(serialfd, data, length);
      //write(serialfd, "\r\n", 2);
      //if (flush)
      //  Serial.flush();
      break;
    default:
      break;
  }
}

#endif

