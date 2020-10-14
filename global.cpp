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
In this file, the global variables are defined
*/


#include "global.h"

// helpers
char received_number[10] = "0";
char received_number_aux[10] = "0.0";
char received_number_aux_2[15] = "0.0";

//flags
volatile bool Flag_refesh_screen = 0;         //flag set if refresh screen needed
volatile bool Flag_CAN_request = 0;           //flag set if CAN request needed
volatile bool Flag_refresh_info = 0;          //flag set if info strip needs to be refreshed
volatile bool Flag_check_serial_comm = 0;     
volatile bool Flag_check_slow_serial_comm = 0;

volatile bool Flag_controler_info_received = 0; //to flag a message is sent , only one setup message at a time
volatile bool Flag_pedelec_info_received = 0;   //to flag that the needed startup info from the pedelec has been received. 

//flags
bool reconfigure_pedelec = 0;  //flag to reconfigure 

unsigned int little_endian_int(unsigned int big_endian)
{// convert received int to little endian

	unsigned int high_byte = big_endian;
	unsigned int low_byte = big_endian;
	unsigned int little_endian = 0;

	//high_byte &= 0xff00;
	high_byte = high_byte << 8;
	low_byte = low_byte >> 8;
	low_byte = (low_byte & 0x00ff);

	little_endian = (high_byte | low_byte);

	return(little_endian);

}

