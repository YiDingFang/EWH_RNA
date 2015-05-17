/*
 * Filename: postBuffer.cpp
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/* 
 * Function postBuffer
 * Function inline inline boolean postBuffer(int airPins[], unsigned long airTime, unsigned long rxnTime, 
                                             int solenoidPins[], unsigned long solenoidTime, int stopPin, 
                                             boolean stopState, LiquidCrystal lcd){
 * Description:        
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
 
boolean postBuffer(unsigned long rxnTime, int solenoidPins[], unsigned long solenoidTime, int stopPin, boolean stopState, LiquidCrystal lcd){

  //incubate
  writeLine(lcd, "INCUBATE", 2);
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  (lcd).setCursor(17,2);
  (lcd).print(":");
  unsigned long timeRxn=rxnTime + millis();
  while(millis()<timeRxn){
    if(digitalRead(stopPin)){
      return true;
    }
    unsigned long timeDif = timeRxn - millis();
    timeRxn = timeDif + pauseCheck(pausePin, unpausePin, lcd, ignore);
    countDown(lcd, timeRxn - millis());
  }
  //vacuum
  writeLine(lcd, "VACUUM", 2);
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  (lcd).setCursor(17,2);
  (lcd).print(":");
  unsigned long timeSolenoid = solenoidTime + millis();
  while(millis()<timeSolenoid){
    runForward(solenoidPins);
    if(digitalRead(stopPin)){
      motorStop(solenoidPins);
      return true;
    }
    unsigned long timeDif = timeSolenoid - millis();
    timeSolenoid = timeDif + pauseCheck(pausePin, unpausePin, lcd, solenoidPins);
    countDown(lcd, timeSolenoid - millis());
  }
  motorStop(solenoidPins);
  (lcd).setCursor(0,2);
  (lcd).print("                    ");
  (lcd).setCursor(0,3);
  (lcd).print("                    ");
  return false;
}
