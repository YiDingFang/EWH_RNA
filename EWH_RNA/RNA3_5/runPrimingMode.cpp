/*
 * Filename: runPrimingMode
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

/*
 * Function runPrimingMode
 * Function void runPrimingMode()
 * Description: Runs the priming mode when called in loop(). The user can change which solution
                to prime by pressing the pause (up) and unpause buttons (down). 
                Pressing the start button starts priming sequence. \
                Stop button exits priming mode.
 * Parameters: None
 * Error Conditions: None
 * Return Value: None
 */
void runPrimingMode()
{
  String currentSolution;
  
  int currentIndex = 0; //defaults to first index of solution pin array
  //Display UI
  int n = 3; // number of indices 
  
  switch(currentIndex)
  {
    case 0: currentSolution = "IR";break;
    case 1: currentSolution = "Elution";break;
    case 2: currentSolution = "Wash";break;
    default: return;
  }
  
  writeLine(lcd,"Select Solution to Prime: " + currentSolution,1);
  writeLine(lcd,"Press and unpause to change solution", 2);
 
  if(digitalRead(pausePin))
  {
    currentIndex++;
    currentIndex = (currentIndex%n + n) % n; // Keeps the index from reaching out of bounds
  }
  else if (digitalRead(unpausePin))
  {
    currentIndex--;
    currentIndex = (currentIndex%n + n) % n;
  }
  
  else if (digitalRead(startPin))
  {
    //if(enterRunningMode(currentIndex))
      //writeLine(lcd, "Selected priming completed", 1);
    //if
    //{
      writeLine(lcd, "Error occurred during priming", 1);
    //} 
  }
 
  else if (digitalRead(stopPin))
    changeMode();
  return;
}
