#pragma once
#ifndef MYOSYN_H
#define MYOSYN_H

#include <iostream>
#include <quickDAQ.h>

#define DIGITAL_HIGH_VOLTS	5.0
#define DIGITAL_LOW_VOLTS	0.0
#define WIND_UP_VOLTS		0.8
#define WIND_DOWN_VOLTS		0.0

extern unsigned muscle_mtr_val[16][2];
extern unsigned muscle_mtr_en [16][2];
extern unsigned muscle_enc_ring[ 8][2];
extern unsigned muscle_enc_kleo[ 8][2];
extern unsigned muscle_enc_mtr[12][2];
extern unsigned muscle_enc_spl[12][2];
extern unsigned muscle_ld_cell[ 8][2];
extern int T5_quickDAQstatus;
extern unsigned numConfiguredMuscles;

typedef enum _DAQarrangement
{
	RING_OF_FIRE	= 0,
	QUADRUPED		= 1,
	MUSCLE_MODULE	= 2
}DAQarrangement;

typedef enum _muscleStatus
{
	MYOSYN_DISABLED			= 0,
	MYOSYN_READY_WINDDOWN	= 1,
	MYOSYN_ENABLED_WINDUP	= 2,
	MYOSYN_CLOSEDLOOP		= 3
} muscleStatus;

void myosynStart();
void myosynStop();

class myosyn {
	unsigned channelID;
	DAQarrangement myDAQarrangement;
	muscleStatus status;

	unsigned maxChannels_mtr, maxChannels_enc;

	unsigned (*motor_enable_config)[2];
	unsigned (*motor_value_config)[2];
	unsigned (*encoder_config)[2];
	unsigned (*encoder_opt_config)[2];
	unsigned (*loadcell_config)[2];

	// Muscle constant parameters for closed-loop control
	double muscleToneValue;
	double maxMuscleTension;

	double loadCell_offset, loadcell_gain; // Voltage to force readout
	double mtrTension_offset, mtrTension_gain; // Voltage to motor Tension

	double refMuscleTension;

public:
	// Constructor and destructor
	myosyn();
	myosyn(unsigned muscleChannel, const DAQarrangement myDAQarrangement);
	~myosyn();
	
	// myosyn support functions
	muscleStatus	getMuscleStatus();
	void			setMuscleStatus(muscleStatus newStatus);

	// DAQ support functions and function pointers
	void			(*startDAQ)();
	void			(*stopDAQ)();

	// Muscle open-loop operations
	void			windUp();
	void			windDown();

	// Functions for muscle closed-loop parameter settings
	void			setMuscleToneValue(double myMuscleTone_value);
	double			getMuscleToneValue();
	void			setMaxMuscleTension(double max_tension);
	double			getMaxMuscleTension();
		// Use these functions to set current closed-loop muscle tension refernce voltage in Newtons
	void			setMuscleTension(double myTension);
	double			getMuscleTension();
};

class T5encoder {
	T5encoder();
};


class T5control {
	T5control();
};

#endif // !MYOSYN_H

