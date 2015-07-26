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

int stopPin=28;
int startPin=26;
int pausePin=24;
int unpausePin=22;
int primePin = 30; //need to add this button in the circuit

int s0 = 6;
int s1 = 7;

int solenoidPins[]= {1, 2};
int IRPins[]= {32, 33};
int washPins[]= {32, 33};
int elutionPins[]= {32, 33};
int ignore= -1;

int mode = 1; //Normal mode = 0, Priming mode = 1 (Default should be prime mode)
int check=1;
boolean stopState=false;
boolean pauseState=false;
boolean startState=false;
boolean sterilizationCheck=false;

boolean printCheck = false;
boolean normalCheck = false;

int solutionPins[3] = {0, 1, 2}; //IR, Wash, Elution
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
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
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
  
  if(mode == 0)
  {
    if(!normalCheck){
      tempWrite(lcd, "Entering EXTRACTION");
    }
    ExtractionMode();
  }
  else if (mode == 1)
  {
    tempWrite(lcd, "Entering Prime Mode");
    runPrimingMode();
  }

  else if(mode == 2){
    tempWrite(lcd, "Preparing to Prime");
    enterRunningMode();
  }
}












