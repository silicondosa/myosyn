// myosyn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <quickDAQ.h>
#include "..\include\myosyn.h"

unsigned muscle_mtr_val[16][2]		=  {{2, 0}, // Muscle Channel 0
										{2, 1}, // Muscle Channel 1
										{2, 2}, // Muscle Channel 2
										{2, 3}, // Muscle Channel 3
										{2, 4}, // Muscle Channel 4
										{2, 5}, // Muscle Channel 5
										{2, 6}, // Muscle Channel 6
										{2, 7}, // Muscle Channel 7
										{2, 8}, // Muscle Channel 8
										{2, 9}, // Muscle Channel 9
										{2,10}, // Muscle Channel 10
										{2,11}, // Muscle Channel 11
										{2,12}, // Muscle Channel 12
										{2,13}, // Muscle Channel 13
										{2,14}, // Muscle Channel 14
										{2,15}};// Muscle Channel 15

unsigned muscle_mtr_en [16][2]		=  {{2, 0},  // Muscle Channel 0
										{2, 1},  // Muscle Channel 1
										{2, 2},  // Muscle Channel 2
										{2, 3},  // Muscle Channel 3
										{2, 4},  // Muscle Channel 4
										{2, 5},  // Muscle Channel 5
										{2, 6},  // Muscle Channel 6
										{2, 7},  // Muscle Channel 7
										{2,16},  // Muscle Channel 8
										{2,17},  // Muscle Channel 9
										{2,18},  // Muscle Channel 10
										{2,19},  // Muscle Channel 11
										{2,20},  // Muscle Channel 12
										{2,21},  // Muscle Channel 13
										{2,22},  // Muscle Channel 14
										{2,23}}; // Muscle Channel 15

unsigned muscle_enc_ring [8][2]		=  {{3, 0},  // Muscle Channel 0
										{3, 1},  // Muscle Channel 1
										{3, 2},  // Muscle Channel 2
										{3, 3},  // Muscle Channel 3
										{3, 4},  // Muscle Channel 4
										{3, 5},  // Muscle Channel 5
										{3, 6},  // Muscle Channel 6
										{3, 7}}; // Muscle Channel 7

unsigned muscle_enc_kleo [8][2]		=  {{7, 0},  // Muscle Channel 0
										{7, 1},  // Muscle Channel 1
										{7, 2},  // Muscle Channel 2
										{7, 3},  // Muscle Channel 3
										{7, 4},  // Muscle Channel 4
										{7, 5},  // Muscle Channel 5
										{7, 6},  // Muscle Channel 6
										{7, 7}}; // Muscle Channel 7

unsigned muscle_enc_mtr[12][2]		=  {{3, 0},  // Muscle Channel 0
										{3, 2},  // Muscle Channel 1
										{3, 4},  // Muscle Channel 2
										{3, 6},  // Muscle Channel 3
										{7, 0},  // Muscle Channel 4
										{7, 2},  // Muscle Channel 5
										{7, 4},  // Muscle Channel 6
										{7, 6},  // Muscle Channel 7
										{8, 0},  // Muscle Channel 8
										{8, 2},  // Muscle Channel 9
										{8, 4},  // Muscle Channel 10
										{8, 6}}; // Muscle Channel 11

unsigned muscle_enc_spl[12][2]		=  {{3, 1},  // Muscle Channel 0
										{3, 3},  // Muscle Channel 1
										{3, 5},  // Muscle Channel 2
										{3, 7},  // Muscle Channel 3
										{7, 1},  // Muscle Channel 4
										{7, 3},  // Muscle Channel 5
										{7, 5},  // Muscle Channel 6
										{7, 7},  // Muscle Channel 7
										{8, 1},  // Muscle Channel 8
										{8, 3},  // Muscle Channel 9
										{8, 5},  // Muscle Channel 10
										{8, 7}}; // Muscle Channel 11

unsigned muscle_ld_cell [8][2]		=  {{5, 0},  // Muscle Channel 0
										{5, 8},  // Muscle Channel 1
										{5, 1},  // Muscle Channel 2
										{5, 9},  // Muscle Channel 3
										{5, 2},  // Muscle Channel 4
										{5,10},  // Muscle Channel 5
										{5,11},  // Muscle Channel 6
										{5, 3}}; // Muscle Channel 7

unsigned channel_limits[3][2] =		   {{ 7, 7},
										{12, 0},
										{12,12}};

int T5_quickDAQstatus = 0;
unsigned numConfiguredMuscles = 0;

void myosynStart() 
{
	switch (quickDAQgetStatus())
	{
	case STATUS_INIT:
		// Setup DAQ sampling rate and trigger mode
		setSampleClockTiming((samplingModes)HW_CLOCKED, DAQmxSamplingRate, DAQmxClockSource, (triggerModes)DAQmxTriggerEdge, DAQmxNumDataPointsPerSample, TRUE);

	case STATUS_READY:
		// Start quickDAQ - only need to call this once
		quickDAQstart();
	case STATUS_RUNNING:
		// you need to do nothing here except maybe print something
		break;
	default:
		// Print some error message
		break;
	}
}

void myosynStop() {
	switch (quickDAQgetStatus())
	{
	case STATUS_RUNNING:
		quickDAQstop();
		break;
	default:
		// print some error message?
		break;
	}
}

myosyn::myosyn()
{
	setMuscleStatus(MYOSYN_DISABLED);
}

myosyn::myosyn(unsigned muscleChannel, const DAQarrangement myDAQarrangement = MUSCLE_MODULE)
{
	// Initialize myosyn object basic settings
	this->channelID			= muscleChannel;
	this->myDAQarrangement	= myDAQarrangement;

	// Initialize quickDAQ library
	if (quickDAQgetStatus() == (int)STATUS_NASCENT) {
		quickDAQinit();
		T5_quickDAQstatus = 1;
	}
	startDAQ = &myosynStart;
	stopDAQ  = &myosynStop;

	// Acquire pin configurations based on DAQ arrangement
	maxChannels_enc		= 0;
	maxChannels_mtr		= 0;
	encoder_opt_config	= NULL;

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
		
		loadcell_config		= NULL;
		break;

	default:
		fprintf(ERRSTREAM, "ERROR: MyoSyn: Invalid DAQ arrangement selected. Program will terminate.");
		quickDAQTerminate();
		exit(-1);
		break;
	}

	// Set pin modes for all pins based on DAQ arrangement
	if (channelID < maxChannels_mtr) {
		// LOAD CELL INPUT
		if (this->myDAQarrangement == RING_OF_FIRE)
		{
			pinMode(loadcell_config[channelID][0], ANALOG_IN, loadcell_config[channelID][1]);
		}
		
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
	
	// Set muscle status and increment numActiveMuscles
	setMuscleStatus(MYOSYN_READY_WINDDOWN);
	numConfiguredMuscles++;
}

myosyn::~myosyn()
{
	if (numConfiguredMuscles > 0 && getMuscleStatus() != MYOSYN_DISABLED) {
		switch (getMuscleStatus()) {
		case MYOSYN_CLOSEDLOOP:
			// Call the function that shuts down closed loop controller
		case MYOSYN_ENABLED_WINDUP:
			windDown();
		default:
			this->status = MYOSYN_DISABLED;
		}
		if (numConfiguredMuscles == 1 && quickDAQgetStatus() > STATUS_NASCENT) {
			(*stopDAQ)();
			quickDAQTerminate();
			T5_quickDAQstatus = 0;
		}
		numConfiguredMuscles--;
	}
	setMuscleStatus(MYOSYN_DISABLED);
}

inline muscleStatus myosyn::getMuscleStatus()
{
	return this->status;
}

inline void myosyn::setMuscleStatus(muscleStatus newStatus)
{
	this->status = newStatus;
}

void myosyn::windUp()
{
	if (getMuscleStatus() == MYOSYN_READY_WINDDOWN) {
		(*startDAQ)();
		if (motor_enable_config[channelID][1] < 8) {
			writeDigitalPin(motor_enable_config[channelID][0], 0, motor_enable_config[channelID][1], TRUE);
		}
		else if (motor_enable_config[channelID][1] > 15 && motor_enable_config[channelID][1] < 24) {
			setAnalogOutPin(motor_enable_config[channelID][0], motor_enable_config[channelID][1], (float64)DIGITAL_HIGH_VOLTS);
			writeAnalog_intBuf(motor_enable_config[channelID][0]);
		}
		setAnalogOutPin(motor_value_config[channelID][0], motor_value_config[channelID][1], (float64)WIND_UP_VOLTS);
		writeAnalog_intBuf(motor_value_config[channelID][0]);
		setMuscleStatus(MYOSYN_ENABLED_WINDUP);
	}
	else {
		// print warning and reurn something?
	}
}

void myosyn::windDown()
{
	switch (getMuscleStatus())
	{
	case MYOSYN_CLOSEDLOOP:
		// do stuff pertaining to active closed loop control
	case MYOSYN_ENABLED_WINDUP:
		if (motor_enable_config[channelID][1] < 8) {
			writeDigitalPin(motor_enable_config[channelID][0], 0, motor_enable_config[channelID][1], FALSE);
		}
		else if (motor_enable_config[channelID][1] > 15 && motor_enable_config[channelID][1] < 24) {
			setAnalogOutPin(motor_enable_config[channelID][0], motor_enable_config[channelID][1], (float64)DIGITAL_LOW_VOLTS);
		}
		setAnalogOutPin(motor_value_config [channelID][0], motor_value_config [channelID][1], (float64)WIND_DOWN_VOLTS);
		writeAnalog_intBuf(motor_enable_config[channelID][0]);
		setMuscleStatus(MYOSYN_READY_WINDDOWN);
		if (numConfiguredMuscles == 1) {
			(*stopDAQ)();
		}
		break;
	default:
		// print error
		break;
	}
}

inline void myosyn::setMuscleToneValue(double myMuscleTone_value)
{
	this->muscleToneValue = myMuscleTone_value;
}

inline double myosyn::getMuscleToneValue()
{
	return this->muscleToneValue;
}

inline void myosyn::setMaxMuscleTension(double max_tension)
{
	this->maxMuscleTension = max_tension;
}

inline double myosyn::getMaxMuscleTension()
{
	return this->maxMuscleTension;
}

inline void myosyn::setMuscleTension(double refTension)
{
	this->refMuscleTension = refTension;
}

inline double myosyn::getMuscleTension()
{
	return this->refMuscleTension;
}

