#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "StepperDriver.h"



enum Pins {
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
StepperDriver driver1(Pins::DIR, Pins::STEP, Pins::MS1, Pins::MS2, Pins::MS3);

int count = 0;

void ICACHE_RAM_ATTR onTimerISR(){
    digitalWrite(LED2,!(digitalRead(LED2)));  //Toggle LED Pin
    timer1_write(600000);//12us
}

void setup() {
	pinMode(Pins::LED1,OUTPUT);
	pinMode(Pins::LED2,OUTPUT);
  driver1.DirectionChooser(StepperDriver::Direction::CLOCKWISE);
  driver1.StepChooser(StepperDriver::Steps::SIXTEENTH);

  // Serial communication setup
  Serial.begin(Constants::BAUD_RATE);

  // Wait until voltage levels are correct
  delay(500);

    // Call Time Elapsed function after that time have elapsed
  //timer.attach(0.005f, TimeElapsed);
  timer.attach_ms(1000, TimeElapsed); //ciclo is the double  
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(600000); //120000 us
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

