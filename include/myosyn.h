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

/**
 * @brief Enumerates the different types of configurations supported by myosyn
*/
typedef enum _DAQarrangement
{
	RING_OF_FIRE	= 0,
	QUADRUPED		= 1,
	MUSCLE_MODULE	= 2
}DAQarrangement;

/**
 * @brief This ENUM lists the possible statuses of each muscle channel.
*/
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

/**
 * @brief Returns the number of muscles configured by the library.
 * @return Unsigned integer number of muscles.
*/
unsigned myosynNumMuscles();

/**
 * @brief Returns the muscle configuration type to which the myosyn library is configured from the possible types of configurations are listed in @enum DAQarrangement.
 * @return Returns the muscle configuration/arrangement of the library - the possibilities are listed in @enum DAQarrangement.
*/
DAQarrangement myosynGetConfiguration();

/**
 * @brief Set the muscle configuration/arrangement type of the library. List of possibilities in the \ref DAQarrangement enum.
 * @param DAQconfiguration The muscle configuration type (from the \ref DAQarrangement enum) to which the library muscle be set.
*/
void myosynSetConfiguration(DAQarrangement DAQconfiguration);

/**
 * @brief Sets the sampling rate of the data acquisition and controller of all the muscle channels. The default rate is 1000Hz.
 * @param newSamplingRate The global sampling rate of the DAQ subsystem. Cannot be overriden.
 * @return If successful, returns the positive sampling rate that was set. Otherwise, it returns a negative error code.
*/
double myosynSamplingRate(double newSamplingRate = myosyn_samplingRate_global);

/**
 * @brief This routine starts the data acquisition on all muscle channels that haven't already been started so long as at least one channel has been configured.
*/
void myosynStart();

/**
 * @brief This function sops the data acquisition of all muscle channels that were started (ie. active).
*/
void myosynStop();

/**
 * @brief Sets the leader/master muscle channel which governs/triggers data acquisition on all other muscles.
 * @param newLeader The muscle channel which needs to be set as a leader.
*/
void myosynSetLeaderChannel(int newLeader);

/**
 * @brief Returns the leader muscle channel which governs/triggers data acquisition on all other muscles.
 * @return If successful, the postive/zero leader muscle channel number. Otherwise, a negative error code.
*/
int myosynGetLeaderChannel();

/**
 * @brief Global function to read all DAQ inputs in a synchronized manner. The results are put into the class objects of each muscle channel.
*/
void myosynReadInputs();

/**
 * @brief Global function to write all DAQ outputs in a synchronized manner. The results are put into the class objects of each muscle channel.
*/
void myosynWriteOutputs();

/**
 * @brief This blocking function waits for the clock period to finish between two function calls.
 * So, if the sampling period is 1ms, the second call of this function will block until 1ms has passed betwen two function calls.
*/
void myosynWaitForClock();

// MyoSyn class

/**
 * @brief This is the class object for each muscle channel.
*/
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

	// Controller variables
	double motorCommand;

public:
	// Constructors and destructor

	/**
	 * @brief Default constructor of the \ref myosyn class. Auto-selects muscle channel when setting up a muscle channel.
	*/
	myosyn();
	
	/**
	 * @brief Constructor for the @ref myosyn class.
	  * User provides the channel number to configure and if it is yet to be configured, 
	  * the muscle channel is setup and the class object is initialized with the setting and channel ID number.
	 * @param muscleChannel The channel number/ID of the muscle channel that is to be configured.
	*/
	myosyn(unsigned muscleChannel);

	~myosyn();
	
	// myosyn support functions

	/**
	 * @brief Returns the channel ID number of the muscle channel.
	 * @return The channel number of the muscle (values greater than or equal to 0 possible).
	*/
	unsigned		getChannelID();
	
	/**
	 * @brief Returns the status of the muscle channel. The possible statuses are listed in the enum \ref muscleStatus.
	 * @return Returns the muscle status as a \ref muscleStatus type-defined enum.
	*/
	muscleStatus	getMuscleStatus();

	/**
	 * @brief Function sets the status of the muscle channel object.
	 * @param newStatus The new status of the object of type @ref muscleStatus .
	*/
	void			setMuscleStatus(muscleStatus newStatus);

	// DAQ support functions and function pointers

	/**
	 * @brief Function pointer to the global @ref myosynStart function to start data acquisition.
	 * Only need to call this for one of the class objects.
	*/
	void			(*startDAQ)();
	
	/**
	 * @brief @brief Function pointer to the global @ref myosynStop function to stop data acquisition.
	 * Only need to call this for one of the class objects.
	*/
	void			(*stopDAQ)();

	// Muscle open-loop operations
	
	/**
	 * @brief Function starts/energizes the motor of the muscle channel to wind up the tendons to prevent slack.
	*/
	void			windUp();

	/**
	 * @brief Function stops/deenergizes the motor of the muscle channel to wind down the tendons before system shutdown.
	*/
	void			windDown();

	// Functions for muscle closed-loop parameter settings
	
	/**
	 * @brief Sets the minimum tension held by the motor of a muscle channel.
	  * Corresponds to muscle tone and is the lower bound of possible muscle tension values.
	 * @param myMuscleTone_value Muscle tone and minimum muscle tension allowed (in newtons).
	*/
	void			setMuscleToneTension(double myMuscleTone_value);

	/**
	 * @brief Returns the current muscle tone of the muscle channel
	 * @return The currently set muscle tone of the channel (in newtons).
	*/
	double			getMuscleToneTension();

	/**
	 * @brief Sets the upper limit to possible muscle tension of the motor of the muscle channel as a protection.
	 * @param max_tension The upper limit (in newtons) of the muscle tension of the motor.
	*/
	void			setMaxMuscleTension(double max_tension);

	/**
	 * @brief Returns the upper limit of possible muscle tensions by the motor of the muscle channel.
	 * @return The upper limit (in newtons) of the motor of the muscle channel.
	*/
	double			getMaxMuscleTension();

	/**
	 * @brief The current reference/target muscle tension which the internal closed-loop controller will attempt to achieve on the motor.
	 * @param myTension The reference/target muscle tension (in newtons) which the motor controller should try to achieve.
	*/
	void			setReferenceTension(double myTension);

	/**
	 * @brief Returns the current reference/target muscle tension that the motor controller is trying to achieve/maintain.
	 * @return The current reference/target muscle tension (in newtons) that the motor controller is trying to achieve/maintain.

	*/
	double			getReferenceTension();

	// Functions to read and scale sensor data

	/**
	 * @brief Sets the gain to calibrate the transformation of the loadcell voltage to force in newtons.
	 * Also winds up and reads the load cell's initial bias voltage to tare the loadcell.
	 * @param loadCellGain The gain value (units: N/V)
	*/
	void			calibrateTension(double loadCellGain = NAN);

	/**
	 * @brief Returns the force on the loadcell when supplied with the voltage based on the gain and bias set by \ref calibrateTension .
	 * @param LCvoltage The voltage value to be converted to newtons.
	 * @return The force/tension (in newtons) corresponding to the voltage supplied.
	*/
	double			loadcellV2F(double LCvoltage);

	/**
	 * @brief Uses the DAQ subsystem to read the muscle tension detected by the loadcell and puts it in the internal memory.
	 * Use \ref getMuscleTension to read the value in the internal memory.
	*/
	void			readMuscleTension();

	/**
	 * @brief Returns the current muscle tension value (in newtons) in the internal memory.
	 * @return The current muscle tension value (in newtons) stored in the muscle channel object.
	*/
	double			getMuscleTension();

	/**
	 * @brief Calibrates the rotary incremental shaft encoder to zero degrees and set the conversion from angle to excursion.
	 * This is done by using the diameter of the shaft that spools the tendon. 360 degrees of rotation corresponds to one shaft circumference of excursion.
	 * @param spoolDiameter The diameter of the shaft (in mm) that is used to spool the tendon. 
	*/
	void			calibrateExcursion(double spoolDiameter = NAN);

	/**
	 * @brief Returns the tendon excursion given an encoder angle.
	 * @param encoderAngle Encoder angle (in degrees).
	 * @return Tendon excursion (in mm).
	*/
	double			encoderAngle2Excursion(double encoderAngle);

	/**
	 * @brief Uses the DAQ subsystem to read the tendon excursion detected by the shaft encoder and puts it in the internal memory.

	*/
	void			readTendonExcursion();
	
	/**
	 * @brief Returns the current tendon excursion value (in mm) in the internal memory.
	 * @return The current tendon excursion value (in mm) stored in the muscle channel object.
	*/
	double			getTendonExcursion();

	// Functions for controller and output write ops

	/**
	 * @brief Starts the force controller and update the status of the muscle channel.
	 * This function only needs to be called for one of the muscle channels.
	*/
	void			startMuscleControl();

	/**
	 * @brief Returns the current motor command that is being commanded to the motor.
	 * @return Current motor command by the motor controller (in volts)
	*/
	double			getMotorCommand();

	/**
	 * @brief Sets the current motor command to be sent to the DC motor.
	 * @param newCommand The new motor command (in volts) to be send to the motor.
	*/
	void			setMotorCommand(double newCommand);

	/**
	 * @brief [FOR FUTURE USE ONLY] Function pointer to any control law you might want the muscle channel to use.
	*/
	void			(*controlRoutine)(unsigned numOutputs, double* outputs, unsigned numInputs, double* inputs);

	/**
	 * @brief Executes the control law set by the \ref controlRoutine function pointer.
	*/
	void			executeControl();

	/**
	 * @brief Stops the force controller and update the status of the muscle channel.
	 * This function only needs to be called for one of the muscle channels.
	*/
	void			stopMuscleControl();
};

class T5encoder {
	T5encoder();
};


class T5control {
	T5control();
};

#endif // !MYOSYN_H

