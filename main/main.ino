#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 256

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

enum Direction {DOWN, UP, LEFT, RIGHT};

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

// Define uma função para ajustar o brilho
uint32_t adjustBrightness(uint8_t r, uint8_t g, uint8_t b, float brightness) {
  // Multiplica cada componente pelo fator de brilho
  r = r * brightness;
  g = g * brightness;
  b = b * brightness;
  return pixels.Color(r, g, b);
}

// Define o brilho desejado (0.0 a 1.0)
const float brightness = 0.01;

// Define as cores ajustadas pelo brilho
const uint32_t red     = adjustBrightness(255, 0, 0, brightness);
const uint32_t green   = adjustBrightness(0, 255, 0, brightness);
const uint32_t blue    = adjustBrightness(0, 0, 255, brightness);
const uint32_t white   = adjustBrightness(255, 255, 255, brightness);
const uint32_t black   = adjustBrightness(0, 0, 0, brightness);
const uint32_t yellow  = adjustBrightness(255, 255, 0, brightness);
const uint32_t cyan    = adjustBrightness(0, 255, 255, brightness);
const uint32_t magenta = adjustBrightness(255, 0, 255, brightness);
const uint32_t orange  = adjustBrightness(255, 120, 0, brightness);

const byte leftStick = 13;
const byte rightStick = 12;
const byte upStick = 10;  
const byte downStick = 11;

byte leftState;
byte rightState;
byte upState;
byte downState;

byte ledStates[NUMPIXELS] = {};

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));
  
  pinMode(leftStick, INPUT_PULLUP);
  pinMode(rightStick, INPUT_PULLUP);
  pinMode(upStick, INPUT_PULLUP);
  pinMode(downStick, INPUT_PULLUP);

  pixels.begin();
  pixels.show();
}

unsigned long lastDescendTime = 0;
unsigned long lastMovementTime = 0;
const unsigned long movementInterval = 20;
const unsigned long descendInterval = 200;

bool falling = false;
byte fallingPiecePixels[4][2];
byte tetrominoNum;
byte currentRotation;

const byte tetrominoSpawnLocation[7][4][2] = {
  { {0, 3}, {0, 4}, {1, 3}, {1, 4}},
  { {1, 2}, {1, 3}, {1, 4}, {1, 5}},
  { {0, 2}, {1, 2}, {1, 3}, {1, 4}},
  { {0, 4}, {1, 2}, {1, 3}, {1, 4}},
  { {0, 3}, {0, 4}, {1, 2}, {1, 3}},
  { {0, 2}, {0, 3}, {1, 3}, {1, 4}},
  { {0, 3}, {1, 2}, {1, 3}, {1, 4}},
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

const uint32_t tetrominoColors[8] = {black,yellow,cyan,blue,orange,green,red,magenta};

void loop() {
  if (!falling) new_tetromino();

  unsigned long currentTime = millis();

  if (currentTime - lastMovementTime >= movementInterval) {
    player_movement();
    lastMovementTime = currentTime;
  }

  if (currentTime - lastDescendTime >= descendInterval) {
    descend_tetromino();
    lastDescendTime = currentTime;
  }

  pixels.show();
}

bool check_collision(int rowInc, int colInc) {
  for (byte pixel = 0; pixel < 4; pixel++) {
    int row = fallingPiecePixels[pixel][0] + rowInc;
    int col = fallingPiecePixels[pixel][1] + colInc;

    if (row < 0 || col < 0 || row >= 32 || col >= 8 || ledStates[led_map[row][col]] != 0) {
      return true;
    }
  }
  return false;
}

void move_tetromino(Direction direction) {
  byte rowInc = 0, colInc = 0;

  switch (direction) {
    case DOWN: rowInc = 1; break;
    case LEFT: colInc = -1; break;
    case RIGHT: colInc = 1; break;
  }

  // Erase previous tetromino
  for (int i = 0; i < 4; i++) {
    byte row = fallingPiecePixels[i][0];
    byte col = fallingPiecePixels[i][1];

    pixels.setPixelColor(led_map[row][col], black);
  }

  // Draw tetramino on new position
  for (int i = 0; i < 4; i++) {
    byte row = fallingPiecePixels[i][0] + rowInc;
    byte col = fallingPiecePixels[i][1] + colInc;
    
    pixels.setPixelColor(led_map[row][col], tetrominoColors[tetrominoNum]);

    fallingPiecePixels[i][0] = row;
    fallingPiecePixels[i][1] = col;
  }
}

void descend_tetromino() {
  if (!check_collision(1, 0)) {
    move_tetromino(DOWN);
  }

  else {
    int highRow = -1;
    int lowRow = 32;
    
    for (byte i = 0; i < 4; i++) {
      byte row = fallingPiecePixels[i][0];
      byte col = fallingPiecePixels[i][1];

      ledStates[led_map[row][col]] = tetrominoNum;

      if (row > highRow) highRow = row;
      if (row < lowRow) lowRow = row;
    }

    process_completed_lines(lowRow, highRow);

    falling = false;
  }
}

void new_tetromino() {
  tetrominoNum = random(1, 8);
  // tetrominoNum = 1;
  for (byte i = 0; i < 4; i++) {
    byte row = tetrominoSpawnLocation[tetrominoNum - 1][i][0];
    byte col = tetrominoSpawnLocation[tetrominoNum - 1][i][1];

    fallingPiecePixels[i][0] = row;
    fallingPiecePixels[i][1] = col;

    pixels.setPixelColor(led_map[row][col], tetrominoColors[tetrominoNum]);
  }
  falling = true;
  currentRotation = 0;
}

void player_movement() {
  leftState = !digitalRead(leftStick);
  rightState = !digitalRead(rightStick);
  downState = !digitalRead(downStick);
  upState = !digitalRead(upStick);
  
  if (downState) descend_tetromino();
  else if (leftState && !check_collision(0, -1)) move_tetromino(LEFT);
  else if (rightState && !check_collision(0, 1)) move_tetromino(RIGHT);

  if (upState && !check_rotation_collision(RIGHT)) rotate_tetromino(RIGHT);
}

void process_completed_lines(byte lowRow, byte highRow) {
  
  byte completeRowsCount = 0;
  int completeRows[4] = {};
  
  // Check for completed rows
  for (int row = lowRow; row <= highRow; row++) {
    for (int col = 0; col < 8; col++) {
      if (ledStates[led_map[row][col]] == 0) break;
      
      if (col == 7) {
        completeRows[completeRowsCount] = row;
        completeRowsCount++;
      }
    }
  }

  for (byte row = 0; row < completeRowsCount; row++) {
    clear_row(completeRows[row]);
    pixels.show();
  }

  if (completeRowsCount > 0) {
    delay(500);
    byte rowShift = 1;
    for (byte row = completeRows[completeRowsCount - 1] - 1; row > 2; row--) {
      if (row == completeRows[completeRowsCount - rowShift - 1] && rowShift < completeRowsCount) {
        rowShift++;
        continue;
      }

      for (int col = 0; col < 8; col++) {
        uint32_t color = tetrominoColors[ledStates[led_map[row][col]]];
        pixels.setPixelColor(led_map[row + rowShift][col], color);
        ledStates[led_map[row + rowShift][col]] = ledStates[led_map[row][col]];
      }

      clear_row(row);
    }
  }
}

void clear_row(int row) {
  for (int col = 0; col < 8; col++) {
    pixels.setPixelColor(led_map[row][col], tetrominoColors[0]);
    ledStates[led_map[row][col]] = tetrominoColors[0];
  }
}

bool check_rotation_collision(Direction direction) {
  for (byte pixel = 0; pixel < 4; pixel++) {
    int row = fallingPiecePixels[pixel][0] + tetrominoRotations[tetrominoNum-1][currentRotation][pixel][0];
    int col = fallingPiecePixels[pixel][1] + tetrominoRotations[tetrominoNum-1][currentRotation][pixel][1];

    if (row < 0 || col < 0 || row >= 32 || col >= 8 || ledStates[led_map[row][col]] != 0) {
      return true;
    }
  }

  return false;
} 

void rotate_tetromino(Direction direction) {

  for (byte pixel = 0; pixel < 4; pixel++) {
    byte row = fallingPiecePixels[pixel][0];
    byte col = fallingPiecePixels[pixel][1];

    pixels.setPixelColor(led_map[row][col], tetrominoColors[0]);
  }

  for (byte pixel = 0; pixel < 4; pixel++) {
    byte row = fallingPiecePixels[pixel][0];
    byte col = fallingPiecePixels[pixel][1];

    byte nextRow = row + tetrominoRotations[tetrominoNum-1][currentRotation][pixel][0];
    byte nextCol = col + tetrominoRotations[tetrominoNum-1][currentRotation][pixel][1];

    pixels.setPixelColor(led_map[nextRow][nextCol], tetrominoColors[tetrominoNum]);

    fallingPiecePixels[pixel][0] = nextRow;
    fallingPiecePixels[pixel][1] = nextCol;
  }
  currentRotation = (currentRotation + 1) % 4;
}
