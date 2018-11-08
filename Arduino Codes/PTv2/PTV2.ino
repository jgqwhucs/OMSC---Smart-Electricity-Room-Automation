// Modern Minimalist Clock


/* RTC */
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


/* Neo Pixel */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            6
#define NUMPIXELS      12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a second


/* Mapping Function */

int hours;
int minutes;
int seconds;

int neopixelHour;
int neopixelMinute;
int neopixelSecond;

void setup () {

  /* RTC */
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //uncomment to update RTC clock to CPU clock date and time
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }


  /* Neo pixel */
  pixels.begin();
}

void loop () {
  RTC_FUNCTION();
  UpdateClock();
}

void UpdateClock() {
  pixels.setPixelColor(neopixelSecond, pixels.Color(0, 0, 0)); // Moderately bright green color.
  pixels.setPixelColor(neopixelSecond, pixels.Color(0, 150, 0)); // Moderately bright green color.
  pixels.show();
}

void RTC_FUNCTION() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);

  Serial.println();

  hours = now.hour();
  minutes = now.minute();
  seconds = now.second();

  neopixelHour = map(hours, 1, 24, 1, 12);
  //Serial.println(neopixelHour);

  neopixelMinute = map(minutes, 1, 60, 1, 12);
  //Serial.println(neopixelMinute);

  neopixelSecond = map(seconds, 1, 60, 1, 12);
  Serial.println(neopixelSecond);
}

void NEOPIXEL_FUNTION() {
  for (int i = 0; i < NUMPIXELS; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
