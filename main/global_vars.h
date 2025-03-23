#ifndef GLOBALS_H
#define GLOBALS_H

#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "aux_functions.h"

struct Position{
  unsigned char row;
  unsigned char col;
};

struct frogger{
  struct Position posAtual;
  unsigned char posMaxY = 0;
  unsigned long lastMovementTimeFrogger;
  unsigned char movementIntervalFrogger = 40;
};

struct faixaTroncoOuRua{
  bool type; //rua ou rio
  bool direction; //false = esquerda e true = direita
  unsigned char tamElements; //tamanho dos veiculos ou troncos na linha
  unsigned char space; //espaco entre os carros ou troncos da linha
  unsigned short speed; //velocidade dos carros ou troncos em leds por segundo
  uint32_t cor; //cor dos carros ou troncos da linha
  unsigned long lastMovementTime;
  unsigned char verificadorDeContinuidade = 0;
};

enum Direction {DOWN, UP, LEFT, RIGHT};

///////////////////// GENERAL VARIABLES /////////////////////

// CONSTANTS
constexpr float BRIGHTNESS = 0.1;
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
extern const uint32_t colors[13];

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
  unsigned long lastMovementTime;
  unsigned char movementInterval;
}tFrog;

typedef struct {
  Mode type; //rua ou rio
  Direction direction; //right or left
  unsigned char tamElements; //tamanho dos veiculos ou troncos na linha
  unsigned char space; //espaco entre os carros ou troncos da linha
  unsigned short movementInterval; //velocidade dos carros ou troncos em leds por segundo
  char colorIndice; //indice da cor dos carros ou troncos da linha
  unsigned long lastMovementTime; // Este campo pode não ser constante
  unsigned char qtdElementosAAdicionar; // Este campo pode não ser constante
  unsigned char qtdEspacosAAdicionar;
}tFaixa;

extern Mode gameMode;
extern tFrog frog;
extern tFaixa street[6];
extern tFaixa river[6];
extern bool newGame;

///////////////////// SNAKE VARIABLES /////////////////////

// GLOBAL VARIABLES

extern uint8_t turningPointCount;

struct TurningPoint {
  uint8_t row;       // Linha onde a mudança ocorreu
  uint8_t col;       // Coluna onde a mudança ocorreu
  Direction newDir;  // Nova direção que a cabeça passou a ter
};

typedef struct
{
  byte row;
  byte col;
}Position;

typedef struct
{
  Position head;
  Position tail;
  Direction headDir;
  Direction tailDir;
  TurningPoint turningPoints[50];
  uint8_t turningPointCount;
}Snake;


extern Snake snake;

//CONSTANTS 
extern const unsigned long snakeMovementInterval;
extern unsigned long lastAutoTime;
extern bool restarting;
extern unsigned long gameOverTime;

#endif
