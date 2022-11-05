#pragma once
#ifndef MYOSYN_H
#define MYOSYN_H

#include <iostream>
#include <quickDAQ.h>

#define eprintf(_STR, ...) fprintf(ERRSTREAM, _STR, __VA_ARGS__)

#define DIGITAL_HIGH_VOLTS	5.0
#define DIGITAL_LOW_VOLTS	0.0
#define WIND_UP_VOLTS		0.8
#define WIND_DOWN_VOLTS		0.0
#define DEFAULT_MUSCLE_TONE 1.0		// Newtons
#define MAX_MUSCLE_TENSION	30.0	// Newtons
#define MOTOR_SHAFT_DIAMTR	6		// millimeters
#define LOADCELL_TARE_NSAMP 100

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

// myosyn global variables
//extern int T5_quickDAQstatus;
extern DAQarrangement myosynConfiguration;
extern unsigned numConfiguredMuscles;
extern int myosynLeader;
extern double myosyn_samplingRate_global;

// Some global reconfigurable DAQ configuration/pin mapping variables
extern unsigned muscle_mtr_val	[16][2];
extern unsigned muscle_mtr_en	[16][2];
extern unsigned muscle_enc_ring	[ 8][2];
extern unsigned muscle_enc_kleo	[ 8][2];
extern unsigned muscle_enc_mtr	[12][2];
extern unsigned muscle_enc_spl	[12][2];
extern unsigned muscle_ld_cell	[ 8][2];
extern double	loadcell_calib	[ 8][2]; // gain, bias for each of 8 load cells
extern double	mtr_trque_calib	[16][2]; // gain, bias for each of 8 load cells

// Global myosyn functions
unsigned myosynNumMuscles();
DAQarrangement myosynGetConfiguration();
void myosynSetConfiguration(DAQarrangement DAQconfiguration);
double myosynSamplingRate(double newSamplingRate = myosyn_samplingRate_global);
void myosynStart();
void myosynStop();
int myosynGetLeader();
void myosynReadInputs();
void myosynWriteOutputs();
void myosynWaitForClock();

// MyoSyn class
class myosyn {
	unsigned channelID;
	DAQarrangement* myDAQarrangement;
	muscleStatus status;

	unsigned maxChannels_mtr, maxChannels_enc;

	unsigned (*motor_enable_config)[2];
	unsigned (*motor_value_config)[2];
	unsigned (*encoder_config)[2];
	unsigned (*encoder_opt_config)[2];
	unsigned (*loadcell_config)[2];

	// Muscle constant parameters for closed-loop control
	double muscleToneTension;
	double maxMuscleTension;

	//Sensor and motor calibration parameters
	double loadcell_offset, loadcell_gain; // Voltage to force calibration for loadcell
	double encoder_offset_angle; // zero out encoder angle after windup
	double encoder_angle_to_excursion_ratio; // Angle to tendon excursion calibration for encoder

	double mtrTension_offset, mtrTension_gain; // Voltage to motor Tension

	// Live update variables
	double refMuscleTension;
	double myMuscleTension;

public:
	// Constructors and destructor
	myosyn();
	myosyn(unsigned muscleChannel);
	~myosyn();
	
	// myosyn support functions
	unsigned		getChannelID();
	muscleStatus	getMuscleStatus();
	void			setMuscleStatus(muscleStatus newStatus);

	// DAQ support functions and function pointers
	void			(*startDAQ)();
	void			(*stopDAQ)();

	// Muscle open-loop operations
	void			windUp();
	void			windDown();

	// Functions for muscle closed-loop parameter settings
	void			setMuscleToneTension(double myMuscleTone_value);
	double			getMuscleToneTension();
	void			setMaxMuscleTension(double max_tension);
	double			getMaxMuscleTension();
		// Use these functions to set current closed-loop muscle tension refernce voltage in Newtons
	void			setReferenceTension(double myTension);
	double			getReferenceTension();

	// Functions to read and scale sensor data
	void			calibrateTension(double loadCellGain = NAN);
	double			loadcellV2F(double LCvoltage);
	void			readMuscleTension();
	double			getMuscleTension();
	
	void			calibrateExcursion(double spoolDiameter = NAN); // diameter in mm
	double			encoderAngle2Excursion(double encoderAngle);
	void			readTendonExcursion();
	double			getTendonExcursion();
};

class T5encoder {
	T5encoder();
};


class T5control {
	T5control();
};

#endif // !MYOSYN_H

