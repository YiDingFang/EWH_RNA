#include <LiquidCrystal.h>
#include "RNA3_5.h"

void setup() {
  lcd.begin(20, 4);
  pinMode(startPin, INPUT);
  pinMode(pausePin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(unpausePin, INPUT);
  pinMode(airPins[0], OUTPUT);
  pinMode(airPins[1], OUTPUT);
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
  stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);
  
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
  
  // 
  stopState=digitalRead(stopPin);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }

  if(check==1)
  {
    // write the current round to the LCD
    writeLine(lcd, "IR ROUND", 1);
    writeLine(lcd, "BUFFER", 2);
    stopState = runBuffer(IRPins, 5000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    check++;
  }

  stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  tempWrite(lcd, "IR ROUND COMPLETE");




  //WASH BUFFER ROUND 1 

  //run wash buffer for 5000ms
  if(check==2)
  {
    writeLine(lcd, "WASH ROUND1", 1);
    writeLine(lcd, "BUFFER", 2);
    stopState = runBuffer(washPins, 5000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    check++;
  }

  stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  tempWrite(lcd, "WASH ROUND1 COMPLETE");  

  //WASH ROUND 2    
  if(check==3)
  {
    writeLine(lcd, "WASH ROUND2", 1);
    writeLine(lcd, "BUFFER", 2);
    stopState = runBuffer(washPins, 5000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    check++;
  }

  stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  tempWrite(lcd, "WASH ROUND2 COMPLETE");   


  //Pause so technician can do the thing  
  writeLine(lcd, "REPLACE SPIN COLUMN", 1);
  pauseState = true;
  pauseCheck(pauseState, pausePin, unpausePin, lcd, ignore);


  //ELUTION BUFFER ROUND


  //run elution buffer
  if(check==4)
  {
    writeLine(lcd, "ELUTION ROUND", 1);
    writeLine(lcd, "BUFFER", 2);
    stopState = runBuffer(elutionPins, 5000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    check=5;
  }

  stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);
  if(stopState) {
    tempWrite(lcd, "STOP");
    return;
  }
  tempWrite(lcd, "ELTN ROUND COMPLETE");


  //pause to exchange waste
  writeLine(lcd, "REMOVE WASTE", 1);
  pauseState = true;
  
  pauseCheck(pauseState, pausePin, unpausePin, lcd, ignore);


  //sterilization round (ethanol)
  if(check==5)
  {
    writeLine(lcd, "STERILIZING...", 1);
    writeLine(lcd, "SOLUTION", 2);
    stopState = runBuffer(sterilizationPins, 5000UL, stopPin, pausePin, unpausePin, pauseState, lcd);
    if(stopState) {
      tempWrite(lcd, "STOP");
      return;
    }
    stopState= postBuffer(airPins, 5000UL, 5000UL, solenoidPins, 5000UL, stopPin, stopState, lcd);  
    lcd.clear();
    tempWrite(lcd, "STERILIZED");
    pauseState = false;
  }   


  tempWrite(lcd, "PROTOCOL COMPLETE");
  lcd.clear();

} // void loop end













