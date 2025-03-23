#include "tetris.h"
#include "global_vars.h"
#include "aux_functions.h"

bool check_collision(int rowInc, int colInc) {
  for (byte pixel = 0; pixel < 4; pixel++) {
    int row = fallingPiecePixels[pixel][0] + rowInc;
    int col = fallingPiecePixels[pixel][1] + colInc;

    if (row < 0 || col < 0 || row >= 32 || col >= 8 || ledStates[pgm_read_byte(&led_map[row][col])] != 0) {
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

    pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), black);
  }

  // Draw tetramino on new position
  for (int i = 0; i < 4; i++) {
    byte row = fallingPiecePixels[i][0] + rowInc;
    byte col = fallingPiecePixels[i][1] + colInc;
    
    pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[tetrominoNum]);

    fallingPiecePixels[i][0] = row;
    fallingPiecePixels[i][1] = col;
  }
}

void clear_row(int row) {
  for (int col = 0; col < 8; col++) {
    pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[0]);
    ledStates[pgm_read_byte(&led_map[row][col])] = colors[0];
  }
}

void process_completed_lines(byte lowRow, byte highRow) {
  
  byte completeRowsCount = 0;
  int completeRows[4] = {};
  
  // Check for completed rows
  for (int row = lowRow; row <= highRow; row++) {
    for (int col = 0; col < 8; col++) {
      if (ledStates[pgm_read_byte(&led_map[row][col])] == 0) break;
      
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

    score += completeRowsCount * 8;

    byte rowShift = 1;
    for (byte row = completeRows[completeRowsCount - 1] - 1; row > 2; row--) {
      if (row == completeRows[completeRowsCount - rowShift - 1] && rowShift < completeRowsCount) {
        rowShift++;
        continue;
      }

      for (int col = 0; col < 8; col++) {
        uint32_t color = colors[ledStates[pgm_read_byte(&led_map[row][col])]];
        pixels.setPixelColor(pgm_read_byte(&led_map[row + rowShift][col]), color);
        ledStates[pgm_read_byte(&led_map[row + rowShift][col])] = ledStates[pgm_read_byte(&led_map[row][col])];
      }

      clear_row(row);
    }
  }
}

void gameOverTetris() {
  // Piscar LEDs vermelhos
  for (int i = 0; i < 3; i++) {
    pixels.fill(red);
    pixels.show();
    delay(150);
    pixels.clear();
    pixels.show();
    delay(150);
  }

  resetArduino();
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

      ledStates[pgm_read_byte(&led_map[row][col])] = tetrominoNum;

      if (row > highRow) highRow = row;
      if (row < lowRow) lowRow = row;
    }

    if (lowRow == 1) {
      gameOverTetris();
    }

    process_completed_lines(lowRow, highRow);

    falling = false;
  }
}

void new_tetromino() {

  tetrominoNum = random(1, 8);
  
  for (byte i = 0; i < 4; i++) {
    byte row = pgm_read_byte(&tetrominoSpawnLocation[tetrominoNum - 1][i][0]);
    byte col = pgm_read_byte(&tetrominoSpawnLocation[tetrominoNum - 1][i][1]);

    fallingPiecePixels[i][0] = row;
    fallingPiecePixels[i][1] = col;

    pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[tetrominoNum]);
  }
  falling = true;
  currentRotation = 0;
}

bool check_rotation_collision(Direction dir) {
  int currentRotationInc; // -1 if left, 0 if right
  int direction; // -1 if left, +1 if right
  
  switch (dir) {
    case LEFT: 
      currentRotationInc = -1;
      direction = -1;
      break;
    
    case RIGHT: 
      currentRotationInc = 0;
      direction = 1;
      break;
  }

  for (byte pixel = 0; pixel < 4; pixel++) {
    byte row = fallingPiecePixels[pixel][0];
    byte col = fallingPiecePixels[pixel][1];

    char rowInc = (char)pgm_read_byte(&tetrominoRotations[tetrominoNum-1][(currentRotation + currentRotationInc +4) %4][pixel][0]);
    char colInc = (char)pgm_read_byte(&tetrominoRotations[tetrominoNum-1][(currentRotation + currentRotationInc +4) %4][pixel][1]);

    byte nextRow = row + (direction)*rowInc;
    byte nextCol = col + (direction)*colInc;

    if (nextRow < 0 || nextCol < 0 || nextRow >= 32 || nextCol >= 8 || ledStates[pgm_read_byte(&led_map[row][col])] != 0) {
      return true;
    }
  }

  return false;
}

void rotate_tetromino(Direction dir) {
  int currentRotationInc; // -1 if left, 0 if right
  int direction; // -1 if left, +1 if right
  
  switch (dir) {
    case LEFT: 
      currentRotationInc = -1;
      direction = -1;
      break;
    
    case RIGHT: 
      currentRotationInc = 0;
      direction = 1;
      break;
  }
  
  for (byte pixel = 0; pixel < 4; pixel++) {
    byte row = fallingPiecePixels[pixel][0];
    byte col = fallingPiecePixels[pixel][1];

    pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[0]);
  }

  for (byte pixel = 0; pixel < 4; pixel++) {
    byte row = fallingPiecePixels[pixel][0];
    byte col = fallingPiecePixels[pixel][1];

    char rowInc = (char)pgm_read_byte(&tetrominoRotations[tetrominoNum-1][(currentRotation + currentRotationInc +4) %4][pixel][0]);
    char colInc = (char)pgm_read_byte(&tetrominoRotations[tetrominoNum-1][(currentRotation + currentRotationInc +4) %4][pixel][1]);

    byte nextRow = row + (direction)*rowInc;
    byte nextCol = col + (direction)*colInc;

    pixels.setPixelColor(pgm_read_byte(&led_map[nextRow][nextCol]), colors[tetrominoNum]);

    fallingPiecePixels[pixel][0] = nextRow;
    fallingPiecePixels[pixel][1] = nextCol;
  }

  currentRotation = (currentRotation + 4 + direction) % 4;
}

void player_movement() {
  leftState = !digitalRead(leftStick);
  rightState = !digitalRead(rightStick);
  downState = !digitalRead(downStick);
  upState = !digitalRead(upStick);

  if (downState) descend_tetromino();
  else if (leftState && !check_collision(0, -1)) move_tetromino(LEFT);
  else if (rightState && !check_collision(0, 1)) move_tetromino(RIGHT);
}

void player_rotation() {
  leftState = !digitalRead(leftButton);
  rightState = !digitalRead(rightButton);

  if (leftState && !check_rotation_collision(LEFT)) rotate_tetromino(LEFT);
  else if (rightState && !check_rotation_collision(RIGHT)) rotate_tetromino(RIGHT);
}

void tetrisLoop() {
  if (!falling) new_tetromino();

  unsigned long currentTime = millis();

  if (currentTime - lastMovementTime >= movementInterval) {
    player_movement();
    lastMovementTime = currentTime;
  }

  if (currentTime - lastRotationTime >= rotationInterval) {
    player_rotation();
    lastRotationTime = currentTime;
  }

  if (currentTime - lastDescendTime >= descendInterval) {
    descend_tetromino();
    lastDescendTime = currentTime;
  }
  
  pixels.show();
}

void tetrisSetup() {
  Serial.println("Tetris Setup");
}