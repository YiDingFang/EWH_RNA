
#include <Arduino.h>

//LCD setup
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int check=1;
boolean stopState=false;
boolean pauseState=false;
boolean startState=false;
boolean sterilizationCheck=false;

int stopPin=50;
int startPin=26;
int pausePin=24;
int unpausePin=22;

int airPins[] = {2, 3};
int solenoidPins[]= {2, 3};
int IRPins[] = {2, 3};
int washPins[] = {2, 3};
int elutionPins[] = {2, 3};
int sterilizationPins[] = {2, 3};
int ignore[] = {-1, -1};

boolean printCheck = false;

void writeLine(LiquidCrystal lcd, String string, int line);
void countDown(LiquidCrystal lcd, unsigned long timeLeft);
void tempWrite (LiquidCrystal lcd, String string);
unsigned long pauseCheck (boolean pauseState, int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]);
void motorStop(int motorPins[]);
void runForward(int motorPins[]);
void runBackward(int motorPins[]);

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
 
inline boolean postBuffer(int airPins[], unsigned long airTime, unsigned long rxnTime, int solenoidPins[], unsigned long solenoidTime, int stopPin, boolean stopState, LiquidCrystal lcd){
  
  writeLine(lcd, "AIR", 2);
  //prints TIME to the LCD
  (lcd).setCursor(10,2);
  (lcd).print("TIME: ");
  (lcd).setCursor(17,2);
  (lcd).print(":");
  
  // set the end time for the Air round
  unsigned long timeAir = airTime + millis();
  // while the interval has not ended
  while(millis()<timeAir){
    // run the airPump
    runForward(airPins);
    
    // if stop is pressed, end proccess
    if(digitalRead(stopPin)){
      motorStop(airPins);
      return true;
    }
    
    // keep track of remaining time, should we pause the cycle
    unsigned long timeDif = timeAir - millis();
    // reset air time to accomodate for time lost
    timeAir = timeDif + pauseCheck(pauseState, pausePin, unpausePin, lcd, airPins);
    countDown(lcd, timeAir-millis());
  }
  motorStop(airPins);

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
    timeRxn = timeDif + pauseCheck(pauseState, pausePin, unpausePin, lcd, ignore);
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
    timeSolenoid = timeDif + pauseCheck(pauseState, pausePin, unpausePin, lcd, solenoidPins);
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
unsigned long pauseCheck(boolean pauseState, int pausePin, int unpausePin, LiquidCrystal lcd, int motorPins[]){
/*
  do{
    //if puased stop the motor
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
    duration = timeDif + pauseCheck(pauseState, pausePin, unpausePin, lcd, motorPins);
    
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

