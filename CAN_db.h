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
in this file are defined all the CAN messages and ID's. They are declared here and defined in CAN_db.cpp
TBR every line shall be revised to define the CAN database of the project.

*/

#ifndef _CAN_DB_h
#define _CAN_DB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


//define ID's of the messages of every node

//pedelec
#define PEDELEC_1_ID      0x151
#define PEDELEC_2_ID	  0x152		// id of the pedelec message
#define PEDELEC_3_ID	  0x153
#define PEDELEC_4_ID	  0x155
#define pedelec_ID_5	  0x156		// id of the pedelec message
#define PEDELEC_6_ID	  0x157		// id of the pedelec message
#define pedelec_answer_ID	0x154
#define pedelec_command_ID	0x140		// more priority than the others
//bms
#define bms_digital_ID	  0x1f4
#define bms_analog_ID     0x1f3
#define bms_analog2_ID	  0x1f2
//controller
#define controller_0_ID	  0x1E0
#define controller_1_ID	  0x1E1
#define controller_2_ID	  0x1E2
#define controller_3_ID	  0x1E3
#define controller_4_ID	  0x1E4
#define controller_5_ID	  0x1E5
#define controller_6_ID	  0x1E6
#define CCP_DTO_ID        0x101           /* CAN identifier ECU -> Master */
#define CCP_CRO_ID        0x100           /* CAN identifier Master -> ECU , higher priority than DTO*/

//commands to pedelec
#define SEND_PI_PARAMETERS			0x01
#define RECEIVE_PI_PARAMETERS 		0x02
#define SAVE_PI_PARAMETERS			0x03

#define SEND_START_PARAMETERS		0x04
#define RECEIVE_START_PARAMETERS 	0x05
#define SAVE_START_PARAMETERS		0x06

#define SEND_ERROR_COUNTER_1		0x07
#define SEND_ERROR_COUNTER_2		0x08

#define RESET_ERROR_COUNTERS		0x09
#define SEND_ERROR_COUNTER_3		0x0a
#define SEND_ERROR_COUNTER_4		0x0b
#define SEND_ERROR_COUNTER_5		0x0c

// commands to controller
#define SEND_GEN_PARAMETERS			0x01
#define RECEIVE_GEN_PARAMETERS 		0x02
#define SAVE_GEN_PARAMETERS			0x03

#define SEND_PI_PARAMETERS_CNTR			0x0e
#define RECEIVE_PI_PARAMETERS_CNTR 		0x0f
#define SAVE_PI_PARAMETERS_CNTR			0x10


//define all the messages using union structure. Also define the conversion factors if needed

//--------------------------------------------------------------------------------pedelec node----------------------------
#define TX_pedelec_1_dlc 8            //pedelec broadcast
union TX_pedelec_1 {
	unsigned char tx_message[TX_pedelec_1_dlc];
	struct {
		unsigned char version_high_byte;
		unsigned char version_low_byte;
		unsigned int throttle_supply_voltage;
		unsigned int sensed_speed;
		unsigned int bottom_bracket_speed;
	}signal;
};
#define throttle_supply_factor 0.01
extern union TX_pedelec_1 TX_pedelec_brdcast_1; //broadcast message 1

#define TX_pedelec_2_dlc 8						//pedelec broadcast
union TX_pedelec_2 {
	unsigned char tx_message[TX_pedelec_2_dlc];
	struct {
		signed char temperature;
		unsigned char reset_cause;
		unsigned int Error1;
		unsigned int Error_flash;
		unsigned char info;
		unsigned char warning;

	}signal;
};
extern union TX_pedelec_2 TX_pedelec_brdcast_2;	//broadcast message 2


#define TX_pedelec_6_dlc 8						//pedelec broadcast

union TX_pedelec_6 {
	unsigned char tx_message[TX_pedelec_6_dlc];
	struct {
		unsigned int	odometer;
		unsigned int	working_hours;
		unsigned int 	supply_voltage;
		unsigned int 	free2;

	}signal;
};
extern union TX_pedelec_6 TX_pedelec_brdcast_6;	//broadcast message 1


#define TX_pedelec_3_dlc 8						//pedelec broadcast

union TX_pedelec_3 {
	unsigned char tx_message[TX_pedelec_3_dlc];
	struct {
		signed int increment;
		unsigned int throttle_input;
		unsigned int throttle_output;
		unsigned int sensed_speed;
	}signal;
};
extern union TX_pedelec_3 TX_pedelec_brdcast_3;	//broadcast message 1

#define TX_pedelec_4_dlc 8						//pedelec broadcast

union TX_pedelec_4 {
	unsigned char tx_message[TX_pedelec_4_dlc];
	struct {
		unsigned char a;
		signed char pedal_counter;
		unsigned int free;
		unsigned int free1;
		unsigned int free2;
	}signal;
};
extern union TX_pedelec_4 TX_pedelec_brdcast_4;	//broadcast message 1


// communication messages
#define TX_pedelec_dlc 8
union TX_CAN_PEDELEC_COMMAND {
	unsigned char tx_message[TX_pedelec_dlc];

};

extern union TX_CAN_PEDELEC_COMMAND TX_msg_PEDELEC_COMMAND;


union RX_pedelec_answer {
	unsigned char rx_message[TX_pedelec_dlc];

};
extern union RX_pedelec_answer RX_pedelec_brdcast_answer;	//broadcast message 1


//--------------------------------------------------------------------------------------------------BMS node---------------------------------
#define bms_analog_1_size  8
union RX_CAN_BMS_analog {
	unsigned char received_message[bms_analog_1_size];
	struct {
		signed int voltage;
		signed int current;
		signed int temperature;
		signed int SoC;
	}signal;
};
//factors

#define bms_voltage_factor	0.01
#define bms_current_factor	0.01
#define bms_temp_factor	1
#define bms_soc_factor	1

extern union RX_CAN_BMS_analog RX_msg_BMS_analog;     // received message from BMS

#define bms_analog_2_size  8
union RX_CAN_BMS_analog2 {
	unsigned char received_message[bms_analog_2_size];
	struct {
		signed int cicles;

	}signal;
};

extern union RX_CAN_BMS_analog2 RX_msg_BMS_analog2;     // received message from BMS

#define bms_digital_size  8
union RX_CAN_BMS_digital {
	unsigned char received_message[bms_digital_size];
	struct {
		unsigned int digital_flags;
		unsigned int word2;
		unsigned int word3;
		unsigned int word4;

	}signal;
};
extern union RX_CAN_BMS_digital RX_msg_BMS_digital;			// received message from BMS

//-----------------------------------------------------CONTROLLER---node-------------------------------------------------------------------------------------


//------Controller Broadcast 0 
#define RX_Ctl_brdcst_0_dlc 8						//controller boradcast_0 datalength
union RX_Broadcast_0 {								// broadcast message 0
	unsigned char rx_message[RX_Ctl_brdcst_0_dlc];
	struct {
		unsigned char Vehicle_speed;
		unsigned char Driving_mode;
		unsigned int Mileage;
		unsigned int  Trip;
		unsigned int working_hours;
	}signal;
};
extern union RX_Broadcast_0 RX_Ctl_brdcst_0;    	// broadcast message 0

													//------Controller Broadcast 1 

#define RX_Ctl_brdcst_1_dlc 8						//controller boradcast_1 datalength
union RX_Broadcast_1 {								// broadcast message 1
	unsigned char rx_message[RX_Ctl_brdcst_1_dlc];
	struct {
		unsigned int Power_output;
		unsigned char Cruise_control;
		unsigned char LEDS;
		int ERROR_word_1;
		int ERROR_word_2;
	}signal;
};
extern union RX_Broadcast_1 RX_Ctl_brdcst_1;    	// broadcast message 1

													//------Controller Broadcast 2
#define RX_Ctl_brdcst_2_dlc 8						//controller boradcast_2 datalength
union RX_Broadcast_2 {
	unsigned char rx_message[RX_Ctl_brdcst_2_dlc];
	struct {
		unsigned int WARNING_word_1;
		unsigned int WARNING_word_2;
		unsigned char current_block;
		unsigned char prev_block;
		unsigned char efficiency;
		unsigned char free;
	}signal;
};
extern union RX_Broadcast_2 RX_Ctl_brdcst_2;	//broadcast message 2

												//------Controller Broadcast 3
#define RX_Ctl_brdcst_3_dlc 8						//controller boradcast 3 datalength
union RX_Broadcast_3 {
	unsigned char rx_message[RX_Ctl_brdcst_3_dlc];
	struct {
		signed int Motor_temp;
		signed int Controller_temp;
		signed int Input_current;
		signed int Input_voltage;
	}signal;
};
extern union RX_Broadcast_3 RX_Ctl_brdcst_3;	//broadcast message 3
//factors

#define Input_voltage_factor	0.1
#define Input_current_factor	0.1


#define RX_Ctl_brdcst_4_dlc 8						//controller broadcast 4 datalength
union RX_Broadcast_4 {
	unsigned char rx_message[RX_Ctl_brdcst_4_dlc];
	struct {
		char brake_state;
		char hall_sensor_state;
		char throttle_pos;
		char motor_lim_fac;
		char contr_lim_fac;
		char batt_temp_fac;
		char dc_lim_fac;
		char ac_lim_fac;
	}signal;
};
extern union RX_Broadcast_4 RX_Ctl_brdcst_4;	//broadcast message 4


#define RX_Ctl_brdcst_5_dlc 8						//controller broadcast 5 datalength
union RX_Broadcast_5 {
	unsigned char rx_message[RX_Ctl_brdcst_5_dlc];
	struct {
		char torque_req_bc;
		char current_req_bc;
		char phase_current_bc;
		char phase_current_arms_bc;
		char torque_delivered_bc;
		char modulation_index_bc;
		unsigned int input_power;
	}signal;
};
extern union RX_Broadcast_5 RX_Ctl_brdcst_5;	//broadcast message 5


#define RX_Ctl_brdcst_6_dlc 8						//controller broadcast 5 datalength
union RX_Broadcast_6 {
	unsigned char rx_message[RX_Ctl_brdcst_6_dlc];
	struct {
		unsigned int gate_time;
		unsigned int current_req;
		unsigned int current_out;
		signed char pi_reg;
		signed char free;

	}signal;
};
extern union RX_Broadcast_6 RX_Ctl_brdcst_6;	//broadcast message 9
#define current_req_factor	0.1
#define current_out_factor	0.1
//-----------------------------------------------------------------------------------------communication messages
#define TX_cntr_dlc 8
union TX_CAN_CNTR_COMMAND {
	unsigned char tx_message[TX_cntr_dlc];

};

extern union TX_CAN_CNTR_COMMAND TX_msg_CNTR_COMMAND;


union RX_CNTR_answer {
	unsigned char rx_message[TX_cntr_dlc];

};
extern union RX_CNTR_answer RX_CNTR_answer;	//broadcast message 1

#endif


