/*
 *   Copyright (C) 2016 by Hans Reiser, DL9RDZ
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
#include "IO.h"

#if defined(LINUX_SDR)
#include <string>
#include <iostream>
#include <stdio.h>

const uint16_t DC_OFFSET = 2048U;

void signal(const char *what, bool on)
{
   printf("output %s is %d\n", what, on);
}


void CIO::initInt()
{
}

void CIO::startInt()
{
  signal("PTT", m_pttInvert);
  signal("COSLED",0);
  signal("LED",1);
}

void CIO::deliver(uint16_t count, uint16_t samples[])
{
	int i;
	for(i=0; i<count; i++) {
	   m_rxBuffer.put(samples[i], MARK_NONE);
           m_rssiBuffer.put(0U);
        }
}

#if 0
void CIO::interrupt(uint8_t source)
{
  if ((ADC->ADC_ISR & ADC_ISR_EOC_Chan) == ADC_ISR_EOC_Chan) {    // Ensure there was an End-of-Conversion and we read the ISR reg
    uint8_t control = MARK_NONE;
    uint16_t sample = DC_OFFSET;

    m_txBuffer.get(sample, control);
    DACC->DACC_CDR = sample;

    sample = ADC->ADC_CDR[ADC_CDR_Chan];
    m_rxBuffer.put(sample, control);

    m_rssiBuffer.put(0U);

    m_watchdog++;
  }
}
#endif

bool CIO::getCOSInt()
{
  //return digitalRead(PIN_COS) == HIGH;
  return 0;
}

void CIO::setLEDInt(bool on)
{
  signal("LED", on);
  //digitalWrite(PIN_LED, on ? HIGH : LOW);
}

void CIO::setPTTInt(bool on)
{
  signal("PTT", on);
  //digitalWrite(PIN_PTT, on ? HIGH : LOW);
}

void CIO::setCOSInt(bool on)
{
  signal("COS", on);
  //digitalWrite(PIN_COSLED, on ? HIGH : LOW);
}

void CIO::setDStarInt(bool on)
{
  signal("DStar",on);
  //digitalWrite(PIN_DSTAR, on ? HIGH : LOW);
}

void CIO::setDMRInt(bool on) 
{
  signal("DMR",on);
  //digitalWrite(PIN_DMR, on ? HIGH : LOW);
}

void CIO::setYSFInt(bool on)
{
  signal("YSF",on);
  //digitalWrite(PIN_YSF, on ? HIGH : LOW);
}

void CIO::setP25Int(bool on) 
{
  signal("P25", on);
  //digitalWrite(PIN_P25, on ? HIGH : LOW);
}

#endif

