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

/*global variables are declared here, as well as all the includes needed to have all the global variables is this file*/
#ifndef _GLOBAL_h
#define _GLOBAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif



#include "Serial_CAN_Module.h"
#include <SoftwareSerial.h>
#include <stdint.h>
#include "TFTv2.h"
#include "bit_defs.h"
#include "screen_defs.h"
#include "screen_setup.h"
#include "Joystick.h"
#include "Variables.h"
#include "Analog_setup.h"
#include "Serial_setup.h"
#include "CAN_db.h"
#include "CAN_setup.h"
#include <SPI.h>
#include "mcp_can.h"
#include <TimerOne.h>



//functions
unsigned int little_endian_int(unsigned int big_endian);

//defines
#define TRUE 1
#define FALSE 0

// strings
extern char received_number[10];// = "0";
extern char received_number_aux[10];// = "0.0";
extern char received_number_aux_2[15];// = "0.0";


// flags
extern volatile bool Flag_refesh_screen, Flag_CAN_request, Flag_refresh_info,Flag_check_serial_comm, Flag_check_slow_serial_comm;

/*TBR if a noded is added or erased, that will send a CAN message under request*/
extern volatile bool Flag_controler_info_received; //to flag a message is sent , only one setup message at a time
extern volatile bool Flag_pedelec_info_received ;   //to flag that the needed startup info from the pedelec has been received. 

/*TBR if the pedelec node is not used, it can be deleted*/
extern bool reconfigure_pedelec;  //flag if a reconfiguration of the pedelec node is needed

#endif
