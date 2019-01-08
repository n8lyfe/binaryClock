/********************************************************
      binary_clock_v3
    *                                                      *
      Author:  n8lyfe
               
    *                                                      *
      Purpose:  Running a Binary Clock on an atmega328p with an 3231
    *                                                      *
********************************************************/

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

int dow = 0;
int mo = 1;
int d = 1;
int h = 1;

boolean summerTime = true;

const int clockPin = 0;//Pin connected to SH_CP of 74HC595
const int latchPin = 1;//Pin connected to ST_CP of 74HC595
const int dataPin = 2; //Pin connected to DS of 74HC595


//delay() alternative to use buttons
unsigned long previousMillis_sec = 0; // saves how many milliseconds pasts between the last change
unsigned long seconds = 1000;    // Interval. 1000 equals one second

void setup ()
{
  rtc.begin();
  //Serial.begin(9600);
  //set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

}

void loop() {

  DateTime now = rtc.now();
  DateTime cest (now + TimeSpan(0, 0, 0, 12));

  DateTime cet (cest - TimeSpan(0, 1, 0, 0));

  dow = now.dayOfTheWeek();
  mo = now.month();
  d = now.day();



  if (dow == 6 && mo == 10 && d >= 25 && h == 3 && summerTime == true) {
    summerTime = false;
  }

  else if (dow == 6 && mo == 3 && d >= 25 && h == 2 && summerTime == true) {
    summerTime = true;
  }
  if (summerTime == false) {
    //shift out the time to the 74hc595
    digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
    shiftOut(dataPin, clockPin, MSBFIRST, cet.second());
    shiftOut(dataPin, clockPin, MSBFIRST, cet.minute());
    shiftOut(dataPin, clockPin, MSBFIRST, cet.hour());
    digitalWrite(latchPin, HIGH); //pull the latchPin to save the data
    h = cet.hour();
  }
  else {
    //shift out the time to the 74hc595
    digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
    shiftOut(dataPin, clockPin, MSBFIRST, cest.second());
    shiftOut(dataPin, clockPin, MSBFIRST, cest.minute());
    shiftOut(dataPin, clockPin, MSBFIRST, cest.hour());
    digitalWrite(latchPin, HIGH); //pull the latchPin to save the data
    h = cest.hour();
  }
  //delay(50);
}
