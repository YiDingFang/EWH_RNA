/*
 * Filename: writeLine
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */
 
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

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
 ** NOTES ABOUT THE LCD
 ** The first line will be reserved for "START," "STOP," "PAUSED," and "UNPAUSED," as well as other 'temporary' alerts.  Essentially, anything that calls the tempWrite() function will write the text on the first line.
 ** The second line will contain the name of the buffer round.
 ** The third line will contain time, plus the current stage of the round (i.e. buffer, air, incubate, vacuum).
 ** The fourth line will contain the volume.
 */
