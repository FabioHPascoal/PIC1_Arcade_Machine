#include <avr/wdt.h>

#include "aux_functions.h"
#include "global_vars.h"
#include "tetris.h"
#include "frogger.h"
#include "snake.h"

void resetArduino() {
  wdt_enable(WDTO_15MS); // Ativa o watchdog com tempo de 15ms para resetar o Arduino
  while (1); // Entra em loop infinito até o watchdog reiniciar o sistema
}

byte gameSetup() {
  bool stateP1 = !digitalRead(switch1Pin);
  bool stateP2 = !digitalRead(switch2Pin);

  if (stateP1) {
    currentGame = 3;
    froggerSetup();
  }

  else if (stateP2) {
    currentGame = 2;
    snakeSetup();
  }

  else {
    currentGame = 1;
    tetrisSetup();
  }
}

void startSystem() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  pinMode(leftStick, INPUT_PULLUP);
  pinMode(rightStick, INPUT_PULLUP);
  pinMode(upStick, INPUT_PULLUP);
  pinMode(downStick, INPUT_PULLUP);

  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);

  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(switch1Pin), resetArduino, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switch2Pin), resetArduino, CHANGE);

  gameSetup();

  display.setBrightness(7);
  display.clear();
  display.showNumberDec(score, false);
  
  pixels.begin();
  pixels.show();

  delay(100);
}

// Controls the brightness on the led matrix
uint32_t adjustBrightness(uint8_t r, uint8_t g, uint8_t b, float brightness) {
  r = r * brightness;
  g = g * brightness;
  b = b * brightness;
  return pixels.Color(r, g, b);
}