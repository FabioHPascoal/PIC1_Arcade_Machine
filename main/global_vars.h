#ifndef GLOBALS_H
#define GLOBALS_H

#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "aux_functions.h"

enum Direction {DOWN, UP, LEFT, RIGHT};

///////////////////// GENERAL VARIABLES /////////////////////

// CONSTANTS
constexpr float BRIGHTNESS = 0.05;
constexpr int NUM_PIXELS = 256;

// Pins
constexpr uint8_t leftStick = 12;
constexpr uint8_t rightStick = 13;
constexpr uint8_t upStick = 11;
constexpr uint8_t downStick = 10;
constexpr uint8_t ledMatrixPin = 9;
constexpr uint8_t switch1Pin = 2;
constexpr uint8_t switch2Pin = 3;
constexpr uint8_t leftButton = 6;
constexpr uint8_t rightButton = 7;
constexpr uint8_t ssegDio = 5;
constexpr uint8_t ssegClk = 4;

// Colors (declaradas como `extern`, definidas no `.cpp`)
extern uint32_t red, green, blue, white, black, yellow, cyan, magenta, orange;
extern const uint32_t colors[8];

// Lookup Table (armazenada em PROGMEM)
extern const byte led_map[32][8] PROGMEM;

// GLOBAL VARIABLES (agora apenas declarações)
extern TM1637Display display;
extern Adafruit_NeoPixel pixels;
extern byte ledStates[NUM_PIXELS];

extern byte leftState, rightState, upState, downState;
extern byte leftButtonState, rightButtonState;
extern byte currentGame;
extern int score;

///////////////////// TETRIS VARIABLES /////////////////////

// GLOBAL VARIABLES
extern byte fallingPiecePixels[4][2];
extern byte tetrominoNum, currentRotation;
extern bool falling;
extern unsigned long lastDescendTime, lastMovementTime, lastRotationTime;

// CONSTANTS (agora armazenadas em PROGMEM)
extern const unsigned long movementInterval, descendInterval, rotationInterval;
extern const byte tetrominoSpawnLocation[7][4][2] PROGMEM;
extern const char tetrominoRotations[7][4][4][2] PROGMEM;

#endif
