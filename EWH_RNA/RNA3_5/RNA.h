/*
 * Filename: RNA.h
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 
 *
 * Data Fields:
 *            int check=1;
 *            boolean stopState=false;
 *            boolean pauseState=false;
 *            boolean startState=false;
 *            boolean sterilizationCheck=false;
 *            boolean sterilizationCheck=false;
 *            boolean IRPrimed = false;
 *            boolean washPrimed = false;
 *            boolean elutionPrimed = false;
 *
 *            int stopPin=50;
 *            int startPin=26;
 *            int pausePin=24;
 *            int unpausePin=22;
 *            int primePin = 20;
 *
 *            int airPins[] = {2, 3};
 *            int solenoidPins[]= {2, 3};
 *            int IRPins[] = {2, 3};
 *            int washPins[] = {2, 3};
 *            int elutionPins[] = {2, 3};
 *            int sterilizationPins[] = {2, 3};
 *            int ignore[] = {-1, -1};
 *
 *            boolean printCheck = false;
 *
 * Methods:
 *            void writeLine(LiquidCrystal lcd, String string, int line);
 *            void countDown(LiquidCrystal lcd, unsigned long timeLeft);
 *            void tempWrite (LiquidCrystal lcd, String string);
 *            unsigned long pauseCheck (int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]);
 *            void motorStop(int motorPins[]);
 *            void runForward(int motorPins[]);
 *            void runBackward(int motorPins[]);
 *            double voltsToFlow(int rawADC)
 *
 * Sources of Help:
 */
#ifndef RNA_H
#define RNA_H
 
 
#include <LiquidCrystal.h>
#include <Arduino.h>

//LCD setup
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int mode = 0; //Normal mode = 0, Priming mode = 1
int check=1;
boolean stopState;
boolean pauseState;
boolean startState;
boolean sterilizationCheck;
boolean IRPrimed;
boolean washPrimed;
boolean elutionPrimed;

int stopPin;
int startPin;
int pausePin;
int unpausePin;
int primePin; //need to add this button in the circuit

unsigned long primeTime;

int solenoidPins[];
int IRPins[];
int washPins[];
int elutionPins[];
int sterilizationPins[;
int ignore[];

int solutionPins[];

boolean printCheck;

void writeLine(LiquidCrystal lcd, String string, int line);
void countDown(LiquidCrystal lcd, unsigned long timeLeft);
void tempWrite (LiquidCrystal lcd, String string);

unsigned long pauseCheck (int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]);

inline boolean postBuffer(unsigned long rxnTime, int solenoidPins[], unsigned long solenoidTime, int stopPin, boolean stopState, LiquidCrystal lcd);
inline boolean runBuffer(int motorPins[], unsigned long bufferTime, int stopPin, int pausePin, int unpausePin, boolean pauseState, LiquidCrystal lcd);

void motorStop(int motorPins[]);
void runForward(int motorPins[]);
void runBackward(int motorPins[]);

void changeMode();
boolean enterRunningMode(int,String);
void runNormalMode();
void runPrimingMode();



/*
  void control(PID pid, int bufferOutputPin, Adafruit_DCMotor *bufferMotor){
 double flowRate = voltsToFlow(analogRead(bufferOutputPin));
 pid.SetInput(flowRate);
 pid.Compute();
 bufferMotor->setSpeed(pid.GetOutput());
 bufferMotor->run(FORWARD);
 }
 */
/*
double voltsToFlow(int rawADC) //Analog to Digital Conversion
{
  //do something w/ the input from an Output Pin...hmm, maybe I should rename those...
  int magic = 1;
  double flowRate = rawADC * magic;  //magic is performed, i.e. blackbox that needs to be filled in
  return flowRate;
}
*/

/* 
 * Function name: motorStop()
 * Function prototype:void motorStop( int motorPins[]){
 * Description: Stop the motors              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void motorStop(int motorPins[]){
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], LOW);
}

/* 
 * Function name: runForward()
 * Function prototype:void runBackWard( int motorPins[]){
 * Description: Run the motor forwards              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void runForward(int motorPins[]){
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], HIGH);
}

/* 
 * Function name: runBackward()
 * Function prototype:void runBackWard( int motorPins[]){
 * Description: Run the motor backwards              
 * Parameters: array containing the pins assigned to the motors
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void runBackward(int motorPins[]){
  digitalWrite(motorPins[0], HIGH);
  digitalWrite(motorPins[1], LOW);
}

#endif
