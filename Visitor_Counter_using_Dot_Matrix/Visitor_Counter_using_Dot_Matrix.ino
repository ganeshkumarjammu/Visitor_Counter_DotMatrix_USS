#include "LedControl.h"

//8*8 Dot Matrix
#define CLK 2
#define CS  3
#define DIN 4
#define MAX_DEVICES 1
LedControl Display = LedControl(DIN, CLK, CS, MAX_DEVICES);

//Ultrasonic Sensor
#define trigPin 11    // Trigger
#define echoPin 12    // Echo
#define MaxDistance 100
long duration, distance, maxDistance;
int count = 0;

//Potentiometer 
#define POT A0
//8*8 matrix 8 bit codes
const byte smile[8] =   {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
const byte Number[10][8] = {
  {
    0b00000000,
    0b00111100,
    0b01100110,
    0b01101110,
    0b01110110,
    0b01100110,
    0b01100110,
    0b00111100
  },
  {
    0b00000000,
    0b00011000,
    0b00011000,
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b01111110
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b00000110,
    0b00001100,
    0b00110000,
    0b01100000,
    0b01111110
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b00000110,
    0b00011100,
    0b00000110,
    0b01100110,
    0b00111100
  }, {
    0b00000000,
    0b00001100,
    0b00011100,
    0b00101100,
    0b01001100,
    0b01111110,
    0b00001100,
    0b00001100
  }, {
    0b00000000,
    0b01111110,
    0b01100000,
    0b01111100,
    0b00000110,
    0b00000110,
    0b01100110,
    0b00111100
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b00111100
  }, {
    0b00000000,
    0b01111110,
    0b01100110,
    0b00001100,
    0b00001100,
    0b00011000,
    0b00011000,
    0b00011000
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100
  }, {
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b01100110,
    0b00111100
  }
};

const byte Soham[][8] = {
  { //S
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100000,
    0b00111100,
    0b00000110,
    0b01100110,
    0b00111100
  },
  { //O
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100
  },
  { //H
    0b00000000,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01111110,
    0b01100110,
    0b01100110,
    0b01100110
  },
  { //A
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b01111110,
    0b01100110,
    0b01100110,
    0b01100110
  },
  { //M
    0b00000000,
    0b01100011,
    0b01110111,
    0b01111111,
    0b01101011,
    0b01100011,
    0b01100011,
    0b01100011
  }
};

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Display.shutdown(0, false);      //The MAX72XX is in power-saving mode on startup
  Display.setIntensity(0, 15);     // Set the brightness to maximum value
  Display.clearDisplay(0);
  Serial.println("SOHAM ");
  for (int count = 0 ; count <= 4 ; count++) {
    printByte(Soham[count]);
    delay(500);
  }
  printByte(smile);
  delay(500);
}

void loop() {
  int analogValue = analogRead(POT);
  calcDist();
  int maxDistance = map(analogValue, 0, 1024, 0, MaxDistance+1);
  Serial.print("Distance / MaxDistance : ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print("/");
  Serial.print(maxDistance);
  Serial.println(" cm");
  if (distance < maxDistance) {
    count = count + 1;
    if (count > 9) count = 0 ;
    printByte(Number[count]);
    Serial.println(count);
    delay(1000);
    while (distance < maxDistance) {
      calcDist();
      Serial.print("Distance : ");
      Serial.print(distance);
      Serial.println(" cm");
      Serial.println(count);
      delay(50);
    }
  }
}

void calcDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object
  duration = pulseIn(echoPin, HIGH);
  // Convert the time into a distance
  distance = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
}

void printByte(byte character []) {
  for (int i = 0; i < 8; i++) {
    Display.setRow(0, i, character[i]);
  }
}
