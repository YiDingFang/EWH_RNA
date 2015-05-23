/*
 * Filename: changeMode
 * Author: UCSD_EWH RNA
 * Description: Header file containing methods for RNA extraction.
 * Date: 

 * Sources of Help:
 */


#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RNA.h"

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
  if(mode == 0)
    mode = 1;
  else if (mode == 1)
    mode = 0;
    
    normalCheck = false;
}
