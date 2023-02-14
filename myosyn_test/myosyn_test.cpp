// myosyn_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <myosyn.h>
#include <math.h>
#include <macrodef.h>
#include <quickDAQ.h>

using namespace std;

const unsigned nMuscles = 2;
unsigned muscleID[nMuscles] = { 3,2 };


int main()
{
	std::cout << "Hello World!\n";
	unsigned i, j, updateCount;
	double t;
	double mySamplingRate = 1000.0; // Hz
	double samplePeriod = 1 / mySamplingRate;
	unsigned mySampleCount = 30000;
	double mc[nMuscles];
	myosyn *m = new myosyn[nMuscles];
	
	// Stuff for measuring joint angle sensor
	double jtAngle;

	//quickDAQinit();
	//pinMode(5, ANALOG_IN, 31);

	// Initialize myosyn library
	myosynSetConfiguration(RING_OF_FIRE);
	myosynSamplingRate(mySamplingRate);

	for (i = 0; i < nMuscles; i++) {
		new(&(m[i])) myosyn(muscleID[i]); // Based on https://stackoverflow.com/a/35089001/4028978
	}
	pinMode(5, ANALOG_IN, 31);

	cout << "Muscles initialized. Press any key to calibrate loadcells and wind up motor for each muscle...\n";
	getchar();
	for (i = 0; i < nMuscles; i++) {
		m[i].calibrateTension();
		m[i].windUp();
		cout << "\t- Muscle " << i << " wound up and LC calibrated...\n";
	}
	myosynWaitForClock();

	cout << "Muscle windup complete! Press any key to calibrate encoders for each muscle...\n";
	getchar();
	myosynWaitForClock();
	for (i = 0; i < nMuscles; i++) {
		m[i].calibrateExcursion();
		cout << "\t- Muscle " << i << " calibrated...\n";
	}

	cout << "Calibration complete! Let's read data for " << mySampleCount/mySamplingRate << "seconds...\n";
	getchar();
	myosynWaitForClock();
	cout << endl;
	for (i = 0, t = 0.0, updateCount = 0; i < mySampleCount; i++, t += samplePeriod) {
		
		// Changing motor command every second
		for (j = 0; j < myosynNumMuscles(); j++) {
			if (i == 0) {
				m[j].startMuscleControl();
			}
			// write motor command
			//mc[j] = m[j].getMuscleToneTension() + (updateCount % 2 == 0) ? ((j % 2 == 0) ? (0.1 * (updateCount)) : 0) : ((j % 2 == 0) ? 0 : (0.1 * (updateCount)));
			mc[j] = max(((j%2==0)?4:4) * cos(2 * 3.1416 * 1 * t + ((j%2==0)?0:3.1416)), 0);
			//cortexDrive[1] = max((cortexVoluntaryAmp - 0) * sin(2 * 3.1416 * cortexVoluntaryFreq * tick + 3.1416), 0);
			m[j].setMotorCommand(mc[j]);
		}

		m[0].executeControl();
		myosynWaitForClock();
		m[0].readMuscleTension();
		m[0].readTendonExcursion();
		
		jtAngle = (double) getAnalogInPin(5, 31);
		printf("%0.3lf: q1 %+06.2lf ", t, jtAngle);

		
		for (j = 0; j < myosynNumMuscles(); j++) {
			// SCR: DO PRINTING HERE
			printf("| M(%d): MC %+06.2lfV, MT %+06.2lfN, TE %+06.2lfmm ", \
				(int)m[j].getChannelID(), \
					 m[j].getMotorCommand(), \
					 m[j].getMuscleTension(), \
					 m[j].getTendonExcursion());
		}
		printf("\r");
	}
	
	cout << "\nSampling complete! Press any key to wind down motors.\n";
	getchar();
	myosynWaitForClock();
	for (i = 0; i < nMuscles; i++) {
		m[i].windDown();
	}
	
	delete[] m;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
