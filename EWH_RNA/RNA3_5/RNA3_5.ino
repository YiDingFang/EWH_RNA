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
#include "RNA.h"

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

int stopPin=50;
int startPin=26;
int pausePin=24;
int unpausePin=22;
int primePin = 22; //need to add this button in the circuit

int solenoidPins[]= {1, 2};
int IRPins[] = {33, 32};
int washPins[] = {43, 42};
int elutionPins[] = {35, 34};
int sterilizationPins[] = {43, 42}; 
int ignore[] = {-1, -1};

int mode = 1; //Normal mode = 0, Priming mode = 1 (Default should be prime mode)
int check=1;
boolean stopState=false;
boolean pauseState=false;
boolean startState=false;
boolean sterilizationCheck=false;

boolean printCheck = false;


int* solutionPins[3] = {IRPins, washPins, elutionPins};
String solutionNames[3] = {"IR", "WASH", "ELUTION"};
int solutionIndex = 0;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

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
/*  
  if(digitalRead(primePin)){
    changeMode();
  }
*/  
  if(mode == 0)
  {
    tempWrite(lcd, "Entering Normal Mode"); //should change this to make sure the tempWrite statement is not repeated; possible make normalMode a self-contained while-loop
    runNormalMode();
  }
  else if (mode == 1)
  {
    tempWrite(lcd, "Entering Priming Mode");
    runPrimingMode();
  }

  else if(mode == 2){
    tempWrite(lcd, "Preparing to Prime");
    enterRunningMode();
  }
}












