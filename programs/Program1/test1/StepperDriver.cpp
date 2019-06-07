/*
 * Generic Stepper Motor Driver Driver
 * Indexer mode only.
 * Copyright (C)2019 Jonathan Ramiro Vargas Suasnavas
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 *
 */

#include "StepperDriver.h"

/*
 * Constructors
 */

StepperDriver::StepperDriver(){
	// Pins Setup
	pinMode(MS1,OUTPUT);
	pinMode(MS2,OUTPUT);
	pinMode(MS3,OUTPUT);
	pinMode(STEP,OUTPUT);
	pinMode(DIR,OUTPUT);
}
StepperDriver::StepperDriver(short dir_pin, short step_pin, short ms1_pin, short ms2_pin, short ms3_pin){
	// Pins Setup (Make a global copy)
	StepperDriver::MS1 = ms1_pin;
	StepperDriver::MS2 = ms2_pin;
	StepperDriver::MS3 = ms3_pin;
	StepperDriver::STEP = step_pin;
	StepperDriver::DIR = dir_pin;
	pinMode(StepperDriver::MS1,OUTPUT);
	pinMode(StepperDriver::MS2 ,OUTPUT);
	pinMode(StepperDriver::MS3,OUTPUT);
	pinMode(StepperDriver::STEP,OUTPUT);
	pinMode(StepperDriver::DIR ,OUTPUT);
	
}

/***************************************************************************
 *
 * Direction Function
 * It allows to choose the direction of the step motor
 *
 ***************************************************************************/
void StepperDriver::DirectionChooser(StepperDriver::Direction direction = StepperDriver::Direction::COUNTERCLOCKWISE){
  switch (direction)
  {
  case CLOCKWISE:
    digitalWrite(DIR,HIGH);
    break;
  case COUNTERCLOCKWISE:
    digitalWrite(DIR,LOW);  
    break;
  default:
    digitalWrite(DIR,LOW);
    break;
  }
}

/***************************************************************************
 *
 * StepChooser Function
 * It allows to choose the microstep resolution os step motor
 *
 ***************************************************************************/

void StepperDriver::StepChooser(StepperDriver::Steps step = StepperDriver::Steps::FULL){
  switch (step)
  {
  case FULL:
    digitalWrite(MS1,LOW);
    digitalWrite(MS2,LOW);
    digitalWrite(MS3,LOW);
    break;
  case HALF:
    digitalWrite(MS1,HIGH);  
    digitalWrite(MS2,LOW);
    digitalWrite(MS3,LOW);
    break;
  case QUARTER:
    digitalWrite(MS1,LOW);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,LOW);
    break;
  case EIGHTH:
    digitalWrite(MS1,HIGH);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,LOW);
    break;
  case SIXTEENTH:
    digitalWrite(MS1,HIGH);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,HIGH);
    break;
  
  default:
    digitalWrite(MS1,LOW);
    digitalWrite(MS2,LOW);
    digitalWrite(MS3,LOW);
    break;
  }
}

