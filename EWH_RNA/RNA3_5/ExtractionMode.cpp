/*
 * Filename: ExtractionMode
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"


/*
 * Function runNormalMode
 * Function void runNormalMode()
 * Runs the normal running mode
 * Parameter: None
 * Error Condition: None
 * Return Value: None
*/

void ExtractionMode()
{
  normalCheck = true;
  // clear the LCD screen
  lcd.clear();
  
   if(digitalRead(primePin)){
    changeMode();
  }
  
  // check if there is a button press or not
  startState=digitalRead(startPin);
  // print start and indicate start with check boolean
  if(startState==true){
    Serial.print("START");
    tempWrite(lcd, "START");
    check = 1;
  }
  // if the button has not been pressed exit the loop
  else if(startState==false){ 
    return;
  }

  //PROCEED WITH PROTOCOL; possible have:  startState = false;
  //air-vac-sol  

  //stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);

  
  // check stop state
  if(stopState) {
    // if stop has been pressed report and end loop
    Serial.print("check1");
    tempWrite(lcd, "STOP");
    return;
  }
  
  // status for debug purposes
  Serial.print("check2");

  //IR BUFFER ROUND
  Serial.print("1");
  
  // check the stop pin 
  stopState=digitalRead(stopPin);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  
  // IR BUFFER
  // if start sequence is sucessful
  if(check==1)
  {
    // begin IR round
    writeLine(lcd, "IR ROUND", 1);
    writeLine(lcd, "BUFFER", 2);

    // run the IR buffer for 2000ms
    stopState = runBuffer(IRPins, 2000UL, stopPin, pausePin, unpausePin, pauseState, lcd);

    // check stops are necessary
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    // increment check if successful
    check++;
  }
  // run postbuffer

  stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);

  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  // report end of IR round
  tempWrite(lcd, "IR ROUND COMPLETE");

  // Pause so technician can do the thing  
  writeLine(lcd, "BREAK BEFORE WASH1", 1);
  // pause the sequence
  pauseState = true;
  // call pausecheck and wait for unpause
  pauseCheck(pausePin, unpausePin, lcd, ignore);


  // WASH BUFFER ROUND 1
  // if the previous check was successful 
  if(check==2)
  {
    writeLine(lcd, "WASH ROUND1", 1);
    writeLine(lcd, "BUFFER", 2);

    //run wash buffer for 2000ms
    stopState = runBuffer(washPins, 2000UL, stopPin, pausePin, unpausePin, pauseState, lcd);

    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    // increment the check
    check++;
  }
  // run post buffer

  stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);

  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  // report the end of the first wash round
  tempWrite(lcd, "WASH ROUND1 COMPLETE");  


  // Pause so technician can do the thing  
  writeLine(lcd, "BREAK BEFORE WASH2", 1);
  // pause the sequence
  pauseState = true;
  // call pausecheck and wait for unpause
  pauseCheck(pausePin, unpausePin, lcd, ignore);

  // WASH BUFFER ROUND 2
  // if the previous round was successful 
  if(check==3)
  {
    writeLine(lcd, "WASH ROUND2", 1);
    writeLine(lcd, "BUFFER", 2);

    // run wash buffer fro 2000ms
    stopState = runBuffer(washPins, 2000UL, stopPin, pausePin, unpausePin, pauseState, lcd);

    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    // increment the check
    check++;
  }
  // run the post buffer

  stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);

  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  // report the end of the second wash round
  tempWrite(lcd, "WASH ROUND2 COMPLETE");   


  // Pause so technician can do the thing  
  writeLine(lcd, "REPLACE SPIN COLUMN", 1);
  // pause the sequence
  pauseState = true;
  // call pausecheck and wait for unpause
  pauseCheck(pausePin, unpausePin, lcd, ignore);


  // ELUTION BUFFER ROUND
  // if the previous round was successful
  if(check==4)
  {
    writeLine(lcd, "ELUTION ROUND", 1);
    writeLine(lcd, "BUFFER", 2);
    // run the elution round

    stopState = runBuffer(elutionPins, 500UL, stopPin, pausePin, unpausePin, pauseState, lcd);

    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    // increment the check
    check=5;
  }
  // run the post buffer for the elution round

  stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);

  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  tempWrite(lcd, "ELTN ROUND COMPLETE");


  // pause to exchange waste
  writeLine(lcd, "REMOVE WASTE", 1);
  // pause the sequence once again
  pauseState = true;
  // call pausecheck and wait for unpause
  pauseCheck(pausePin, unpausePin, lcd, ignore);


  // sterilization round (ethanol)
  if(check==5)
  {
    // report sterilization
    writeLine(lcd, "STERILIZING...", 1);
    writeLine(lcd, "SOLUTION", 2);

    // run sterilizatin round for 500ms
    stopState = runBuffer(sterilizationPins, 500UL, stopPin, pausePin, unpausePin, pauseState, lcd);

    // check for the stop state
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    // run the postBuffer for sterilization

    stopState= postBuffer(2000UL, solenoidPins, 2000UL, stopPin, stopState, lcd);  

    lcd.clear();
    tempWrite(lcd, "STERILIZED");
    pauseState = false;
  }   
  // report end of protocol
  tempWrite(lcd, "PROTOCOL COMPLETE");
  // clear lcd
  lcd.clear();

}
