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
  while(!digitalRead(startPin)){
    switch(currentIndex)
    {
      case 0: currentSolution = "IR";break;
      case 1: currentSolution = "Elution";break;
      case 2: currentSolution = "Wash";break;
      default: return;
    }
    
    writeLine(lcd,"Select Soln: " + currentSolution,1);
    writeLine(lcd,"P/UP to change soln", 2);
   
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
  }

    enterRunningMode(currentIndex, currentSolution);

 /*
  if(digitalRead(modePin)){
    changeMode();
    }
    */
}
