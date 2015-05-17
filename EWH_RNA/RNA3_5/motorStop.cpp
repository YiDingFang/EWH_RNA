/*
 * Filename: motorStop
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */


#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"


/* 
 * Function name: motorStop()
 * Function prototype:void motorStop( int motorPins[]){
 * Description: Stop the motors              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void motorStop(int motorPins[]){
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], LOW);
}
