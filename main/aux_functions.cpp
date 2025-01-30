#include "aux_functions.h"
#include "global_vars.h"

void startSystem() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  pinMode(leftStick, INPUT_PULLUP);
  pinMode(rightStick, INPUT_PULLUP);
  pinMode(upStick, INPUT_PULLUP);
  pinMode(downStick, INPUT_PULLUP);

  pixels.begin();
  pixels.show();
}

// Controls the brightness on the led matrix
uint32_t adjustBrightness(uint8_t r, uint8_t g, uint8_t b, float brightness) {
  r = r * brightness;
  g = g * brightness;
  b = b * brightness;
  return pixels.Color(r, g, b);
}