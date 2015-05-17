/*
 * Filename: runBuffer
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/* 
 * Function runBuffer
 * Function inline boolean runBuffer(int motorPins[], unsigned long bufferTime,
 *                                   int stopPin, int pausePin, int unpausePin, 
 *                                   boolean pauseState, LiquidCrystal lcd)
 * Description: execute the runBuffer sequence           
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
 
boolean runBuffer(int motorPins[], unsigned long bufferTime, int stopPin, int pausePin, int unpausePin, boolean pauseState, LiquidCrystal lcd){
  // set the flowrate
  double flowRate = 1;
  // set up the LCD display based on runBuffer
  // set cursor and write TIME to screen
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  // set cursor and write VOLUME to screen
  (lcd).setCursor(8,3);
  (lcd).print("VOLUME: ");
  // set cursor and write : to screen
  (lcd).setCursor(17,2);
  (lcd).print(":");
  
  // set the end time
  unsigned long duration = bufferTime + millis();
  
  // while the interval is not over, run the motorPins
  while(millis() < duration){
    runForward(motorPins);
    // if stop is pressed at any time, return true
    if(digitalRead(stopPin)){
      motorStop(motorPins);
      return true;
    }
    // record the amount of time remaining
    unsigned long timeDif = duration - millis();
    // reset the end time withthe remaining time to accomodate for pause duration
    duration = timeDif + pauseCheck(pausePin, unpausePin, lcd, motorPins);
    
    // lcd prints volume
    unsigned long volume = (unsigned long)flowRate*(millis()%60000)/1000;  //need to revise length of volume digits once know more definite flowrate and final volumes***************************************
    // move cursor to bot
    (lcd).setCursor(17,3);
    (lcd).print(volume);
    //lcd prints time remaining every second - in theory; I'm not sure if the time it takes to process the prior code will mess with how the code keeps track of time; will need to test empirically
    countDown(lcd, duration - millis());
  }
  // stop the motorPins
  motorStop(motorPins);
  return false;
}
