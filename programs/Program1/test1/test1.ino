#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "StepperDriver.h"



enum Pin {
  LED1 = 2,     //D4
  LED2 = 16,    //D0
	MS1 = 5,      //D1
 	MS2 = 4,      //D2
	MS3 = 0,      //D3
	STEP = 14,     //D5
	DIR = 12,     //D6
};

enum Constants {
  BAUD_RATE = 115200,
};

Ticker timer;
StepperDriver driver1(Pin::DIR, Pin::STEP, Pin::MS1, Pin::MS2, Pin::MS3);

int count = 0;

void setup() {
	pinMode(Pin::LED1,OUTPUT);
	pinMode(Pin::LED2,OUTPUT);
  driver1.DirectionChooser(StepperDriver::Direction::CLOCKWISE);
  driver1.StepChooser(StepperDriver::Steps::SIXTEENTH);

  // Serial communication setup
  Serial.begin(Constants::BAUD_RATE);

  // Wait until voltage levels are correct
  delay(500);

    // Call Time Elapsed function after that time have elapsed
  //timer.attach(0.005f, TimeElapsed);
  timer.attach_ms(1000, TimeElapsed); //ciclo is the double  
  driver1.StartTimer();
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

