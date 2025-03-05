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

// Lenth Constants
constexpr unsigned char matrixNumCol = 8;
constexpr unsigned char matrixNumRow = 32;

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
extern uint32_t red, green, blue, white, black, yellow, cyan, magenta, orange, gray, emerald, brown;
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

///////////////////// FROGGER VARIABLES /////////////////////

enum Mode {STREET, RIVER};

typedef struct {
  unsigned char row;
  unsigned char col;
}tPosition;

typedef struct{
  tPosition posAtual;
  unsigned char posMaxY;
  unsigned long lastMovementTimeFrogger;
  unsigned char movementIntervalFrogger;
}tFrog;

typedef struct {
  Mode type; //rua ou rio
  Direction direction; //right or left
  unsigned char tamElements; //tamanho dos veiculos ou troncos na linha
  unsigned char space; //espaco entre os carros ou troncos da linha
  unsigned short speed; //velocidade dos carros ou troncos em leds por segundo
  uint32_t color; //cor dos carros ou troncos da linha
  unsigned long lastMovementTime; // Este campo pode não ser constante
  unsigned char verificadorDeContinuidade; // Este campo pode não ser constante
}tFaixa;

extern Mode gameMode;
extern tFrog frog;
extern tFaixa street[6];
extern tFaixa river[6];
extern const uint32_t matrixMap[2][32][8] PROGMEM;
extern bool newGame;

#endif
