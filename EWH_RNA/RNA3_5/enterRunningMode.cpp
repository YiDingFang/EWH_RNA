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
boolean enterRunningMode(int solutionIndex, String solutionName)
{
  boolean runningComplete = false;
  String currentSolution;
  writeLine(lcd, "Selected Buffer: " + solutionName, 1);
  writeLine(lcd, "Hold start to perform priming",2);
  while(digitalRead(startPin))
  {
    runForward(solutionPins[solutionIndex]);
  }
  motorStop(solutionPins[solutionIndex]);
  runningComplete = true;
  return runningComplete;
}

