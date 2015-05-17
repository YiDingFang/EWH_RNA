/*
 * Filename: RNA3_5.h
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
 *            int* solutionPins[3] = {IRPins, washPins, elutionPins};
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

#include <Arduino.h>

//LCD setup
extern LiquidCrystal lcd;


extern int stopPin;
extern int startPin;
extern int pausePin;
extern int unpausePin;
extern int primePin; //need to add this button in the circuit

extern int solenoidPins[];
extern int IRPins[];
extern int washPins[];
extern int elutionPins[];
extern int sterilizationPins[];
extern int ignore[];


extern int mode; //Normal mode = 0, Priming mode = 1 (Default should be prime mode)
extern int check;
extern boolean stopState;
extern boolean pauseState;
extern boolean startState;
extern boolean sterilizationCheck;
extern boolean IRPrimed;
extern boolean washPrimed;
extern boolean elutionPrimed;

extern boolean printCheck;
extern int* solutionPins[3];


void writeLine(LiquidCrystal lcd, String string, int line);
void countDown(LiquidCrystal lcd, unsigned long timeLeft);
void tempWrite (LiquidCrystal lcd, String string);
unsigned long pauseCheck (int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]);
void motorStop(int motorPins[]);
void runForward(int motorPins[]);
void runBackward(int motorPins[]);
void changeMode();

void enterRunningMode(int,String);

boolean postBuffer(unsigned long rxnTime, int solenoidPins[], unsigned long solenoidTime, int stopPin, boolean stopState, LiquidCrystal lcd);
boolean runBuffer(int motorPins[], unsigned long bufferTime, int stopPin, int pausePin, int unpausePin, boolean pauseState, LiquidCrystal lcd);

void runNormalMode();
void runPrimingMode();


#endif
