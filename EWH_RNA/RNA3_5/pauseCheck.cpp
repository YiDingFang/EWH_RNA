/*
 * Filename: pauseCheck
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/* 
 * Function pauseCheck
 * Function iunsigned long pauseCheck(boolean pauseState, 
 *                                    int pausePin, int unpausePin, 
 *                                    LiquidCrystal lcd, int motorPins[])
 * Description: check if the pauseButton has been pressed between now and the end       
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
unsigned long pauseCheck(int pausePin, int unpausePin, LiquidCrystal lcd, int motor){
  do{
     //pauseState=digitalRead(pausePin);
    // check the state of the pausePin
    if(pauseState){
      motorStop();
      if(!printCheck){
        Serial.print("PAUSED");
        (lcd).home();
        (lcd).print("                    ");
        (lcd).home();
        (lcd).print("PAUSED");
        printCheck = true;
      } 
      pauseState=!digitalRead(unpausePin);
      if(!pauseState){
        Serial.print("UNPAUSED");
        tempWrite(lcd, "UNPAUSED");
        printCheck = false;
        runMotor(motor);
      }
    }
    else{
      pauseState=digitalRead(pausePin);
    }
  }while (pauseState);
  return millis();
 
}
