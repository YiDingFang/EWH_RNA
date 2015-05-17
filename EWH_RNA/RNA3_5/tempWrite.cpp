/*
 * Filename: tempWrite
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

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
 ** NOTES ABOUT THE LCD
 ** The first line will be reserved for "START," "STOP," "PAUSED," and "UNPAUSED," as well as other 'temporary' alerts.  Essentially, anything that calls the tempWrite() function will write the text on the first line.
 ** The second line will contain the name of the buffer round.
 ** The third line will contain time, plus the current stage of the round (i.e. buffer, air, incubate, vacuum).
 ** The fourth line will contain the volume.
 */
