/*
 * Filename: runForward
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/* 
 * Function name: runForward()
 * Function prototype:void runBackWard( int motorPins[]){
 * Description: Run the motor forwards              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void runForward(int motorPins[]){
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], HIGH);
}
