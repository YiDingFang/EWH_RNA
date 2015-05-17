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


#include <Arduino.h>

//LCD setup
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int mode = 0; //Normal mode = 0, Priming mode = 1
int check=1;
boolean stopState=false;
boolean pauseState=false;
boolean startState=false;
boolean sterilizationCheck=false;
boolean IRPrimed = false;
boolean washPrimed = false;
boolean elutionPrimed = false;

int stopPin=50;
int startPin=26;
int pausePin=24;
int unpausePin=22;
int primePin = 22; //need to add this button in the circuit

unsigned long primeTime = 5000;

int solenoidPins[]= {1, 2};
int IRPins[] = {33, 32};
int washPins[] = {43, 42};
int elutionPins[] = {35, 34};
int sterilizationPins[] = {43, 42};
int ignore[] = {-1, -1};

int* solutionPins[3] = {IRPins, washPins, elutionPins};

boolean printCheck = false;

void writeLine(LiquidCrystal lcd, String string, int line);
void countDown(LiquidCrystal lcd, unsigned long timeLeft);
void tempWrite (LiquidCrystal lcd, String string);
unsigned long pauseCheck (int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]);
void motorStop(int motorPins[]);
void runForward(int motorPins[]);
void runBackward(int motorPins[]);
void changeMode();
boolean enterRunningMode(int,String);
void runNormalMode();
void runPrimingMode();


double voltsToFlow(int rawADC) //Analog to Digital Conversion
{
  //do something w/ the input from an Output Pin...hmm, maybe I should rename those...
  int magic = 1;
  double flowRate = rawADC * magic;  //magic is performed, i.e. blackbox that needs to be filled in
  return flowRate;
}


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
 * Function postBuffer
 * Function inline inline boolean postBuffer(int airPins[], unsigned long airTime, unsigned long rxnTime, 
                                             int solenoidPins[], unsigned long solenoidTime, int stopPin, 
                                             boolean stopState, LiquidCrystal lcd){
 * Description:        
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
 
inline boolean postBuffer(unsigned long rxnTime, int solenoidPins[], unsigned long solenoidTime, int stopPin, boolean stopState, LiquidCrystal lcd){

  //incubate
  writeLine(lcd, "INCUBATE", 2);
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  (lcd).setCursor(17,2);
  (lcd).print(":");
  unsigned long timeRxn=rxnTime + millis();
  while(millis()<timeRxn){
    if(digitalRead(stopPin)){
      return true;
    }
    unsigned long timeDif = timeRxn - millis();
    timeRxn = timeDif + pauseCheck(pausePin, unpausePin, lcd, ignore);
    countDown(lcd, timeRxn - millis());
  }
  //vacuum
  writeLine(lcd, "VACUUM", 2);
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  (lcd).setCursor(17,2);
  (lcd).print(":");
  unsigned long timeSolenoid = solenoidTime + millis();
  while(millis()<timeSolenoid){
    runForward(solenoidPins);
    if(digitalRead(stopPin)){
      motorStop(solenoidPins);
      return true;
    }
    unsigned long timeDif = timeSolenoid - millis();
    timeSolenoid = timeDif + pauseCheck(pausePin, unpausePin, lcd, solenoidPins);
    countDown(lcd, timeSolenoid - millis());
  }
  motorStop(solenoidPins);
  (lcd).setCursor(0,2);
  (lcd).print("                    ");
  (lcd).setCursor(0,3);
  (lcd).print("                    ");
  return false;
}

/* 
 * Function pauseCheck
 * Function iunsigned long pauseCheck(boolean pauseState, 
 *                                    int pausePin, int unpausePin, 
 *                                    LiquidCrystal lcd, int motorPins[])
 * Description: check if the pauseButton has been pressed between now and the end       
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
unsigned long pauseCheck(int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]){
/*
  do{
    //if paused stop the motor
    if(pauseState) {
      motorStop(motorPins);
      // report the pause
      if(!printCheck){
        Serial.print("PAUSED");
        (lcd).home();
        (lcd).print("                    ");
        (lcd).home();
        (lcd).print("PAUSED");
        printCheck = true;
      }
      goto UNPAUSE;
    }    
    pauseState=digitalRead(pausePin);
    if(pauseState){
      motorStop(motorPins);
      if(!printCheck){
        Serial.print("PAUSED");
        (lcd).home();
        (lcd).print("                    ");
        (lcd).home();
        (lcd).print("PAUSED");
        printCheck = true;
      }
UNPAUSE: 
      pauseState=!digitalRead(unpausePin);
      if(!pauseState){
        Serial.print("UNPAUSED");
        tempWrite(lcd, "UNPAUSED");
        printCheck = false;
        runForward(motorPins);
      }
    }
  }while (pauseState);
  return millis();

*/  
  do{
     //pauseState=digitalRead(pausePin);
    // check the state of the pausePin
    if(pauseState){
      motorStop(motorPins);
      if(!printCheck){
        Serial.print("PAUSED");
        (lcd).home();
        (lcd).print("                    ");
        (lcd).home();
        (lcd).print("PAUSED");
        printCheck = true;
      } 
      pauseState=!digitalRead(unpausePin);
      if(!pauseState){
        Serial.print("UNPAUSED");
        tempWrite(lcd, "UNPAUSED");
        printCheck = false;
        runForward(motorPins);
      }
    }
    else{
      pauseState=digitalRead(pausePin);
    }
  }while (pauseState);
  return millis();
 
}

/* 
 * Function runBuffer
 * Function inline boolean runBuffer(int motorPins[], unsigned long bufferTime,
 *                                   int stopPin, int pausePin, int unpausePin, 
 *                                   boolean pauseState, LiquidCrystal lcd)
 * Description: execute the runBuffer sequence           
 * Parameters: motorPins[] refers to an array containing the pins of the motors
 *             bufferTime refers to the run time of the sequence
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */
 
inline boolean runBuffer(int motorPins[], unsigned long bufferTime, int stopPin, int pausePin, int unpausePin, boolean pauseState, LiquidCrystal lcd){
  // set the flowrate
  double flowRate = 1;
  // set up the LCD display based on runBuffer
  // set cursor and write TIME to screen
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  // set cursor and write VOLUME to screen
  (lcd).setCursor(8,3);
  (lcd).print("VOLUME: ");
  // set cursor and write : to screen
  (lcd).setCursor(17,2);
  (lcd).print(":");
  
  // set the end time
  unsigned long duration = bufferTime + millis();
  
  // while the interval is not over, run the motorPins
  while(millis() < duration){
    runForward(motorPins);
    // if stop is pressed at any time, return true
    if(digitalRead(stopPin)){
      motorStop(motorPins);
      return true;
    }
    // record the amount of time remaining
    unsigned long timeDif = duration - millis();
    // reset the end time withthe remaining time to accomodate for pause duration
    duration = timeDif + pauseCheck(pausePin, unpausePin, lcd, motorPins);
    
    // lcd prints volume
    unsigned long volume = (unsigned long)flowRate*(millis()%60000)/1000;  //need to revise length of volume digits once know more definite flowrate and final volumes***************************************
    // move cursor to bot
    (lcd).setCursor(17,3);
    (lcd).print(volume);
    //lcd prints time remaining every second - in theory; I'm not sure if the time it takes to process the prior code will mess with how the code keeps track of time; will need to test empirically
    countDown(lcd, duration - millis());
  }
  // stop the motorPins
  motorStop(motorPins);
  return false;
}


/*
 * Function changeMode
 * Function void changeMode()
 * Description: changes the current mode between normal mode (mode = 0) and priming mode (mode = 1)
 * Parameters: None
 * Error Conditions: None
 * Return Value: None
*/

void changeMode()
{
  if(mode = 0)
    mode = 1;
  else if (mode = 1)
    mode = 0;
}

/*
 * Function runNormalMode
 * Function void runNormalMode()
 * Runs the normal running mode
 * Parameter: None
 * Error Condition: None
 * Return Value: None
*/

void runNormalMode()
{
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
  
  switch(currentIndex)
  {
    case 0: currentSolution = "IR";break;
    case 1: currentSolution = "Elution";break;
    case 2: currentSolution = "Wash";break;
    default: return;
  }
  
  writeLine(lcd,"Select Solution to Prime: " + currentSolution,1);
  writeLine(lcd,"Press and unpause to change solution", 2);
 
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
  
  else if (digitalRead(startPin))
  {
    if(enterRunningMode(currentIndex))
      writeLine(lcd, "Selected priming completed", 1);
    else
    {
      writeLine(lcd, "Error occurred during priming", 1);
    } 
  }
 
  else if (digitalRead(stopPin))
    changeMode();
  return;
}

/*
  Function: enterRunningMode
  Function: boolean enterRunningMode(int solutionIndex)
  Description: Begins the running mode for the selected solution index. This index will be used to call the
               motor pins for the solution.
  Parameters: Index to begin flow of solution.
  Error Conditions: Index Out of Bounds
  Return Value: True if priming is completed properly. Otherwise, returns false.
*/
boolean enterRunningMode(int solutionIndex, String solutionName)
{
  boolean runningComplete = false;
  String currentSolution;
  writeLine(lcd, "Selected Buffer: " + solutionName, 1);
  writeLine(lcd, "Hold start to perform priming",2);
  while(digitalRead(startPin))
  {
    runForward(solutionPins[solutionIndex]);
  }
  motorStop(solutionPins[solutionIndex]);
  runningComplete = true;
  return runningComplete;
}


/* 
 * Function primeBuffers
 * Function void primeBuffers()
 * Description: execute the primeBuffers sequence           
 * Parameters: 
 * Side Effects: None
 * Error Conditions: None.
 * Return Value:
 */

/*
 * We probably need some sort of static variable to reset once the machine is turned off for the day, and a method that will reset the primed booleans to false and either withdraw all remaining buffer in the tubes
 * Maybe just an "Off Button," which means the prime Button could just be an "On Button"
 * Also, I think we can probably pull a lot of our variables out of the parameters, and just let them be defined at the beginning of the header (which we already pretty much do); hence, no parameters for this method required
 * Another potential concern: if the machine is stopped during the priming step, maybe there should be a step that withdraws all remaining fluid in that tube so there's no overflow next time
 */
// 
//void primeBuffers(){
//  
//  if(!IRPrimed){
//    writeLine(lcd, "PRIMING IR", 1);
//    stopState = runBuffer(IRPins, primeTime, stopPin, pausePin, unpausePin, pauseState, lcd);
//    if(stopState) {
//      tempWrite(lcd, "STOP");
//      return;
//    }
//    IRPrimed = true;
//  }
//  tempWrite(lcd, "IR PRIMED");
//  
//  if(!washPrimed){
//    writeLine(lcd, "PRIMING WASH", 1);
//    stopState = runBuffer(washPins, primeTime, stopPin, pausePin, unpausePin, pauseState, lcd);
//    if(stopState) {
//      tempWrite(lcd, "STOP");
//      return;
//    }
//    washPrimed = true;
//  }
//  tempWrite(lcd, "WASH PRIMED");
//  
//  if(!elutionPrimed){
//    writeLine(lcd, "PRIMING ELUTION", 1);
//    stopState = runBuffer(elutionPins, primeTime, stopPin, pausePin, unpausePin, pauseState, lcd);
//    if(stopState) {
//      tempWrite(lcd, "STOP");
//      return;
//    }
//    elutionPrimed = true;
//  }
//  tempWrite(lcd, "ELUTION PRIMED");
//  
//  
//  writeLine(lcd, "                    ", 1);
//  tempWrite(lcd, "BUFFERS PRIMED");
//  
//  //add vacuum step?
//}

/* 
 * Function name: tempWrite()
 * Function prototype:void tempWrite (LiquidCrystal lcd, String string)
 * Description: write a line of text to the LCD and then clear           
 * Parameters: LCD refers to the LiquidCrystal display to print to
 *             String refers to the message to be printed
 * Side Effects: clears the screen
 * Error Conditions: None.
 * Return Value: void
 */
void tempWrite (LiquidCrystal lcd, String string){
  (lcd).home();
  (lcd).print("                    ");
  (lcd).home();
  (lcd).print(string);
  
  // set the end time to be used for a 3 second delay
  unsigned long time = millis() + 3000;
  while(millis()<time){}
  
  // if STOP is written, clear the LCD and exit
  if(string.equals("STOP")) {
    (lcd).clear();
    return;
  }
  
  (lcd).home();
  (lcd).print("                    ");
}

/* 
 * Function name: writeLine
 * Function prototype: void writeLine(LiquidCrystal lcd, String string, int line){
 * Description: write a line of text to the LCD       
 * Parameters: lcd refers to the LiquidCrystal display to print to
 *             string refers to the message to be printed
 *             line refers to the line to print to
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */ 
void writeLine(LiquidCrystal lcd, String string, int line){
  // set cursor to appropriate line
  (lcd).setCursor(0,line);
  // clear line
  (lcd).print("                    ");
  // set cursor to appropriate line
  (lcd).setCursor(0,line);
  // print string
  (lcd).print(string);
}

/* 
 * Function name: countDown()
 * Function prototype:void countDown(LiquidCrystal lcd, unsigned long timeLeft)
 * Description: display countdown from the specified time on the LCD           
 * Parameters: LCD refers to the LiquidCrystal display to print to
 *             timeLeft refers to the time to count down from
 * Side Effects: none
 * Error Conditions: None.
 * Return Value: void
 */
void countDown(LiquidCrystal lcd, unsigned long timeLeft){
  // convert to milliseconds
  unsigned long minutes = timeLeft/60000;
  unsigned long seconds = (timeLeft%60000)/1000;
  
  // set cursor to bottom right of the screen
  (lcd).setCursor(16,2);
  
  // print minutes
  (lcd).print(minutes);
  (lcd).setCursor(18,2);
  
  // print seconds
  // add a placeholder 0 in front of the seconds
  if(seconds < 10){
    (lcd).print(0);
  }
  (lcd).print(seconds);
}

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


/* 
 ** NOTES ABOUT THE LCD
 ** The first line will be reserved for "START," "STOP," "PAUSED," and "UNPAUSED," as well as other 'temporary' alerts.  Essentially, anything that calls the tempWrite() function will write the text on the first line.
 ** The second line will contain the name of the buffer round.
 ** The third line will contain time, plus the current stage of the round (i.e. buffer, air, incubate, vacuum).
 ** The fourth line will contain the volume.
 */

