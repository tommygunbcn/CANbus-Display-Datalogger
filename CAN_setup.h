/*
2019 Copyright (c) microvoltia s.l.

author: Tommygunbcn

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
Lesser General Public License for more details.*/
/*this file defines the connectivity of the CAN shield and the functions used */
#ifndef _CAN_setup_h
#define _CAN_setup_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "mcp_can.h"
// define the required speed of the CAN bus
#define CAN_SPEED CAN_250KBPS
/*the module MCP2515 connects to arduino via SPI comunication. Define here the CS pin needed*/
#define SPI_CS_PIN  9
//extern declaration
extern MCP_CAN CAN;
//functions
void Check_CAN_Task(void);


#endif

