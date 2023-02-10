#include <Arduino.h>
// #include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include "NewPing.h"

#include <Wire.h>


#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)


float temp_amb;
float temp_obj;



// Hook up HC-SR04 with Trig to Arduino Pin 9, Echo to Arduino pin 10
#define TRIGGER_PIN 10
#define ECHO_PIN 9

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

#define laser 5


Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int btVal = 0;
int button = 2;
int relay = 7;


void dist() {

  // Send ping, get distance in cm
  distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println(" cm");
  // Send results to Serial Monitor

  if (distance <= 10 && distance>2)
  {
    digitalWrite(relay, HIGH);
    delay(1000);
    digitalWrite(relay, LOW);
    delay(10000);
  }
  else if (distance>10)
  {
    digitalWrite(relay, LOW);
    
  }
}

void tempCheck() {
  btVal = digitalRead(button);
  if (btVal == 1) {
    digitalWrite(laser, LOW);
    delay(500);
    temp_obj = mlx.readObjectTempC();
    delay(1000);
    digitalWrite(laser, HIGH);
    Serial.print("Object temp = ");
    Serial.println(temp_obj);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setFont(&FreeSansBold24pt7b);
    display.setCursor(0, 45);
    display.setTextSize(1);
    display.print(temp_obj);
    display.print((char)247);
    display.print("C");
    display.display();
    delay(5000);
    // put your main code here, to run repeatedly:
  }
}
void setup()
{
  Serial.begin(9600);
  mlx.begin();
  Serial.println("Temperature Sensor MLX90614");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)

  pinMode(laser, OUTPUT); // Connect LASER
  pinMode(relay,OUTPUT);
  digitalWrite(relay, LOW);
  digitalWrite(laser, HIGH);

  delay(1000);
  display.clearDisplay();
  display.setCursor(25, 15);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Thermometer");
  display.setCursor(25, 35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(2000);
}
void loop() {
  display.clearDisplay();
  display.setCursor(13, 25);
  display.setFont(&FreeSerifBold9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Press Button");
  display.setCursor(33, 50);
  display.setTextSize(1);
  display.print("to Scan");
  display.display();
  delay(500);
  digitalWrite(relay, LOW);

  
  tempCheck();
  dist();
}
