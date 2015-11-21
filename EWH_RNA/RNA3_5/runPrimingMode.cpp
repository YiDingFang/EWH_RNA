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
  int n = 4; // number of indices
  writeLine(lcd,"Select Soln: ",1); 
  writeLine(lcd,"P/UP to change soln", 2);
  lcd.setCursor(13,1);
  lcd.print(solutionNames[solutionIndex]); 
  while(!digitalRead(startPin)){
    
    if(digitalRead(primePin)){
      modeChanged=true;
      break;
    }   
 
    if(digitalRead(pausePin))
    {
      solutionIndex++;
      solutionIndex = (solutionIndex%n + n) % n; // Keeps the index from reaching out of bounds
      lcd.setCursor(13,1);
      lcd.print("       ");
      lcd.setCursor(13,1);
      lcd.print(solutionNames[solutionIndex]);
    }
    else if (digitalRead(unpausePin))
    {
      solutionIndex--;
      solutionIndex = (solutionIndex%n + n) % n;
      lcd.setCursor(13,1);
      lcd.print("       ");
      lcd.setCursor(13,1);
      lcd.print(solutionNames[solutionIndex]);
    }
  }

  if(modeChanged){
    changeMode();
  }
  else{
    mode = 2;
  }
 lcd.clear();   
}
