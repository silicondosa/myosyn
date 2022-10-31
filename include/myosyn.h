#pragma once
#ifndef MYOSYN_H
#define MYOSYN_H

#include <iostream>
#include <quickDAQ.h>

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

unsigned muscle_ld_cell [8][2]		=  {{7, 0},  // Muscle Channel 0
										{7, 8},  // Muscle Channel 1
										{7, 1},  // Muscle Channel 2
										{7, 9},  // Muscle Channel 3
										{7, 2},  // Muscle Channel 4
										{7,10},  // Muscle Channel 5
										{7,11},  // Muscle Channel 6
										{7, 3}}; // Muscle Channel 7

int T5_quickDAQstatus = 0;

typedef enum _DAQarrangement
{
	RING_OF_FIRE	= 0,
	QUADRUPED		= 1,
	MUSCLE_MODULE	= 2
}DAQarrangement;

class myosyn {
	unsigned channelID;
	DAQarrangement myDAQarrangement;

	unsigned (*motor_enable_config)[2];
	unsigned (*motor_value_config)[2];
	unsigned (*encoder_config)[2];
	unsigned (*encoder_opt_config)[2];
	unsigned (*loadcell_config)[2];

	double muscleToneValue;
	double max_muscleTension;

	double loadCell_offset;

	double current_muscleTension;

public:
	myosyn(unsigned muscleChannel, const DAQarrangement myDAQarrangement);
	
	void	enable();
	void	setMuscleTone(double myMuscleTone_value);
	double	getMuscleTone();
	void	setMaxMuscleTension(double max_tension);
	double	getMaxMuscleTension();
	void	disable();

	void	setMuscleTension(double myTension);
	double	getMuscleTension();

	void disable();
};

class T5encoder {
	T5encoder();
};


class T5control {
	T5control();
};

#endif // !MYOSYN_H

