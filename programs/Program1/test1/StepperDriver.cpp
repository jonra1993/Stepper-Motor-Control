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

//! Cointains data for timer interrupt.
StepperDriver::speedRampData speedrd;
StepperDriver::GLOBAL_FLAGS status;

void ICACHE_RAM_ATTR onTimerISR(){
  // Holds next delay period.
  unsigned int new_step_delay;
  // Remember the last step delay used when accelrating.
  static int last_accel_delay;
  // Counting steps when moving.
  static unsigned int step_count = 0;
  // Keep track of remainder from new_step-delay calculation to incrase accurancy
  static unsigned int rest = 0;

  OCR1A = speedrd.step_delay;

  switch(speedrd.run_state) {
    case StepperDriver::States::STOP:
      step_count = 0;
      rest = 0;
      // Stop Timer/Counter 1.
      timer1_detachInterrupt();
      status.running = false;
      break;

    case StepperDriver::States::ACCEL:
      sm_driver_StepCounter(speedrd.dir);
      step_count++;
      speedrd.accel_count++;
      new_step_delay = speedrd.step_delay - (((2 * (long)speedrd.step_delay) + rest)/(4 * speedrd.accel_count + 1));
      rest = ((2 * (long)speedrd.step_delay)+rest)%(4 * speedrd.accel_count + 1);
      // Chech if we should start decelration.
      if(step_count >= speedrd.decel_start) {
        speedrd.accel_count = speedrd.decel_val;
        speedrd.run_state = StepperDriver::States::DECEL;
      }
      // Chech if we hitted max speed.
      else if(new_step_delay <= speedrd.min_delay) {
        last_accel_delay = new_step_delay;
        new_step_delay = speedrd.min_delay;
        rest = 0;
        speedrd.run_state = StepperDriver::States::RUN;
      }
      break;

    case StepperDriver::States::RUN:
      sm_driver_StepCounter(speedrd.dir);
      step_count++;
      new_step_delay = speedrd.min_delay;
      // Chech if we should start decelration.
      if(step_count >= speedrd.decel_start) {
        speedrd.accel_count = speedrd.decel_val;
        // Start decelration with same delay as accel ended with.
        new_step_delay = last_accel_delay;
        speedrd.run_state = StepperDriver::States::DECEL;
      }
      break;

    case StepperDriver::States::DECEL:
      sm_driver_StepCounter(speedrd.dir);
      step_count++;
      speedrd.accel_count++;
      new_step_delay = speedrd.step_delay - (((2 * (long)speedrd.step_delay) + rest)/(4 * speedrd.accel_count + 1));
      rest = ((2 * (long)speedrd.step_delay)+rest)%(4 * speedrd.accel_count + 1);
      // Check if we at last step
      if(speedrd.accel_count >= 0){
        speedrd.run_state = StepperDriver::States::STOP;
      }
      break;
  }
  speedrd.step_delay = new_step_delay;
  timer1_write(2);//2uS
}

/*
 * Constructors
 */

StepperDriver::StepperDriver(){
	// Pins Setup
	pinMode(StepperDriver::MS1,OUTPUT);
	pinMode(StepperDriver::MS2,OUTPUT);
	pinMode(StepperDriver::MS3,OUTPUT);
	pinMode(StepperDriver::STEP,OUTPUT);
	pinMode(StepperDriver::DIR,OUTPUT);
}
/*! \brief Init of Timer/Counter1.
 *
 *  Set up Timer/Counter1 to use mode 1 CTC and
 *  enable Output Compare A Match Interrupt.
 */
void StepperDriver::StartTimer(){
  // Tells what part of speed ramp we are in.
  speedrd.run_state = StepperDriver::States::STOP;
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(2); //2uS
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
    StepperDriver::SPR=StepperDriver::FSPR;
    break;
  case HALF:
    digitalWrite(MS1,HIGH);  
    digitalWrite(MS2,LOW);
    digitalWrite(MS3,LOW);
    StepperDriver::SPR=StepperDriver::FSPR*2;
    break;
  case QUARTER:
    digitalWrite(MS1,LOW);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,LOW);
    StepperDriver::SPR=StepperDriver::FSPR*4;
    break;
  case EIGHTH:
    digitalWrite(MS1,HIGH);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,LOW);
    StepperDriver::SPR=StepperDriver::FSPR*8;
    break;
  case SIXTEENTH:
    digitalWrite(MS1,HIGH);
    digitalWrite(MS2,HIGH);
    digitalWrite(MS3,HIGH);
    StepperDriver::SPR=StepperDriver::FSPR*16;
    break;
  default:
    digitalWrite(MS1,LOW);
    digitalWrite(MS2,LOW);
    digitalWrite(MS3,LOW);
    StepperDriver::SPR=StepperDriver::FSPR;
    break;
  }
}
void StepperDriver::SpeedCntrMove(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{

}