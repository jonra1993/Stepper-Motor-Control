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
speedRampData srd;

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

/*! \brief Move the stepper motor a given number of steps.
 *
 *  Makes the stepper motor move the given number of steps.
 *  It accelrate with given accelration up to maximum speed and decelerate
 *  with given deceleration so it stops at the given step.
 *  If accel/decel is to small and steps to move is to few, speed might not
 *  reach the max speed limit before deceleration starts.
 *
 *  \param step  Number of steps to move (pos - CW, neg - CCW).
 *  \param accel  Accelration to use, in 0.01*rad/sec^2.
 *  \param decel  Decelration to use, in 0.01*rad/sec^2.
 *  \param speed  Max speed, in 0.01*rad/sec.
 */
void StepperDriver::SpeedCntrMove(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
  //! Number of steps before we hit max speed.
  unsigned int max_s_lim;
  //! Number of steps before we must start deceleration (if accel does not hit max speed).
  unsigned int accel_lim;

  // Set direction from sign on step value.
  if(step < 0){
    srd.dir = CCW;
    step = -step;
  }
  else{
    srd.dir = CW;
  }

  // If moving only 1 step.
  if(step == 1){
    // Move one step...
    srd.accel_count = -1;
    // ...in DECEL state.
    srd.run_state = DECEL;
    // Just a short delay so main() can act on 'running'.
    srd.step_delay = 1000;
    status.running = TRUE;
    OCR1A = 10;
    // Run Timer/Counter 1 with prescaler = 8.
    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
  }
  // Only move if number of steps to move is not zero.
  else if(step != 0){
    // Refer to documentation for detailed information about these calculations.

    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srd.min_delay = A_T_x100 / speed;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srd.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel))/100;

    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = ((long)step*decel) / (accel+decel);
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srd.decel_val = accel_lim - step;
    }
    else{
      srd.decel_val = -((long)max_s_lim*accel)/decel;
    }
    // We must decelrate at least 1 step to stop.
    if(srd.decel_val == 0){
      srd.decel_val = -1;
    }

    // Find step to start decleration.
    srd.decel_start = step + srd.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srd.step_delay <= srd.min_delay){
      srd.step_delay = srd.min_delay;
      srd.run_state = RUN;
    }
    else{
      srd.run_state = ACCEL;
    }

    // Reset counter.
    srd.accel_count = 0;
    status.running = TRUE;
    OCR1A = 10;
    // Set Timer/Counter to divide clock by 8
    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
  }
}