/*
 * Filename: enterRunningMode
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/*
  Function: enterRunningMode
  Function: boolean enterRunningMode(int solutionIndex)
  Description: Begins the running mode for the selected solution index. This index will be used to call the
               motor pins for the solution.
  Parameters: Index to begin flow of solution.
  Error Conditions: Index Out of Bounds
  Return Value: True if priming is completed properly. Otherwise, returns false.
*/
void enterRunningMode()
{
  writeLine(lcd, "Buffer: " + solutionNames[solutionIndex], 1);
  writeLine(lcd, "Hold START to prime",2);
  while(!digitalRead(stopPin)){
    while(digitalRead(startPin))
    {
      runMotor(solutionPins[solutionIndex]);
    }
    motorStop();
  }
  mode = 1;
  tempWrite(lcd, "Priming completed");
}
