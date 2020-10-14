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
In this file all the variables from each node shall be declared. TBR every single line of the file, it the node changes.
*/

#ifndef _Variables_h
#define _Variables_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "bit_defs.h"
//------------------------------------------------------------------------------------------Analog read values
//current sensor

extern int max_current_ARMS;//maximum current arms that the sensor can sense
extern int current_read_value;
extern float ph_current_measured_arms;
//------------------------------------------------------------------------------------------------------------------------------ Pedelec values
extern float Thrott_supply_volt;// = 0;
extern float Power_supply_volt;// = 0;
extern float throttle_input;// = 0;
extern float throttle_output;// = 0;
extern float ped_counter;// = 0;

extern unsigned int odometer;
extern unsigned int working_hours;
// param values
extern byte_def config_byte;
extern unsigned char max_speed;
extern unsigned char pedal_events;
extern float no_pedal_thr;
extern float max_pedal_thr;
extern bool debug_state;
//define here the configuration parameters packed in a union, to be received via  CAN under request
union Start_params {
	struct {

		unsigned int no_pedal_output_throttle;
		unsigned int max_pedal_output_throttle;
		unsigned char pwm_dash;
		unsigned char valid_events;
		unsigned char config_byte;
		unsigned char command;
	}signal;
};
extern union Start_params Start_parameters;
//the error word/byte , if present, must be defined here
extern volatile union  word_def ped_ERROR_Word_1;
extern volatile union  byte_def ped_warning_byte;
extern volatile union  byte_def ped_info_byte;
//define the error flags
#define Flag_can_busoff			ped_ERROR_Word_1.b.b_0		// pedelec in bus off situation, obviosuly will not be in the displayed, but will be flashed
#define Flag_can_error			ped_ERROR_Word_1.b.b_1		//pedelec in error passive, max retransmit or other CAN error
#define Flag_main_task_error	ped_ERROR_Word_1.b.b_2		// main task has reached its time limit to perform. only reset by hard reset
#define Flag_throttle_error	    ped_ERROR_Word_1.b.b_3		// invalid throttle input measurement, out of range
#define Flag_thrtt_supply_over	ped_ERROR_Word_1.b.b_4		// invalid throttle supply measurement
#define Flag_thrtt_supply_under	ped_ERROR_Word_1.b.b_5		// invalid throttle supply measurement, undervoltage
#define Flag_can_tx_error		ped_ERROR_Word_1.b.b_6		// timeout transmitting via CAN, possible overload
#define Flag_no_bms_can_error	ped_ERROR_Word_1.b.b_7		// timeout of bms messaging
#define Flag_bms_digital_error	ped_ERROR_Word_1.b.b_8		// any error flag from the bms is set
#define Flag_no_contr_can_error	ped_ERROR_Word_1.b.b_9		// timeout of controller messaging
#define Flag_contr_alarm_error	ped_ERROR_Word_1.b.b_10		// any error flag from the controller is set
#define Flag_DAC_error			ped_ERROR_Word_1.b.b_11		// the DAC reached MAX_DAC_ERRORs consecutively, som problem in DAC
#define Flag_pw_supply_over		ped_ERROR_Word_1.b.b_12		// power supply overvoltage
#define Flag_pw_supply_under	ped_ERROR_Word_1.b.b_13		// power supply undervoltage
#define Flag_blank_flash_error	ped_ERROR_Word_1.b.b_14		// blank flash memory, problem reading flash mem at startup
#define Flag_flash_mem_error	ped_ERROR_Word_1.b.b_15		// an error reading flash memory has occurred, will try to read the backup mem
//define the warning flags
#define Flag_ped_motor_overtemp				ped_warning_byte.b.b_0		// temperature of the motor is high
#define Flag_ped_controller_overtemp		ped_warning_byte.b.b_1		// temperature of the controller is high
#define Flag_ped_battery_overtemp			ped_warning_byte.b.b_2		// temperature of the battery is high
// define the info bits
#define Flag_pedaling		ped_info_byte.b.b_0		//flag to indicate pedaling is sensed
#define Flag_breaking		ped_info_byte.b.b_1		// flag to indicate breaking is being applied
#define Flag_RB				ped_info_byte.b.b_2		// flag to indicate unlimited RB version
//declare here the text that each error will show in the warning strip, defined in variables.cpp. in this case, the text for ped_warning_byte flags is not used
extern const  char ped_can_error[];
extern const  char ped_sw_error[];
extern const  char ped_thr_in_error[];
extern const  char ped_thr_sup_1_error[];
extern const  char ped_thr_sup_2_error[];
extern const  char ped_can_tx_error[];
extern const  char ped_no_bms_can_error[];
extern const  char ped_bms_dig_error[];
extern const  char ped_no_contr_can_error[];
extern const  char ped_DAC_error[];
extern const  char ped_supply_over_error[];
extern const  char ped_supply_under_error[];
extern const  char ped_blank_flash_error[];
extern const  char ped_memory_error[];
//--------------------------------------------------------------------------------------------------------------------------------------------BMS values
extern float BMS_voltage;// = 0;
extern float BMS_current;// = 0;
extern unsigned int BMS_temp;// = 0;
extern unsigned int BMS_soc;// = 0;
extern unsigned int BMS_cycles;// = 0;
//the error word/byte , if present, must be defined here
extern volatile union  word_def BMS_digital_word;
//define the bms digital flags
#define Flag_bms_charging_state		BMS_digital_word.b.b_0		// 1 if charging
#define Flag_bms_overtemp			BMS_digital_word.b.b_1		// 1 if bms overtemp
#define Flag_bms_overcurrent		BMS_digital_word.b.b_2		// 1 if bms overcurrent
#define Flag_bms_cell_imbalance		BMS_digital_word.b.b_3		// 1 if imbalance between cells voltage dV > 0,5V
#define Flag_bms_cell_out_range		BMS_digital_word.b.b_4		// 1 if 2,8 > Uc > 4,2 V
#define Flag_bms_overvoltage		BMS_digital_word.b.b_5		// 1 if Battery pack overvoltage Ub > 58, 5V(14s)
#define Flag_bms_undervoltage		BMS_digital_word.b.b_6		// 1 if Battery pack overvoltage Ub v 40V(14s)
#define Flag_bms_comm_fault			BMS_digital_word.b.b_7		// 1 if BMS COM fault
#define Flag_bms_full_charge		BMS_digital_word.b.b_8		// 1 if Battery pack 100% Soc
#define Flag_bms_batt_kind			BMS_digital_word.b.b_9		// 1 Low Voltage (14s7p) or High Voltage(18s7p)
#define Flag_bms_no_comm			BMS_digital_word.b.b_10		// 1 if no com from bms
//declare here the text that each error will show in the warning strip, defined in variables.cpp. 
extern const  char bms_charging[];	
extern const  char bms_overtemp[];
extern const  char bms_overcurr[];
extern const  char bms_cell_imbalance[];
extern const  char bms_cell_oor[];
extern const  char bms_overvoltage[];
extern const  char bms_undervoltage[];
extern const  char bms_com_fault[];
extern const  char bms_lost_comm[];
//-----------------------------------------------------------------------------------------------------------------------------------------Controller values
extern float Cntr_voltage;// = 0;
//extern unsigned int Mot_temp;// = 0;
//extern unsigned int Cntr_temp;// = 0;
extern float Cntr_current;// = 0;
extern unsigned char Cntr_breaking;
extern float Cntr_ph_current_req;// = 0;
extern float Cntr_ph_current;// = 0;
//extern signed int Cntr_pi_res;// = 0;
//extern unsigned char Cntr_mod_index;// = 0;
extern float Cntr_power_input;// = 0;
extern unsigned int speed_int;
//extern unsigned long Cntr_odo;// = 0;
//extern unsigned int Cntr_trip;// = 0;
extern unsigned int Cntr_usage;// = 0;
//general params value
extern unsigned char contr_version_high;// = 0;
extern unsigned char contr_version_low;// = 0;
extern bool beta_version;// = 0;
//pi param values. These are the values that will be modifiable
extern unsigned char ki;
extern unsigned char kp;
extern unsigned char max_gate_var;
extern unsigned char bemf_trim;
//define here the configuration parameters packed in a union, to be received via  CAN under request
union Gen_params {

	struct {
		unsigned int Wheel_radius;
		unsigned int Switching_freq;
		unsigned char motor_cable_resistance;
		unsigned char version;
		unsigned char free1;

	}signal;
};
extern union Gen_params general_parameters;			// memory values to be saved
union Pi_params {

	struct {
		unsigned char ki;
		unsigned char kp;
		unsigned char max_gate_var;
		unsigned char bemf_trim;
		unsigned char free1;
		unsigned char free2;
		unsigned char free3;
	}signal;
};
extern union Pi_params Pi_parameters;			// memory values to be saved
//the error word/byte , if present, must be defined here
extern volatile union  word_def cnt_ERROR_Word_1, cnt_ERROR_Word_2, cnt_WARNING_Word_1;
//define the error flags
//-----------Flags in cnt_ERROR_Word_1:
#define Flag_Short_circuit_hw	cnt_ERROR_Word_1.b.b_0  ///< short circuit detected by hw
#define Flag_throttle_OL	cnt_ERROR_Word_1.b.b_1	///< throttle is not connected,or signal sc to ground
#define Flag_throttle_SC	cnt_ERROR_Word_1.b.b_2	///< short circuit throttle signal to vcc, or overvoltage
#define Flag_motor_overtemp	cnt_ERROR_Word_1.b.b_3	///< motor has reached max. temp.
#define Flag_motor_temp_err	cnt_ERROR_Word_1.b.b_4	///< problem with the motor ntc sensor
#define Flag_contr_overtemp	cnt_ERROR_Word_1.b.b_5	///< controller has reached max. temp.
#define Flag_contr_temp_err	cnt_ERROR_Word_1.b.b_6	///< problem with the controller ntc sensor
#define Flag_batt_overtemp	cnt_ERROR_Word_1.b.b_7	///< battery has reached max. temp.
#define Flag_OverVolt   	cnt_ERROR_Word_1.b.b_8	///< battery has reached max. voltage
#define Flag_UnderVolt 		cnt_ERROR_Word_1.b.b_9	///< battery has reached min voltage
#define Flag_Short_circuit	cnt_ERROR_Word_1.b.b_10	///< short circuit detected by sw
#define Flag_DC_overcurrent	cnt_ERROR_Word_1.b.b_11	///< battery overcurrent detected
#define Flag_WDT			cnt_ERROR_Word_1.b.b_12	///< watch dog timer detected, software problem bug
#define Flag_Low_5v			cnt_ERROR_Word_1.b.b_13	///< internal vdd below 3.8 volts
#define Flag_Clk_Stop		cnt_ERROR_Word_1.b.b_14	///< clock stopped and reoscillate.
#define Flag_sw_overflow	cnt_ERROR_Word_1.b.b_15	///< software overflow interruption

//-----------Flags in ERROR_Word_2:
#define Flag_und_instr   	cnt_ERROR_Word_2.b.b_0		///< undefined instruction in code
#define Flag_CAN_busoff  	cnt_ERROR_Word_2.b.b_1		///< CAN module of the controller is in bus off state
#define Flag_Hall_seq    	cnt_ERROR_Word_2.b.b_2		///< Hall error sequence error
#define Flag_Hall_error  	cnt_ERROR_Word_2.b.b_3		///< Hall sensor general error
#define Flag_blk_timeout 	cnt_ERROR_Word_2.b.b_4		///< timeout waiting hall sensor event!!!!!!!!! tbd
#define Flag_start_timeout 	cnt_ERROR_Word_2.b.b_5		///< timeout waiting for BM startup message
#define Flag_BMS_overtemp	cnt_ERROR_Word_2.b.b_6		///< overtemp flag from BMS, temp >60?
#define Flag_BMS_overcurr	cnt_ERROR_Word_2.b.b_7		///< overcurrent flag from BMS , I> 60A
#define Flag_BMS_overvolt	cnt_ERROR_Word_2.b.b_8		///< overvoltage flag from BMS, V> 58,5
#define Flag_BMS_undervolt	cnt_ERROR_Word_2.b.b_9		///< undervoltage flag from bms V< 40v
#define Flag_Mot_e2p_error  cnt_ERROR_Word_2.b.b_10      ///< some motor parameter in the e2p is found to be corrupted
#define Flag_soft_error  	cnt_ERROR_Word_2.b.b_11      ///< main task in software overtime

//-----------Flags in WARNING_Word_1:

#define Flag_motor_temp_lim		cnt_WARNING_Word_1.b.b_0	///< maximum torque is being limited because of motor temp
#define Flag_cont_temp_lim		cnt_WARNING_Word_1.b.b_1	///< maximum torque is being limited because of controller temp
#define Flag_batt_temp_lim		cnt_WARNING_Word_1.b.b_2	///< maximum torque is being limited because of batt. temp
#define Flag_batt_soc_lim		cnt_WARNING_Word_1.b.b_3	///< maximum torque is being limited because of battery SoC
#define Flag_DC_lim				cnt_WARNING_Word_1.b.b_4	///< maximum torque is being limited because of excessive DC current
#define Flag_DC_lv_lim			cnt_WARNING_Word_1.b.b_5	///< maximum torque is being limited because of too low dc voltage
#define Flag_CAN_Passive  		cnt_WARNING_Word_1.b.b_6	///< CAN module is error passive state, CAN bus is having trouble
#define Flag_CAN_Tx  	  		cnt_WARNING_Word_1.b.b_7	///< Problem in tx in any message
#define Flag_CAN_Rx  	  		cnt_WARNING_Word_1.b.b_8	///< problem receving any message
#define Flag_BKM_timeout  		cnt_WARNING_Word_1.b.b_9	///< bkm periodic message timeout
#define Flag_BMS_timeout  		cnt_WARNING_Word_1.b.b_10///< BMS periodic message timeout
#define Flag_e2p_mem_error		cnt_WARNING_Word_1.b.b_11///< some paremeter value out of range or not valid when reading e2p memory 
															/// at startup. default parameter value used instead.
#define Flag_BMS_cell_oor		cnt_WARNING_Word_1.b.b_12///< unbalance between cells or cell out of range. BMS info 
#define Flag_BMS_comm			cnt_WARNING_Word_1.b.b_13///< comunication falut with BMS
#define Flag_BMS_check			cnt_WARNING_Word_1.b.b_14///< unconsistent info from bms:
//declare here the text that each error will show in the warning strip, defined in variables.cpp. 
//controller error
extern const  char cnt_error[];	// used to concatenate

extern const  char cnt_Short_circuit_hw[];
extern const  char cnt_throttle_OL[];
extern const  char cnt_throttle_SC[];
extern const  char cnt_motor_overtemp[];
extern const  char cnt_motor_temp_err[];
extern const  char cnt_contr_overtemp[];
extern const  char cnt_contr_temp_err[];
extern const  char cnt_batt_overtemp[];
extern const  char cnt_OverVolt[];
extern const  char cnt_UnderVolt[];
extern const  char cnt_Short_circuit[];
extern const  char cnt_DC_overcurrent[];
extern const  char cnt_overcurrent[];
extern const  char cnt_WDT[];
extern const  char cnt_Low_5v[];
extern const  char cnt_Clk_Stop[];
extern const  char cnt_sw_overflow[];
extern const  char cnt_und_instr[];
extern const  char cnt_CAN_busoff[];
extern const  char cnt_Hall_seq[];
extern const  char cnt_Hall_error[];
extern const  char cnt_blk_timeout[];
extern const  char cnt_start_timeout[];
extern const  char cnt_BMS_overtemp[];
extern const  char cnt_BMS_overcurr[];
extern const  char cnt_BMS_overvolt[];
extern const  char cnt_BMS_undervolt[];
extern const  char cnt_Mot_e2p_error[];
extern const  char cnt_soft_error[];

//controller warnings
extern const char warning[];

extern const char warning_mot_temp[];
extern const char warning_contr_temp[];
extern const char warning_bms_temp[];
extern const char warning_Low_soc[];
extern const char warning_DC_current[];
extern const char warning_LV[];
extern const char warning_CAN_passive[];
extern const char warning_CAN_tx[];
extern const char warning_CAN_rx[];

//not static strings. define here the strings that will be used for displaying or sending via serial
//strings for displaying:
//extern String Speed_str;
/*always decalre the strings bearing in mind the last characters "\r\n" so, if need 3 bytes---> declare buffer of 5*/
extern char Speed_str[5];
//--bms
extern char Serial_BMS_Volt_str[7];  // to be sent via serial
extern char Display_BMS_Volt_str[7]; //to be displayed
extern char Serial_BMS_curr_str[7];  // to be sent via serial
extern char Display_BMS_curr_str[7]; //to be displayed
extern char BMS_temp_str[5]; //to be displayed
extern char BMS_soc_str[5]; 
extern char BMS_cycles_str[6];
//--cntr
extern char Serial_Cntr_Volt_str[7];  // to be sent via serial
extern char Display_Cntr_Volt_str[7]; //to be displayed
extern char Serial_Cntr_curr_str[7];  // to be sent via serial
extern char Display_Cntr_curr_str[7]; //to be displayed
extern char Mot_temp_str[5];
extern char Cntr_temp_str[5];
extern char Cntr_ph_current_req_str[5];
extern char Cntr_ph_current_str[5];
extern char ph_current_measured_arms_str[5];
extern char cnt_ERROR_Word_1_str[7];
extern char cnt_ERROR_Word_2_str[7];
extern char cnt_WARNING_Word_1_str[7];
extern char Cntr_breaking_str[3];
extern char Cntr_mod_index_str[5];
extern char Driving_mode_str[3];
extern char Cntr_trip_str[5];
//functions
/*the update parameters function must be created for all the editable parameters*/
void update_pi_params(void);
#endif

