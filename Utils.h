/*
 *   Copyright (C) 2015,2016 by Jonathan Naylor G4KLX
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

#if !defined(UTILS_H)
#define  UTILS_H

#include "Config.h"
#if defined(STM32F4XX) || defined(STM32F4)
#include "stm32f4xx.h"
#include <cstddef>
#elif defined(LINUX_SDR)
#include "noarm_math.h"
#else
#include <Arduino.h>
#endif

uint8_t countBits8(uint8_t bits);

uint8_t countBits32(uint32_t bits);

uint8_t countBits64(uint64_t bits);

#endif

