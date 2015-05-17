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
  
  if(digitalRead(primePin)){
    changeMode();
  }
  
  if(mode == 0)
  {
    tempWrite(lcd, "Entering Normal Mode");
    runNormalMode();
  }
  else if (mode == 1)
  {
    tempWrite(lcd, "Entering Priming Mode");
    runPrimingMode();
  }
  
}












