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
	// Initialize T5muscle object
	unsigned channelID	= muscleChannel;
	this->myDAQarrangement		= myDAQarrangement;

	// Initialize quickDAQ library
	if (T5_quickDAQstatus == 0) {
		quickDAQinit();
		T5_quickDAQstatus = 1;
	}
	
	// Acquire pin numbers based on DAQ arrangement
	int encChannelLimit = 0;
	int mtrChannelLimit = 0;
	encoder_opt_config = NULL;
	switch (this->myDAQarrangement)
	{
	case RING_OF_FIRE:
		mtrChannelLimit		= 7;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		encChannelLimit		= 7;
		encoder_config		= muscle_enc_ring;
		encoder_opt_config	= NULL;
		loadcell_config		= muscle_ld_cell;
		break;

	case QUADRUPED:
		mtrChannelLimit		= 12;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		encChannelLimit		= 0;
		encoder_config		= muscle_enc_kleo;
		encoder_opt_config	= NULL;
		loadcell_config		= NULL;
		break;

	case MUSCLE_MODULE:
		mtrChannelLimit		= 12;
		motor_enable_config = muscle_mtr_en;
		motor_value_config	= muscle_mtr_val;
		encChannelLimit		= 12;
		encoder_config		= muscle_enc_mtr;
		encoder_opt_config	= muscle_enc_spl;
		break;

	default:
		fprintf(ERRSTREAM, "ERROR: T5 Control: Invalid DAQ arrangement selected. Program will terminate.");
		quickDAQTerminate();
		exit(-1);
		break;
	}

	// Set pin modes for all pins based on DAQ arrangement
	if (channelID < mtrChannelLimit) {
		// MOTOR VALUE OUTPUT
		pinMode((*motor_value_config)[0], ANALOG_OUT, (*motor_value_config)[1]);

		// MOTOR ENABLE (ACTIVE HIGH OUTPUT)
		if ((*motor_enable_config)[0] == 2) {
			if ((*motor_enable_config)[1] < 8) { // For digital output pins of NI-DAQmx
				pinMode((*motor_enable_config)[0], DIGITAL_OUT, 0);
			}
			else if ((*motor_enable_config)[1] > 15 && (*motor_enable_config)[1] < 24) { // For analog output pins of NI-DAQmx to behave like digital output pins
				pinMode((*motor_enable_config)[1], ANALOG_OUT, 0);
			}
		}

		// LOAD CELL INPUT
		if (this->myDAQarrangement == RING_OF_FIRE)
		{
			pinMode((*loadcell_config)[0], ANALOG_IN, (*loadcell_config)[1]);
		}

		// ENCODER INPUT
		if (this->myDAQarrangement != QUADRUPED)
		{
			// Main encoder
			pinMode((*encoder_config)[0], CTR_ANGLE_IN, (*encoder_config)[1]);

			// Optional encoder
			if (encoder_opt_config != NULL && this->myDAQarrangement == MUSCLE_MODULE) {
				pinMode((*encoder_opt_config)[0], CTR_ANGLE_IN, (*encoder_opt_config)[1]);
			}
		}
	}
	else {
		fprintf(ERRSTREAM, "ERROR: T5 Control: Requested channel ID %d is greater than the number of available motor channels %d. Program will terminate.", channelID, mtrChannelLimit);
		exit(-1);
	}
	// Setup DAQ sampling rate and trigger mode
}
