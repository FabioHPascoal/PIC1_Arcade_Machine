#ifndef GLOBALS_H
#define GLOBALS_H

//#include <Adafruit_NeoPixel.h>
//#include <Arduino.h>
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
  const uint32_t cor; //cor dos carros ou troncos da linha
  unsigned long lastMovementTime;
  unsigned char verificadorDeContinuidade;
};

enum Direction {DOWN, UP, LEFT, RIGHT};

///////////////////// GENERAL VARIABLES /////////////////////

// CONSTANTS
  const float BRIGHTNESS = 0.01;
  const int NUM_PIXELS = 256;

  // Lenth Constants
  const unsigned char matrixNumCol = 8;
  const unsigned char matrixNumRow = 32;
  
  // Pins
  const byte leftStick = 12;
  const byte rightStick = 13;
  const byte upStick = 11;  
  const byte downStick = 10;

  const byte ledMatrixPin = 6;

  // Colors
  const uint32_t red     = adjustBrightness(255, 0, 0, BRIGHTNESS);
  const uint32_t green   = adjustBrightness(0, 255, 0, BRIGHTNESS);
  const uint32_t blue    = adjustBrightness(0, 0, 255, BRIGHTNESS);
  const uint32_t white   = adjustBrightness(255, 255, 255, BRIGHTNESS);
  const uint32_t black   = adjustBrightness(0, 0, 0, BRIGHTNESS);
  const uint32_t yellow  = adjustBrightness(255, 255, 0, BRIGHTNESS);
  const uint32_t cyan    = adjustBrightness(0, 255, 255, BRIGHTNESS);
  const uint32_t magenta = adjustBrightness(255, 0, 255, BRIGHTNESS);
  const uint32_t orange  = adjustBrightness(255, 120, 0, BRIGHTNESS);
  const uint32_t gray    = adjustBrightness(36, 36, 36, BRIGHTNESS);
  const uint32_t emerald = adjustBrightness(4, 99, 7, BRIGHTNESS);
  const uint32_t brown   = adjustBrightness(150, 75, 0, BRIGHTNESS);

  // Lookup Tables

  const byte led_map[32][8] = {
    {  7,  6,  5,  4,  3,  2,  1,  0},
    {  8,  9, 10, 11, 12, 13, 14, 15},
    { 23, 22, 21, 20, 19, 18, 17, 16},
    { 24, 25, 26, 27, 28, 29, 30, 31},
    { 39, 38, 37, 36, 35, 34, 33, 32},
    { 40, 41, 42, 43, 44, 45, 46, 47},
    { 55, 54, 53, 52, 51, 50, 49, 48},
    { 56, 57, 58, 59, 60, 61, 62, 63},
    { 71, 70, 69, 68, 67, 66, 65, 64},
    { 72, 73, 74, 75, 76, 77, 78, 79},
    { 87, 86, 85, 84, 83, 82, 81, 80},
    { 88, 89, 90, 91, 92, 93, 94, 95},
    {103,102,101,100, 99, 98, 97, 96},
    {104,105,106,107,108,109,110,111},
    {119,118,117,116,115,114,113,112},
    {120,121,122,123,124,125,126,127},
    {135,134,133,132,131,130,129,128},
    {136,137,138,139,140,141,142,143},
    {151,150,149,148,147,146,145,144},
    {152,153,154,155,156,157,158,159},
    {167,166,165,164,163,162,161,160},
    {168,169,170,171,172,173,174,175},
    {183,182,181,180,179,178,177,176},
    {184,185,186,187,188,189,190,191},
    {199,198,197,196,195,194,193,192},
    {200,201,202,203,204,205,206,207},
    {215,214,213,212,211,210,209,208},
    {216,217,218,219,220,221,222,223},
    {231,230,229,228,227,226,225,224},
    {232,233,234,235,236,237,238,239},
    {247,246,245,244,243,242,241,240},
    {248,249,250,251,252,253,254,255}
  };

// GLOBAL VARIABLES
  inline Adafruit_NeoPixel pixels(NUM_PIXELS, ledMatrixPin, NEO_GRB + NEO_KHZ800);
  
  inline byte ledStates[NUM_PIXELS] = {};

  inline byte leftState = 0;
  inline byte rightState = 0;
  inline byte upState = 0;
  inline byte downState = 0;
  
  inline int score = 0;

///////////////////// TETRIS VARIABLES /////////////////////

// GLOBAL VARIABLES
  inline byte fallingPiecePixels[4][2] = {};

  inline byte tetrominoNum = 0;
  inline byte currentRotation = 0;

  inline bool falling = false;

  inline unsigned long lastDescendTime = 0;
  inline unsigned long lastMovementTime = 0;

// CONSTANTS
  const uint32_t tetrominoColors[11] = {black,yellow,cyan,blue,orange,green,red,magenta,gray,emerald,brown};

  // Time constants
  
  const unsigned long movementInterval = 20;
  const unsigned long descendInterval = 200;

  // Lookup Tables

  const byte tetrominoSpawnLocation[7][4][2] = {
    {{0, 3}, {0, 4}, {1, 3}, {1, 4}},
    {{1, 2}, {1, 3}, {1, 4}, {1, 5}},
    {{0, 2}, {1, 2}, {1, 3}, {1, 4}},
    {{0, 4}, {1, 2}, {1, 3}, {1, 4}},
    {{0, 3}, {0, 4}, {1, 2}, {1, 3}},
    {{0, 2}, {0, 3}, {1, 3}, {1, 4}},
    {{0, 3}, {1, 2}, {1, 3}, {1, 4}},
  };

  // Order: tetramino number -> rotation position -> relative increment pair -> increment value in one direction
  const char tetrominoRotations[7][4][4][2] = {
  {{{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}}},
  {{{-1,2}, {0,1}, {1,0}, {2,-1}},   {{2,1}, {1,0}, {0,-1}, {-1,-2}},   {{1,-2}, {0,-1}, {-1,0}, {-2,1}},   {{-2,-1}, {-1,0}, {0,1}, {1,2}}},
  {{{0,2}, {-1,1}, {0,0}, {1,-1}},    {{2,0}, {1,1}, {0,0}, {-1,-1}},    {{0,-2}, {1,-1}, {0,0}, {-1,1}},    {{-2,0}, {-1,-1}, {0,0}, {1,1}}},
  {{{2,0}, {-1,1}, {0,0}, {1,-1}},    {{0,-2}, {1,1}, {0,0}, {-1,-1}},    {{-2,0}, {1,-1}, {0,0}, {-1,1}},    {{0,2}, {-1,-1}, {0,0}, {1,1}}},
  {{{1,1}, {2,0}, {-1,1}, {0,0}},    {{1,-1}, {0,-2}, {1,1}, {0,0}},    {{-1,-1}, {-2,0}, {1,-1}, {0,0}},    {{-1,1}, {0,2}, {-1,-1}, {0,0}}},
  {{{0,2}, {1,1}, {0,0}, {1,-1}},    {{2,0}, {1,-1}, {0,0}, {-1,-1}},    {{0,-2}, {-1,-1}, {0,0}, {-1,1}},    {{-2,0}, {-1,1}, {0,0}, {1,1}}},
  {{{1,1}, {-1,1}, {0,0}, {1,-1}},    {{1,-1}, {1,1}, {0,0}, {-1,-1}},    {{-1,-1}, {1,-1}, {0,0}, {-1,1}},    {{-1,1}, {-1,-1}, {0,0}, {1,1}}}
  };

///////////////////// FROGGER VARIABLES /////////////////////4

// GLOBAL VARIABLES
  bool streetMode;
  struct frogger frog;
  struct faixaTroncoOuRua street[6];
  struct faixaTroncoOuRua river[6];
  uint32_t matrixMapStreet[32][8];
  uint32_t matrixMapRiver[32][8];

// CONSTANTS

#endif