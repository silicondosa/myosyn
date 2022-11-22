// myosyn_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <myosyn.h>
unsigned nMuscles = 2;
unsigned muscleID[] = { 3,2 };

using namespace std;
int main()
{
	std::cout << "Hello World!\n";
	unsigned i, j;
	double t;
	double mySamplingRate = 1000.0; // Hz
	double samplePeriod = 1 / mySamplingRate;
	unsigned mySampleCount = 30000;
	myosyn *m = new myosyn[nMuscles];
	
	// Initialize myosyn library
	myosynSetConfiguration(RING_OF_FIRE);
	myosynSamplingRate(mySamplingRate);

	for (i = 0; i < nMuscles; i++) {
		new(&(m[i])) myosyn(muscleID[i]); // Based on https://stackoverflow.com/a/35089001/4028978
	}
	
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
	for (i = 0, t = 0.0; i < mySampleCount; i++, t+=samplePeriod) {
		printf("%0.3lf : ", t);
		for (j = 0; j < myosynNumMuscles(); j++) {
			myosynWaitForClock();
			m[j].readMuscleTension();
			m[j].readTendonExcursion();
			// SCR: DO PRINTING HERE
			printf("M(%d):: LC: %+0.3lf N, ENC: %+0.3lf mm ", \
				(int)m[j].getChannelID(), \
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
