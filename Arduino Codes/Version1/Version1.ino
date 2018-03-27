// 27/03/2018 6:46PM

#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float ambient;
float object;

boolean AMBIENT = false;
boolean OBJECT = false;

int compareChoose;
float compareOne;
float compareTwo;
float compareThree;
float compareFour;


const int in1 = 9;
const int in2 = 10;
const int in3 = 11;
const int in4 = 12;

String choose;
String one;
String two;
String three;
String four;

void setup() {
  Serial.begin(9600);
  Serial.println("OMSC");

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);

  mlx.begin();
}

void loop() {
  readSensor();
  serial();
  compare();
}

void readSensor() {
  ambient = mlx.readAmbientTempC();
  object = mlx.readObjectTempC();
  Serial.print(String("Ambient: ") + ambient);
  Serial.println(String("Object: ") + object);
}

void serial() {
  if (Serial.available() > 0) {
    choose = Serial.readStringUntil(',');
    one = Serial.readStringUntil(',');
    two = Serial.readStringUntil(',');
    three = Serial.readStringUntil(',');
    four = Serial.readStringUntil('#');

    compareChoose  = choose.toInt();
    compareOne = one.toFloat();
    compareTwo = two.toFloat();
    compareThree = three.toFloat();
    compareFour = four.toFloat();

    Serial.println(choose + one + two + three + four);

    if (compareChoose == 1) {
      Serial.println("Detecting Ambient");
      AMBIENT = true;
      OBJECT = false;
    } else if (compareChoose == 2) {
      Serial.println("Detecting Object");
      OBJECT = true;
      AMBIENT = false;
    }

  }
}

void compare() {
  if (AMBIENT == true) {
    checkAmbient();
  }

  if (OBJECT == true) {
    checkObject();
  }

}

void checkAmbient() {

  if (compareOne < ambient && compareTwo > ambient) {
    Serial.println("Level One");
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

  if (compareTwo < ambient && compareThree > ambient) {
    Serial.println("Level Two");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

  if (compareThree < ambient && compareFour > ambient) {
    Serial.println("Level Three");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  if (compareFour < ambient) {
    Serial.println("Level Four");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  if (compareOne > ambient && compareTwo > ambient && compareThree > ambient && compareFour > ambient) {
    Serial.println("OFF");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

}

void checkObject() {

  if (compareOne < object && compareTwo > object) {
    Serial.println("Level One");
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

  if (compareTwo < object && compareThree > object) {
    Serial.println("Level Two");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

  if (compareThree < object && compareFour > object) {
    Serial.println("Level Three");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  if (compareFour < object) {
    Serial.println("Level Four");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  if (compareOne > object && compareTwo > object && compareThree > object && compareFour > object) {
    Serial.println("OFF");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

}
