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

/*in this file, the function check_task_CAN() is defined. TBR*/

#include "global.h"

//instantiate object
MCP_CAN CAN(SPI_CS_PIN);

//local vars
unsigned int aux1 = 0;

void Check_CAN_Task(void)
{
	/*This task will continuosly check if a an message is in the buffer, and if so, will  extract its info to the correspondent variables.
	 Since it is a full duplex communication, it will send can messages requesting , in a timed fashion, some info needed from other nodes. This info is needed only once(sw version, etc..)
	 so, when this info will be received, it will stop sending messages.
	 every line is TBR if nodes are changing


	*/
	// local variables
	int i = 0;
	byte len = 0;
	byte buf[8];
	byte aux_byte = 0;
	long aux_long = 0;

	//--------------------------------------------------------------------------------------------------------------------check if its needed to send CAN messages
		/*check, in order of importance, if a message needs to be sent*/
		// check if missing config params from the controller node
	if ((!Flag_controler_info_received) && (Flag_CAN_request))
	{

		if ((Pi_parameters.signal.ki == 0x00))	// next one to get pi params from controller
		{
			TX_msg_CNTR_COMMAND.tx_message[0] = SEND_PI_PARAMETERS_CNTR;
			CAN.sendMsgBuf(CCP_CRO_ID, 0, TX_cntr_dlc, TX_msg_CNTR_COMMAND.tx_message);
			Flag_CAN_request = 0;//reset flag
		}
		else if (general_parameters.signal.version == 0x00)
		{
			TX_msg_CNTR_COMMAND.tx_message[0] = SEND_GEN_PARAMETERS;
			CAN.sendMsgBuf(CCP_CRO_ID, 0, TX_cntr_dlc, TX_msg_CNTR_COMMAND.tx_message);
			Flag_CAN_request = 0;//reset flag
		}
	}
	//check if a CAN message needs to be sent to the pedelec node
	else if ((!Flag_no_pedelec_can) && (!Flag_pedelec_info_received) && (Flag_CAN_request))
	{
		// if there is no pedelec, do not overload the bus; send only if pedelec startup info no received, and send it in a timed fashion.
		Flag_CAN_request = 0;//reset flag
		TX_msg_PEDELEC_COMMAND.tx_message[0] = SEND_START_PARAMETERS;
		len = TX_pedelec_dlc;
		CAN.sendMsgBuf(pedelec_command_ID, 0, len, TX_msg_PEDELEC_COMMAND.tx_message);

	}
	//check if some config message has to be sent
	else if ((send_config_CAN_mssg) && (Flag_CAN_request))
	{
		//check what to change
		if (reconfigure_pedelec)
		{
			// swap rb to pedelec
			config_byte.byte = Start_parameters.signal.config_byte;
			config_byte.b.b_6 = ~(config_byte.b.b_6);	// swap rb
			Start_parameters.signal.config_byte = config_byte.byte;

		}

		else if (change_ki)
		{
			Pi_parameters.signal.ki = ki; //update 
			change_ki = 0;
		}
		else if (change_kp)
		{
			Pi_parameters.signal.kp = kp;
			change_kp = 0;
		}
		else if (change_max_gate_var)
		{
			Pi_parameters.signal.max_gate_var = max_gate_var;
			change_max_gate_var = 0;
		}
		else if (change_bemf_trim)
		{
			Pi_parameters.signal.bemf_trim = bemf_trim;
			change_bemf_trim = 0;
		}

		// now send the messages, one at a time
		if (reconfigure_pedelec)//start_parameters
		{
			memcpy(&(TX_msg_PEDELEC_COMMAND.tx_message[1]), &Start_parameters, sizeof(Start_parameters));
			TX_msg_PEDELEC_COMMAND.tx_message[0] = RECEIVE_START_PARAMETERS;
			CAN.sendMsgBuf(0x140, 0, TX_pedelec_dlc, TX_msg_PEDELEC_COMMAND.tx_message);	// send start parameters
			reconfigure_pedelec = 0;//reset flag
			Flag_CAN_request = 0;//reset flag
			send_config_CAN_mssg = 0;	// reset flag, job done
		}
		else if (current_screen_num == 5)//pi_parameters
		{
			memcpy(&(TX_msg_CNTR_COMMAND.tx_message[1]), &Pi_parameters, sizeof(Pi_parameters));
			TX_msg_CNTR_COMMAND.tx_message[0] = SAVE_PI_PARAMETERS_CNTR;

			CAN.sendMsgBuf(CCP_CRO_ID, 0, TX_cntr_dlc, TX_msg_CNTR_COMMAND.tx_message);	// send start parameters
			send_config_CAN_mssg = 0;	// reset flag, job done
			Flag_CAN_request = 0;//reset flag

		}


	}

	//----------------------------------------------------------------------------------------------------------------------------check if some CAN message arrived
	if (CAN_MSGAVAIL == CAN.checkReceive())
	{

		CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
		unsigned int canId = CAN.getCanId();

		//process the recieved CAN message
		switch (canId)
		{
			//---------------------------------------------------------------------------------------------------messages answering request
		case pedelec_answer_ID:
		{
			if ((TX_msg_PEDELEC_COMMAND.tx_message[0] == SEND_START_PARAMETERS)
				|| (TX_msg_PEDELEC_COMMAND.tx_message[0] == RECEIVE_START_PARAMETERS))// just sent a request, process answer
			{

				memcpy(&Start_parameters, buf, 8);

				config_byte.byte = Start_parameters.signal.config_byte;
				debug_state = config_byte.b.b_7;
				if (debug_state)
				{
					strcpy_P(received_number, (On_str));
				}
				else
				{
					strcpy_P(received_number, (Off_str));
				}
				

				if (config_byte.b.b_6)		//rb version
				{
					max_speed = 0;
					strcpy_P(received_number, (Off_str));
				}
				else
				{
					max_speed = config_byte.byte & 0x3f;
					dtostrf(max_speed, 2, 0, received_number);
				}

				pedal_events = (Start_parameters.signal.valid_events);
				
				//prepare message to be sent, little endian
				Start_parameters.signal.no_pedal_output_throttle = little_endian_int(Start_parameters.signal.no_pedal_output_throttle);
				no_pedal_thr = Start_parameters.signal.no_pedal_output_throttle*0.1;
				

				Start_parameters.signal.max_pedal_output_throttle = little_endian_int(Start_parameters.signal.max_pedal_output_throttle);
				max_pedal_thr = Start_parameters.signal.max_pedal_output_throttle*0.1;
				

				Flag_pedelec_info_received = 1;  //set flag so that we have the needed startup info
			}
			break;
		}
		case CCP_DTO_ID:
		{
			if ((TX_msg_CNTR_COMMAND.tx_message[0] == SEND_PI_PARAMETERS_CNTR)
				|| (TX_msg_CNTR_COMMAND.tx_message[0] == SAVE_PI_PARAMETERS_CNTR))
			{
				memcpy(&Pi_parameters, buf, 8);
				
				update_pi_params();
				
			
			}
			else if (TX_msg_CNTR_COMMAND.tx_message[0] == SEND_GEN_PARAMETERS)
			{
				memcpy(&general_parameters, buf, 8);//copy the stuff

				aux_byte = general_parameters.signal.version;


				if ((aux_byte & 0x80))
				{
					beta_version = 1;//this is a beta version

				}
				aux_byte = general_parameters.signal.version;//read again
				contr_version_high = (aux_byte & 0x70) >> 4;
				aux_byte = general_parameters.signal.version;
				contr_version_low = (aux_byte & 0x0f);

				Flag_controler_info_received = 1; //set flag so that no more requests are sent
			}
			break;
		}
		//-------------------------------------------------------pedelec-node---------------------------------------------
		case PEDELEC_1_ID:
		{

			{
				Flag_ped_can_wdt = 1;		// set wdt
				Flag_no_pedelec_can = 0;	//reset flag


			}
			break;
		}
		case PEDELEC_2_ID:
		{

			{
				memcpy(TX_pedelec_brdcast_2.tx_message, buf, 8);

				ped_ERROR_Word_1.word = TX_pedelec_brdcast_2.signal.Error1;
				ped_warning_byte.byte = TX_pedelec_brdcast_2.signal.warning;
				ped_info_byte.byte = TX_pedelec_brdcast_2.signal.info;

				if (Flag_RB)
				{
					speed_color = YELLOW;
				}
				else
				{
					speed_color = WHITE;
				}

			}
			break;
		}
		/////////----------------------------------------------------------------BMS node-------------------------------
		case bms_analog_ID:
		{

			{
				Flag_bms_can_wdt = 1;		// set wdt
				Flag_no_bms_can = 0;	//reset flag

				memcpy(RX_msg_BMS_analog.received_message, buf, 8);

				BMS_voltage = (RX_msg_BMS_analog.signal.voltage)*bms_voltage_factor;
				//update strings
				//bear in mind the number of decimals you want
			    dtostrf(BMS_voltage, 4, 1, Display_BMS_Volt_str);
				dtostrf(RX_msg_BMS_analog.signal.voltage, 4, 0, Serial_BMS_Volt_str);

				BMS_current = (RX_msg_BMS_analog.signal.current)*bms_current_factor;
				//update strings
				//bear in mind the number of decimals you want
				dtostrf(BMS_current, 4, 1, Display_BMS_curr_str);
				dtostrf(RX_msg_BMS_analog.signal.current, 4, 0, Serial_BMS_curr_str);

				BMS_temp = (RX_msg_BMS_analog.signal.temperature)*bms_temp_factor;
				dtostrf(BMS_temp,2, 0, BMS_temp_str);

				BMS_soc = (RX_msg_BMS_analog.signal.SoC)*bms_soc_factor;
				dtostrf(BMS_soc, 3, 0, BMS_soc_str);

			}
			break;
		}
		case bms_analog2_ID:
		{

			{
				if (!analog_2_received)
				{
					memcpy(RX_msg_BMS_analog2.received_message, buf, 8);

					BMS_cycles = RX_msg_BMS_analog2.signal.cicles;
					dtostrf(BMS_cycles, 4, 0, BMS_cycles_str);

					if (BMS_cycles != 0)	// this will not work with a brand new batt, or a CAN less batt! but it is ok, not too harmful.
					{
						analog_2_received = 1;// set flag, do it just once, when a valid num is received
					}



				}
			}
			break;
		}
		case bms_digital_ID:
		{

			{
				Flag_bms_can_wdt = 1;		// set wdt
				Flag_no_bms_can = 0;	//reset flag
				memcpy(RX_msg_BMS_digital.received_message, buf, 8);
				BMS_digital_word.word = RX_msg_BMS_digital.signal.digital_flags;
				//chekc the last 8 bits 
				if (BMS_digital_word.word & 0x04ff)
				{
					Flag_bms_digital_error = 1;
					aux1 = 0;//error counter
					//populate the string buffer that will be shown in the sreen with no fixed titles
					if (Flag_bms_charging_state)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_charging));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_overtemp)
					{

						aux1++;
						strcpy_P(received_number_aux_2, (bms_overtemp));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_overcurrent)
					{

						aux1++;
						strcpy_P(received_number_aux_2, (bms_overcurr));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_cell_imbalance)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_cell_imbalance));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_cell_out_range)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_cell_oor));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_overvoltage)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_overvoltage));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_undervoltage)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_undervoltage));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_comm_fault)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_com_fault));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}
					if (Flag_bms_no_comm)
					{
						aux1++;
						strcpy_P(received_number_aux_2, (bms_lost_comm));
						BMS_digital_str[aux1 - 1] = (String)received_number_aux_2;
					}


				}
				else
				{
					Flag_bms_digital_error = 0;
				}


			}
			break;
		}
		//--------------------------------------------------------------------------controller node-----------------
		case controller_0_ID:
		{

			{
				Flag_controller_can_wdt = 1;		// set wdt
				Flag_no_controller_can = 0;	//reset flag
				memcpy(RX_Ctl_brdcst_0.rx_message, buf, 8);

				speed_int = RX_Ctl_brdcst_0.signal.Vehicle_speed;
				//Cntr_odo = RX_Ctl_brdcst_0.signal.Mileage;
			//	Cntr_trip = RX_Ctl_brdcst_0.signal.Trip;
				//Cntr_usage = RX_Ctl_brdcst_0.signal.working_hours;

				//update the strings
				dtostrf(speed_int, 2, 0, Speed_str);
				dtostrf(RX_Ctl_brdcst_0.signal.Trip, 3, 0, Cntr_trip_str);
				dtostrf(RX_Ctl_brdcst_0.signal.Driving_mode, 1, 0, Driving_mode_str);

			}
			break;
		}
		case controller_1_ID:
		{

			{
				memcpy(RX_Ctl_brdcst_1.rx_message, buf, 8);
				cnt_ERROR_Word_1.word = RX_Ctl_brdcst_1.signal.ERROR_word_1;
				cnt_ERROR_Word_2.word = RX_Ctl_brdcst_1.signal.ERROR_word_2;

				//build the strings to send info via serial
				dtostrf(cnt_ERROR_Word_1.word, 5, 0, cnt_ERROR_Word_1_str);
				dtostrf(cnt_ERROR_Word_2.word, 5, 0, cnt_ERROR_Word_2_str);

			}
			break;
		}
		case controller_2_ID:
		{

			{
				memcpy(RX_Ctl_brdcst_2.rx_message, buf, 8);
				cnt_WARNING_Word_1.word = RX_Ctl_brdcst_2.signal.WARNING_word_1;
				cnt_WARNING_Word_1.word = cnt_WARNING_Word_1.word & 0xf7ff;//avoid some error in e2p mem reading
				//build the strings to send info via serial
				dtostrf(cnt_WARNING_Word_1.word, 5, 0, cnt_WARNING_Word_1_str);

			}
			break;
		}
		case controller_3_ID:
		{

			{
				memcpy(RX_Ctl_brdcst_3.rx_message, buf, 8);

				Cntr_voltage = (RX_Ctl_brdcst_3.signal.Input_voltage)*Input_voltage_factor;
				//Mot_temp = RX_Ctl_brdcst_3.signal.Motor_temp;
				//Cntr_temp = RX_Ctl_brdcst_3.signal.Controller_temp;
				Cntr_current = (RX_Ctl_brdcst_3.signal.Input_current)*Input_current_factor;

				//update strings
				//bear in mind the number of decimals you want
				dtostrf(Cntr_voltage, 4, 1, Display_Cntr_Volt_str);
				dtostrf(RX_Ctl_brdcst_3.signal.Input_voltage, 3, 0, Serial_Cntr_Volt_str);

				dtostrf(Cntr_current, 4, 1, Display_Cntr_curr_str);
				dtostrf(RX_Ctl_brdcst_3.signal.Input_current, 3, 0, Serial_Cntr_curr_str);
				
				dtostrf(RX_Ctl_brdcst_3.signal.Motor_temp,3,0, Mot_temp_str);
				dtostrf(RX_Ctl_brdcst_3.signal.Controller_temp, 3, 0, Cntr_temp_str);

			}
			break;
		}
		case controller_4_ID:
		{

			{
				memcpy(RX_Ctl_brdcst_4.rx_message, buf, 8);

				Cntr_breaking = RX_Ctl_brdcst_4.signal.brake_state;
				dtostrf(Cntr_breaking, 1, 0, Cntr_breaking_str);

			}
			break;
		}
		case controller_5_ID:
		{

			{
				memcpy(RX_Ctl_brdcst_5.rx_message, buf, 8);
				//Cntr_mod_index = RX_Ctl_brdcst_5.signal.modulation_index_bc;
				Cntr_power_input = RX_Ctl_brdcst_5.signal.input_power*0.001;

				//update strings
				dtostrf(RX_Ctl_brdcst_5.signal.modulation_index_bc, 3, 0, Cntr_mod_index_str);	//36 int, up to 1
			}

			break;
		}
		case controller_6_ID:
		{
			{
				memcpy(RX_Ctl_brdcst_6.rx_message, buf, 8);
				Cntr_ph_current_req = RX_Ctl_brdcst_6.signal.current_req*current_req_factor;
				Cntr_ph_current = RX_Ctl_brdcst_6.signal.current_out*current_out_factor;
				//Cntr_pi_res = RX_Ctl_brdcst_6.signal.pi_reg;

				//measure the phase current at the same time aprox the message is received
				current_read_value = analogRead(pinCurrentSense);
				aux_long = current_read_value * max_current_ARMS;
				ph_current_measured_arms = aux_long / 1024;// 10 bits resolution a/d

				//update strings
				dtostrf(Cntr_ph_current_req, 3, 0, Cntr_ph_current_req_str);
				dtostrf(Cntr_ph_current, 3, 0, Cntr_ph_current_str);
				dtostrf(ph_current_measured_arms, 3, 0, ph_current_measured_arms_str);

			}
			break;
		}
		default:
			break;
		}

	}
	else
	{
		/* do nothing*/
	}
}