/*
 * Filename: runBackward
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/* 
 * Function name: runBackward()
 * Function prototype:void runBackWard( int motorPins[]){
 * Description: Run the motor backwards              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void runBackward(int motorPins[]){
  digitalWrite(motorPins[0], HIGH);
  digitalWrite(motorPins[1], LOW);
}
