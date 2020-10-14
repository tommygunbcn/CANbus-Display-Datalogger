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

/*
In this file all the variables related to serial transmission shall be declared.
In this example, a simple full duplex communication is used, with a simple protocol, better explained in the .cpp file. TBR if another protocol  is used*/

#ifndef _Serial_setup_h
#define _Serial_setup_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//uncomment if serial comm is needed
#define SERIAL_COM			
//define the baudrate for serial communication
#define SERIAL_BAUD_RATE 115200

/*define the communication protocol parameters*/
//define what to send via UART, 
#define do_not_send				0
#define send_normal_stuff		1
#define send_controller_params	2


//define instructions received via uart, declared as a chars
#define do_not_send_char	'0'			
#define send_normal_stuff_char	'1'			
#define send_controller_params_char	'2'			

// variables
//declare the machine state variable
extern unsigned char what_to_send;
//buffer for message received, as we expect a short message, make it only 10 bytes, saves RAM!
extern unsigned char received[10];

//functions
void Check_Serial_Task(void);
#endif

