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
 * Description: Stop the solenoid            
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void motorStop(int motorPins[]){
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], LOW);
}

/*
 *Function name: motorStop()
 *Function prototype: vpod motorStop(){
 *Description: stop the motors
 *Parameters: none
 *Side effects: none
 *Error conditions: none
 *Return Value: void
 */

void motorStop(){
   digitalWrite(s0, LOW);
   digitalWrite(s1, LOW);
}
