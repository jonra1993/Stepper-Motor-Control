#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>

Ticker timer;

#define LED1 2     //D4
#define LED2 16    //D0
#define MS1 5      //D1
#define MS2 4      //D2
#define MS3 0      //D3
#define STEP 14    //D5
#define DIR 12     //D6
#define BAUD_RATE 115200

/***************************************************************************
 *
 * Constants
 * 
 ***************************************************************************/
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
int count = 0;

void setup() {
  // Pins Setup
  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  pinMode(MS3,OUTPUT);
  pinMode(STEP,OUTPUT);
  pinMode(DIR,OUTPUT);
  pinMode(LED1,OUTPUT);


  // Serial communication setup
  Serial.begin(BAUD_RATE);


  // Wait until voltage levels are correct
  delay(500);

    // Call Time Elapsed function after that time have elapsed
  //timer.attach(0.005f, TimeElapsed);
  timer.attach_ms(10, TimeElapsed); //ciclo is the double  
}

void loop() {


}



/***************************************************************************
 *
 * TimeElapsed Function
 *
 ***************************************************************************/
void TimeElapsed() {
  count++;
  digitalWrite(LED1,!digitalRead(LED1));
}

/***************************************************************************
 *
 * Direction Function
 * It allows to choose the direction of the step motor
 *
 ***************************************************************************/
void DirectionChooser(Direction direction){
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

void StepChooser(Steps step){
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
