/*
 * Generic Stepper Motor Driver Driver
 * Indexer mode only.
 * Copyright (C)2019 Jonathan Ramiro Vargas Suasnavas
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 *
 */

#ifndef STEPPERDRIVER_H
#define STEPPERDRIVER_H
#include <Arduino.h>

///////////////////    STEPPER MOTOR TYPE SETUP
#define STEPPER_DEGREES			1.8

///////////////////    MICROSTEPPING TIME THRESHOLD
#define FULL_STEP_THRESHOLD			200 	// in microseconds
#define HALF_STEP_THRESHOLD			500  	// (1/2) microseconds
#define QUARTER_STEP_THRESHOLD		800  	// (1/4) microseconds
#define EIGHTH_STEP_THRESHOLD		10000  	// (1/8) microseconds
#define SIXTEENTH_STEP_THRESHOLD	1000000	// (1/16) microseconds


class StepperDriver {
protected:


private:
	///////////////////    Driver pins
	short MS1 = 5;      //D1
 	short MS2 = 4;      //D2
	short MS3 = 0;      //D3
	short STEP = 14;     //D5
	short DIR = 12;     //D6

public:
		enum Steps {
		FULL = 0,
		HALF = 1,
		QUARTER = 2,
		EIGHTH = 3,
		SIXTEENTH = 4
		};
		
		enum Direction {
		CLOCKWISE = 0,
		COUNTERCLOCKWISE= 1
		};

		StepperDriver();
		StepperDriver(short dir_pin, short step_pin, short ms1_pin, short ms2_pin, short ms3_pin);
		void DirectionChooser(Direction direction);
		void StepChooser(Steps step);
};


#endif // STEPPERDRIVER_H