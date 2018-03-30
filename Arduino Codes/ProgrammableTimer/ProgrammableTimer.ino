// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

#include <EEPROM.h>
#include <EEWrap.h>

struct store {
  int16_e HOUR_ON_STORE;
  int16_e MINUTE_ON_STORE;
  int16_e HOUR_OFF_STORE;
  int16_e MINUTE_OFF_STORE;
  int16_e RECALL;
};

store call EEMEM;

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String hour_on;
String minute_on;

String hour_off;
String minute_off;

int hr_on;
int mn_on;

int hr_off;
int mn_off;

int HOURS;
int MINUTES;

const int Blue = 11;
const int Green = 12;
const int Red = 13;

boolean ON_BOOL = true;
boolean OFF_BOOL = false;

boolean activated = false;
boolean deactivated = false;

void setup () {
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (call.RECALL == 1) {
    Serial.println("POWER LOSS WHEN ON");
    ON_BOOL = false;
    OFF_BOOL = true;
    activated = true;
  }

  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  digitalWrite(Red, HIGH);
  digitalWrite(Blue, LOW);
  digitalWrite(Green, HIGH);

  hr_on = call.HOUR_ON_STORE;
  mn_on = call.MINUTE_ON_STORE;
  hr_off = call.HOUR_OFF_STORE;
  mn_off = call.MINUTE_OFF_STORE;

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(" OMSC");
  display.display();

  delay(1000);
  display.clearDisplay();
  display.display();

  digitalWrite(Blue, HIGH);

  Print();
}

void loop () {
  displayDATE();
  serial();
  check();
}


void Print() {
  Serial.print("ON TIME- ");
  Serial.print(hr_on);
  Serial.print(":");
  Serial.print(mn_on);

  Serial.println();

  Serial.print("OFF TIME- ");
  Serial.print(hr_off);
  Serial.print(":");
  Serial.print(mn_off);

  Serial.println();
}


void check() {
  //Serial.println(hr_on + mn_on + hr_off + mn_off);

  if (HOURS == hr_on && MINUTES == mn_on && ON_BOOL == true) {
    Serial.println("Device is on");

    call.RECALL = 1;

    ON_BOOL = false;
    OFF_BOOL = true;

    activated = true;
  }

  if (HOURS == hr_off && MINUTES == mn_off && OFF_BOOL == true) {
    Serial.println("Device is off");
    ON_BOOL = true;
    OFF_BOOL = false;

    call.RECALL = 0;

    activated = false;
  }


  if (activated == true) {
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
  } else {
    digitalWrite(Red, LOW);
    digitalWrite(Green, HIGH);
  }

}

void serial() {
  if (Serial.available() > 0) {

    digitalWrite(Red, HIGH);
    digitalWrite(Green, HIGH);
    digitalWrite(Blue, LOW);

    hour_on = Serial.readStringUntil(',');
    minute_on = Serial.readStringUntil(',');
    hour_off = Serial.readStringUntil(',');
    minute_off = Serial.readStringUntil(',');

    hr_on = hour_on.toInt();
    mn_on = minute_on.toInt();
    hr_off = hour_off.toInt();
    mn_off = minute_off.toInt();

    call.HOUR_ON_STORE = hr_on;
    call.MINUTE_ON_STORE = mn_on;
    call.HOUR_OFF_STORE = hr_off;
    call.MINUTE_OFF_STORE = mn_off;

    Print();


    delay(1000);
    digitalWrite(Blue, HIGH);

  }
}


void displayDATE() {
  DateTime now = rtc.now();

  /*
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
  */

  HOURS = now.hour();
  MINUTES = now.minute();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(String(now.year()) + String("/") + String(now.month()) + String("/") + String(now.day()) + String("  ") + String(daysOfTheWeek[now.dayOfTheWeek()]));

  display.setCursor(0, 8);
  display.print(String(now.hour()) + String(":") + String(now.minute()) + String(":") + String(now.second()));

  display.setCursor(0, 16);
  display.print("ON TIME- ");
  display.print(hr_on);
  display.print(":");
  display.print(mn_on);

  display.println();

  display.print("OFF TIME- ");
  display.print(hr_off);
  display.print(":");
  display.print(mn_off);

  display.println();

  display.display();

  display.clearDisplay();
}
