#include "global_vars.h"

// Inicialização do objeto TM1637Display
TM1637Display display(ssegClk, ssegDio);

// Inicialização do objeto NeoPixel
Adafruit_NeoPixel pixels(NUM_PIXELS, ledMatrixPin, NEO_GRB + NEO_KHZ800);

// Variáveis globais
byte ledStates[NUM_PIXELS] = {};
byte leftState = 0, rightState = 0, upState = 0, downState = 0;
byte leftButtonState = 0, rightButtonState = 0;
byte currentGame = 0;
int score = 0;

// Cores (definição real das variáveis)
uint32_t red     = adjustBrightness(255, 0, 0, BRIGHTNESS);
uint32_t green   = adjustBrightness(0, 255, 0, BRIGHTNESS);
uint32_t blue    = adjustBrightness(0, 0, 255, BRIGHTNESS);
uint32_t white   = adjustBrightness(255, 255, 255, BRIGHTNESS);
uint32_t black   = adjustBrightness(0, 0, 0, BRIGHTNESS);
uint32_t yellow  = adjustBrightness(255, 255, 0, BRIGHTNESS);
uint32_t cyan    = adjustBrightness(0, 255, 255, BRIGHTNESS);
uint32_t magenta = adjustBrightness(255, 0, 255, BRIGHTNESS);
uint32_t orange  = adjustBrightness(255, 120, 0, BRIGHTNESS);

// Array com as cores criadas
const uint32_t colors[8] = {black, yellow, cyan, blue, orange, green, red, magenta};

// Lookup Tables armazenadas na Flash (PROGMEM)
const byte led_map[32][8] PROGMEM = {
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

// Tetris Variables
byte fallingPiecePixels[4][2] = {};
byte tetrominoNum = 0, currentRotation = 0;
bool falling = false;
unsigned long lastDescendTime = 0, lastMovementTime = 0, lastRotationTime = 0;

// Constantes de Tetris
const unsigned long movementInterval = 100, descendInterval = 1000, rotationInterval = 300;

// Lookup Tables armazenadas em Flash (PROGMEM)
const byte tetrominoSpawnLocation[7][4][2] PROGMEM = {
    {{0, 3}, {0, 4}, {1, 3}, {1, 4}},
    {{1, 2}, {1, 3}, {1, 4}, {1, 5}},
    {{0, 2}, {1, 2}, {1, 3}, {1, 4}},
    {{0, 4}, {1, 2}, {1, 3}, {1, 4}},
    {{0, 3}, {0, 4}, {1, 2}, {1, 3}},
    {{0, 2}, {0, 3}, {1, 3}, {1, 4}},
    {{0, 3}, {1, 2}, {1, 3}, {1, 4}},
};

// Matriz de rotações (PROGMEM)
const char tetrominoRotations[7][4][4][2] PROGMEM {
{{{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}},    {{0,0}, {0,0}, {0,0}, {0,0}}},
{{{-1,2}, {0,1}, {1,0}, {2,-1}},   {{2,1}, {1,0}, {0,-1}, {-1,-2}},   {{1,-2}, {0,-1}, {-1,0}, {-2,1}},   {{-2,-1}, {-1,0}, {0,1}, {1,2}}},
{{{0,2}, {-1,1}, {0,0}, {1,-1}},    {{2,0}, {1,1}, {0,0}, {-1,-1}},    {{0,-2}, {1,-1}, {0,0}, {-1,1}},    {{-2,0}, {-1,-1}, {0,0}, {1,1}}},
{{{2,0}, {-1,1}, {0,0}, {1,-1}},    {{0,-2}, {1,1}, {0,0}, {-1,-1}},    {{-2,0}, {1,-1}, {0,0}, {-1,1}},    {{0,2}, {-1,-1}, {0,0}, {1,1}}},
{{{1,1}, {2,0}, {-1,1}, {0,0}},    {{1,-1}, {0,-2}, {1,1}, {0,0}},    {{-1,-1}, {-2,0}, {1,-1}, {0,0}},    {{-1,1}, {0,2}, {-1,-1}, {0,0}}},
{{{0,2}, {1,1}, {0,0}, {1,-1}},    {{2,0}, {1,-1}, {0,0}, {-1,-1}},    {{0,-2}, {-1,-1}, {0,0}, {-1,1}},    {{-2,0}, {-1,1}, {0,0}, {1,1}}},
{{{1,1}, {-1,1}, {0,0}, {1,-1}},    {{1,-1}, {1,1}, {0,0}, {-1,-1}},    {{-1,-1}, {1,-1}, {0,0}, {-1,1}},    {{-1,1}, {-1,-1}, {0,0}, {1,1}}}
};
