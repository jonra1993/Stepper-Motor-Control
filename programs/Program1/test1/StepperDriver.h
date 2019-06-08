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

		/*! \brief Holding data used by timer interrupt for speed ramp calculation.
		*
		*  Contains data used by timer interrupt to calculate speed profile.
		*  Data is written to it by move(), when stepper motor is moving (timer
		*  interrupt running) data is read/updated when calculating a new step_delay
		*/
		typedef struct {
		//! What part of the speed ramp we are in.
		unsigned char run_state : 3;
		//! Direction stepper motor should move.
		unsigned char dir : 1;
		//! Peroid of next timer delay. At start this value set the accelration rate.
		unsigned int step_delay;
		//! What step_pos to start decelaration
		unsigned int decel_start;
		//! Sets deceleration rate.
		signed int decel_val;
		//! Minimum time delay (max speed)
		signed int min_delay;
		//! Counter used when accelerateing/decelerateing to calculate step_delay.
		signed int accel_count;
		} speedRampData;

		StepperDriver();
		StepperDriver(short dir_pin, short step_pin, short ms1_pin, short ms2_pin, short ms3_pin);
		void DirectionChooser(Direction direction);
		void StepChooser(Steps step);
		void SpeedCntrMove(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)

};


#endif // STEPPERDRIVER_H