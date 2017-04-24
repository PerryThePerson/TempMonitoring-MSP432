

#include <Wire.h>
#include "itoa.h"
#include "Screen_HX8353E.h"
#include "Adafruit_MCP9808.h"

#define BUTTON_1 33
#define BUTTON_2 32

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
Screen_HX8353E myScreen;
int safeTemp = 25;

void setup() {

  Serial.begin(9600);   //sets up serial port
  delay(100);
  Wire.begin();         //instantiates wire

  
  myScreen.begin();     //intializes screen
  myScreen.clear(blackColour); //sets background color


  pinMode(BUTTON_1, INPUT_PULLUP);    //sets pinmode of button 1 to input_pullup
  pinMode(BUTTON_2, INPUT_PULLUP);    //sets pinmode of button 2 to input_pullup

  attachInterrupt(BUTTON_1, increaseSafeTemp, FALLING); //attachs interrupt to increase safe temp when button is pressed
  attachInterrupt(BUTTON_2, decreaseSafeTemp, FALLING); //attachs interrupt to increase safe temp when button is pressed
  
  tempsensor.begin(0x18); //intializes temperature sensor to address 0x18

  
}

void loop() 
{

  drawScreen();   //updates screen
  
}
/*
 * Function: drawScreen
 *----------------------------- 
 * Reads and converts temperatures to strings and 
 * updates the screen 
 *
 */
void drawScreen(void)
{
   String strSafeTemp = String(safeTemp);
   char CPUTemp[10];  //char array to store float as string
   

   tempsensor.wake();     //wakes up sensor if registers are not set properly
   float tempSensorCelcius = tempsensor.readTempC(); //reads temperature sensors

  ftoa(CPUTemp, tempSensorCelcius, 1);

   //updates screen
   myScreen.gText(4, 10, "Safe Temp. Sensor", redColour);
   myScreen.gText(4, 30, "Safe temperature: " + strSafeTemp, redColour); 
   myScreen.gText(4, 50, "CPU1 Temp: ", redColour);
   myScreen.gText(75, 50, CPUTemp, redColour);
   myScreen.gText(100, 50, " C" , redColour);

   //sends shutdown signal to the serial port, read by the python script
   if(tempSensorCelcius > safeTemp)
   {

      Serial.println("Shut down");

   }  
}





/*
 * Function: increaseSafeTemp
 *----------------------------- 
 * increases the shutoff temperate threshold
 * of the temperature sensor
 *
 */

void increaseSafeTemp(void)
{
  safeTemp++;

}
/*
 * Function: decreaseSafeTemp
 *----------------------------- 
 * decreases the shutoff temperate threshold
 * of the temperature sensor
 *
 */
 
void decreaseSafeTemp(void)
{
  safeTemp--;

}

/*
 * Function:  ftoa
 * --------------------
 * converts a float to a character array
 *
 *  *a = character array where output string will be store
 *  f = the input number
 *  precision = Number of digits to be considered after point (max: 7)
 *
 *  returns: character array containing float
 *          
 *           
 */
char *ftoa(char *a, float f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}



