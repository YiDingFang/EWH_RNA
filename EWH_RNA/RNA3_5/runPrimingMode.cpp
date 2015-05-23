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
  boolean modeChanged=false;
  //Display UI
  int n = 3; // number of indices 
  while(!digitalRead(startPin)){
    
    if(digitalRead(primePin)){
      modeChanged=true;
      break;
    }
    writeLine(lcd,"Select Soln: " + solutionNames[solutionIndex],1);
    writeLine(lcd,"P/UP to change soln", 2);
   
    if(digitalRead(pausePin))
    {
      solutionIndex++;
      solutionIndex = (solutionIndex%n + n) % n; // Keeps the index from reaching out of bounds
    }
    else if (digitalRead(unpausePin))
    {
      solutionIndex--;
      solutionIndex = (solutionIndex%n + n) % n;
    }
  }

  if(modeChanged){
    changeMode();
  }
  else{
    mode = 2;
  }
    
}
