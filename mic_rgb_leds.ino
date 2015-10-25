#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <EEPROM.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor
long baseline = 0;
long diff = 0;
int i = 0;
int j = 0;
int memAddress = 0;
int myMode;
int randMode = 0;
int sensitivity = 4;//1 or 2 = Most sensitive; 10 = Least Sensitive

void setup ()
{
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  //Let's make it more random
  randomSeed(42);
  myMode = int(EEPROM.read(memAddress));
  if (myMode > 5 || myMode < 0) {
    myMode = 0;
  }
  EEPROM.write(memAddress, myMode + 1);
  for (i = 0; i < strip.numPixels(); i++) {
    setPixel(myMode);
  }
  Serial.begin (9600);
  baseline = analogRead (sensorPin);
  strip.begin();
}

void loop ()
{
  delay(1);
  sensorValue = analogRead (sensorPin);
  baseline = (sensorValue + baseline) / 2;
  Serial.print(myMode);
  Serial.print(" ");
  Serial.print(sensorValue, DEC);
  Serial.print(" ");
  Serial.print(baseline);
  Serial.print(" ");
  randMode = random(0,4);
  diff = max(baseline, sensorValue) - min(baseline, sensorValue);
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  if (diff > 1) {
    if (j > 256 * 5) {//For fade mode
      j = 0;
    }
    for (i = 0; i < diff; i++) {
      Serial.print("=");
      if (myMode == 5) {
        setPixel(randMode);
      } else {
        setPixel(myMode);
      }
      strip.show();
    }
  }
  Serial.println();
  j = j + 1;
}

void setPixel(int myMode) {
  if (myMode == 0) {
    strip.setPixelColor(i - sensitivity, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
  } else if (myMode == 1) {
    strip.setPixelColor(i - sensitivity, strip.Color(100, 100, 100));
  } else if (myMode == 2) {
    strip.setPixelColor(i - sensitivity, strip.Color(255, 0, 0));
  } else if (myMode == 3) {
    strip.setPixelColor(i - sensitivity, strip.Color(0, 255, 0));
  } else if (myMode == 4) {
    strip.setPixelColor(i - sensitivity, strip.Color(0, 0, 255));
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

