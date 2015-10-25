#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor
long baseline = 0;
long diff = 0;
int i = 0;
int j = 0;

void setup () 
{
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  //pinMode (ledPin, OUTPUT);
  Serial.begin (9600);
  baseline = analogRead (sensorPin);
  strip.begin();
  for (j=0; j<100; j++) {
    for (i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        strip.show();
    }
    delay(10);
  }
}
 
void loop () 
{
  delay(1);
  sensorValue = analogRead (sensorPin);
  baseline = (sensorValue+baseline)/2;
  Serial.print(sensorValue, DEC);
  Serial.print(" ");
  Serial.print(baseline);
  Serial.print(" ");
  diff = max(baseline, sensorValue) - min(baseline, sensorValue);
  for(i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  if (diff > 1) {
    //if(diff > 2) { diff = diff / (diff/2); }
    //diff = diff * 2;
    if (j > 256*5) { j = 0; }
    for (i = 0; i < diff; i++) {
      Serial.print("=");
      strip.setPixelColor(i-10, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      strip.show();
    }
  }
  Serial.println();
  j = j + 1;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

