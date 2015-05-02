/*
 * Filename: RNA3_5.h
 * Author: UCSD_EWH RNA
 * Description:
 * Date: 
 * Sources of Help: ...gotta add links here later
 *
 * Extra Comments:
 * My lady's fat, she's ugly, she's fat and she's ugly, but I love her, I need her, I really really love her
 */

#include <LiquidCrystal.h>
#include "RNA3_5.h"

/*
 * Function name: setup
 * Function prototype: void setup{}
 * Description: Instantiate all global variables and setup necessary pins
 *
 * Parameters: None
 * Side Effects:
 * Error Conditions:
 * Return Value: 0
 *
 */

void setup() {
  lcd.begin(20, 4);
  pinMode(startPin, INPUT);
  pinMode(pausePin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(unpausePin, INPUT);
  pinMode(solenoidPins[0], OUTPUT);
  pinMode(solenoidPins[1], OUTPUT);
  pinMode(IRPins[0], OUTPUT);
  pinMode(IRPins[1], OUTPUT);
  pinMode(washPins[0], OUTPUT);
  pinMode(washPins[1], OUTPUT);
  pinMode(elutionPins[0], OUTPUT);
  pinMode(elutionPins[1], OUTPUT);
  pinMode(sterilizationPins[0], OUTPUT);
  pinMode(sterilizationPins[1], OUTPUT);
  Serial.begin(9600);
  Serial.print("set up");
}


/*
 * Function name: loop
 * Function prototype: void loop()
 * Description: Executes RNA extraction sequence
 *              proccess can be paused and unpaused at any time
 *              stopping the proccess will cause the sequence to restart
 * Parameters: None
 * Side Effects:
 * Error Conditions:
 * Return Value: 0
 *
 */

void loop(){
  // clear the LCD screen
  lcd.clear();
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
    stopState = runBuffer(elutionPins, 2000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
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
    // run sterilizatin round for 2000ms
    stopState = runBuffer(sterilizationPins, 2000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
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

} // void loop end













