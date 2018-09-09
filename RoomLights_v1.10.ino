//Include date and time for DS3231 RTC via I2C and Wire library
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc; //create an object?

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //set up days of the week

int WLED = 3; //white led on pin 3 for analog output
int RLED = 11; //red led on pin 11 for analog output
int GLED = 9; //green led on pin 9 for analog output
int BLED = 10; //blue led on pin 10 for analog output
int LEDS_ON = 2; //create the white on option
int Dimmer = A0; //create read in variable for dimmer potentiometer
int LEDS_ON_OFF = 0; //initialize read in variable for white LED
int Brightness = 0; //initialize brightness for output
int timeOfDay = 0; //store the time to test for printing out

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pins as outputs.
  pinMode(WLED, OUTPUT);
  pinMode(RLED, OUTPUT); //the wire connected to the LED strip for the RED LEDS is GREEN
  pinMode(GLED, OUTPUT); //the wire connected to the LED strip for the GREEN LEDS is RED     THEY ARE FLIPPED
  pinMode(BLED, OUTPUT);
  pinMode(LEDS_ON, INPUT);
  pinMode(Dimmer, INPUT);

  //setup for RTC module
#ifndef ESP8266 //define ESP8266 to set up serial monitor
  while (!Serial);
#endif

  Serial.begin(9600); //set 9600 baud rate
  delay(3000); //wait for console to open

  if (! rtc.begin()) { //if the RTC module isn't being read
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}



// the loop routine runs over and over again forever:
void loop() {

  RTC(); //obtain the date and time

  Brightness = analogRead(Dimmer) / 4; //read in an input from the potentiometer for brightness output

  lightOutput(timeOfDay);

  /*THIS NEEDS TO BE MADE AN INTERRUPT*/
  LEDS_ON_OFF = digitalRead(LEDS_ON); //check if the white LED should be on
  /*
    if (LEDS_ON_OFF == LOW) //if the switch designates the white LED should be off
    {
      analogWrite(WLED, 0); //turn off the white LED
      analogWrite(BLED, 0); //turn off the blue LED
      analogWrite(GLED, 0); //turn off the green LED
      analogWrite(RLED, 0); //turn off the red LED
  */
  /*
    if (CONOFF < 256) //if the potentiometer is less than .488 volts turn off the RGB LEDS
    {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, LOW);
    }

    else if (256 < CONOFF && CONOFF < 512) //if the potentiometer is between 1/4 and 1/2, turn on black light
    {
    analogWrite(RLED,167); //black light fluorescent
    analogWrite(GLED, 0);
    analogWrite(BLED, 255);
    }

    else if (512 < CONOFF && CONOFF < 768)
    {
    analogWrite(RLED, 255);  //candle flame
    analogWrite(GLED, 147);
    analogWrite(BLED, 41);
    }
    else if (768 < CONOFF && CONOFF < 1024)
    {
    analogWrite(RLED, 64); //clear blue sky
    analogWrite(GLED, 156);
    analogWrite(BLED, 255);
    }
  */
}
/*
  else if (LEDS_ON_OFF == HIGH) //if the white LED should be on
  {
    if (timeOfDay == 0 || timeOfDay == 10 || timeOfDay == 20 || timeOfDay == 30 || timeOfDay == 40 || timeOfDay == 50)
    {
      analogWrite(WLED, Brightness); //WLED on
      Serial.print(timeOfDay);
      Serial.println();
    }
*/
/*     analogWrite(RLED, Brightness); //turn on the RGBs
     analogWrite(GLED, Brightness);
     analogWrite(BLED, Brightness);
*/
/* if (now.second() == 3)
  {
   analogWrite(RLED, Brightness);
  }
  else if (now.second() == 5)
  {
   analogWrite(GLED, Brightness);
  }
  else if (now.second() == 7)
   analogWrite(RLED, Brightness);
  else
  {
   analogWrite(WLED, 0);
   analogWrite(RLED, 0);
   analogWrite(GLED, 0);
   analogWrite(BLED, 0);
  }
*/
/*
  }
  }
*/
//additions needed

//function for RTC
//arduino should take input to determine the time of day and change the brightness of lights accordingly

/*
    Function Name: RTC
    Description: This function is called to obtain the date and time. It will print the current date and time over the serial monitor.
    Inputs: None
    Outputs: Prints out the current date and time to the serial monitor.
*/
void RTC() {

  DateTime now = rtc.now(); //obtain the date and time

  Serial.print(now.year(), DEC); //print the year
  Serial.print('/');
  Serial.print(now.month(), DEC); //print the month
  Serial.print('/');
  Serial.print(now.day(), DEC); //print the day
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //print the day of the week
  Serial.print(") ");
  Serial.print(now.hour(), DEC); //print the hour
  Serial.print(':');
  Serial.print(now.minute(), DEC); //print the minute
  Serial.print(':');
  Serial.print(now.second(), DEC); //print the second
  Serial.println(); //print a new line

  timeOfDay = now.hour();
}

/*
    Function Name: lightOutput
    Description: This function takes in the time of day in hours and outputs a
    light color corresponding to what the natural light would be like that time.
    Inputs: int timeOfDay (in hours)
    Outputs: PWM output to LEDs based on the time of day.
*/
void lightOutput(int timeOfDay)
{
  if (LEDS_ON_OFF == LOW) //if the switch designates the LEDs should be off, regardless of the time of day
  {
    fullOff();
  }
  else if (LEDS_ON_OFF == HIGH) //if the switch is turned on, activate the LEDs and set their color to correspond to the time of day
  {
    if (timeOfDay == 12) //if it is 12pm, set the values to "High Noon Sun"
    {
      analogWrite(WLED, 200);
      analogWrite(RLED, 255);
      analogWrite(GLED, 255);
      analogWrite(BLED, 251);
    }

    //if the time of day is between 5am and 7am, and 7pm and 10pm, set the color to "Candle"
    else if (timeOfDay == 5 || timeOfDay == 6 || timeOfDay == 7 || timeOfDay == 19 || timeOfDay == 20 || timeOfDay == 21 || timeOfDay == 20)
    {
      analogWrite(WLED, 0);
      analogWrite(RLED, 255);
      analogWrite(GLED, 147);
      analogWrite(BLED, 41);
    }

    //if the time of day is not noon and is between the rest of the day then set the color the "Sky Blue"
    else if (timeOfDay >= 8 && timeOfDay <= 18)
    {
      analogWrite(WLED, 100);
      analogWrite(RLED, 64);
      analogWrite(GLED, 156);
      analogWrite(BLED, 255);
    }

    //if the time of day is any time at night, set the color to a dark red for the best usage as users can have a low ambient light
    //that doesn't affect their sleep cycle and can still be used to navigate a room
    else
    {
      analogWrite(WLED, 0);
      analogWrite(RLED, 200);
      analogWrite(GLED, 0);
      analogWrite(BLED, 0);
    }
  }
}
//pad input function
//should take a control signal from the keypad and output a different combination of colors

//dimmer
//basic potentiometer dimmer
//needs to work with any set of color outputs

//IR function
//needs to detect IR input and turn lights on/off accordingly

/*
   Function Name: fullOff
   Description: Turns off all LEDs, written to make it quicker to program.
   Input: None
   Output: Turns off all LEDs
*/

void fullOff()
{
  analogWrite(WLED, 0); //turn off the white LED
  analogWrite(RLED, 0); //turn off the blue LED
  analogWrite(GLED, 0); //turn off the green LED
  analogWrite(BLED, 0); //turn off the red LED
}
