/*
 * Filename: countDown
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

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
 ** NOTES ABOUT THE LCD
 ** The first line will be reserved for "START," "STOP," "PAUSED," and "UNPAUSED," as well as other 'temporary' alerts.  Essentially, anything that calls the tempWrite() function will write the text on the first line.
 ** The second line will contain the name of the buffer round.
 ** The third line will contain time, plus the current stage of the round (i.e. buffer, air, incubate, vacuum).
 ** The fourth line will contain the volume.
 */
