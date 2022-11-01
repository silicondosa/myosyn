// myosyn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <quickDAQ.h>
#include "..\include\myosyn.h"

// Needed by visual studio
#if defined(_WIN32) || defined(_WIN64)
	#include "..\include\pch.h"
	#include "..\include\framework.h"
#endif


extern unsigned muscle_mtr_val[16][2];
extern unsigned muscle_mtr_en [16][2];
extern unsigned muscle_enc_ring[ 8][2];
extern unsigned muscle_enc_kleo[ 8][2];
extern unsigned muscle_enc_mtr[12][2];
extern unsigned muscle_enc_spl[12][2];
extern unsigned muscle_ld_cell[ 8][2];
extern int T5_quickDAQstatus;

myosyn::myosyn(unsigned muscleChannel, const DAQarrangement myDAQarrangement = MUSCLE_MODULE)
{
	// Initialize myosyn object basic settings
	unsigned channelID	= muscleChannel;
	this->myDAQarrangement		= myDAQarrangement;

	// Initialize quickDAQ library
	if (T5_quickDAQstatus == 0) {
		quickDAQinit();
		T5_quickDAQstatus = 1;
	}
	
	// Acquire pin configurations based on DAQ arrangement
	maxChannels_enc = 0;
	maxChannels_mtr = 0;
	encoder_opt_config = NULL;

	switch (this->myDAQarrangement)
	{
	case RING_OF_FIRE:
		maxChannels_mtr		= 7;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		maxChannels_enc		= 7;
		encoder_config		= muscle_enc_ring;
		encoder_opt_config	= NULL;
		loadcell_config		= muscle_ld_cell;
		break;

	case QUADRUPED:
		maxChannels_mtr		= 12;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		maxChannels_enc		= 0;
		encoder_config		= muscle_enc_kleo;
		encoder_opt_config	= NULL;
		loadcell_config		= NULL;
		break;

	case MUSCLE_MODULE:
		maxChannels_mtr		= 12;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		maxChannels_enc		= 12;
		encoder_config		= muscle_enc_mtr;
		encoder_opt_config	= muscle_enc_spl;
		break;

	default:
		fprintf(ERRSTREAM, "ERROR: MyoSyn: Invalid DAQ arrangement selected. Program will terminate.");
		quickDAQTerminate();
		exit(-1);
		break;
	}

	// Set pin modes for all pins based on DAQ arrangement
	if (channelID < maxChannels_mtr) {
		// MOTOR VALUE OUTPUT
		pinMode(motor_value_config[channelID][0], ANALOG_OUT, motor_value_config[channelID][1]);

		// MOTOR ENABLE (ACTIVE HIGH OUTPUT)
		if (motor_enable_config[channelID][0] == 2) {
			if (motor_enable_config[channelID][1] < 8) { // For digital output pins of NI-DAQmx
				pinMode(motor_enable_config[channelID][0], DIGITAL_OUT, 0);
			}
			else if (motor_enable_config[channelID][1] > 15 && motor_enable_config[channelID][1] < 24) { // For analog output pins of NI-DAQmx to behave like digital output pins
				pinMode(motor_enable_config[channelID][0], ANALOG_OUT, motor_enable_config[channelID][1]);
			}
		}

		// LOAD CELL INPUT
		if (this->myDAQarrangement == RING_OF_FIRE)
		{
			pinMode(loadcell_config[channelID][0], ANALOG_IN, loadcell_config[channelID][1]);
		}

		// ENCODER INPUT
		if (this->myDAQarrangement != QUADRUPED)
		{
			// Main encoder
			pinMode(encoder_config[channelID][0], CTR_ANGLE_IN, encoder_config[channelID][1]);

			// Optional encoder
			if (encoder_opt_config != NULL && this->myDAQarrangement == MUSCLE_MODULE) {
				pinMode(encoder_opt_config[channelID][0], CTR_ANGLE_IN, encoder_opt_config[channelID][1]);
			}
		}
	}
	else {
		fprintf(ERRSTREAM, "ERROR: MyoSyn: Requested channel ID %d is greater than the number of available motor channels %d. Program will terminate.", channelID, maxChannels_mtr);
		exit(-1);
	}
	// Setup DAQ sampling rate and trigger mode
}
