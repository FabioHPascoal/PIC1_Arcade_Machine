#include "tetris.h"
#include "global_vars.h"

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

void clear_row(int row) {
  for (int col = 0; col < 8; col++) {
    pixels.setPixelColor(led_map[row][col], tetrominoColors[0]);
    ledStates[led_map[row][col]] = tetrominoColors[0];
  }
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

void player_movement() {
  leftState = !digitalRead(downStick);
  rightState = !digitalRead(upStick);
  downState = !digitalRead(rightStick);
  upState = !digitalRead(leftStick);

  // leftState = 0;
  // rightState = 0;
  // downState = 0;
  // upState = 0;
  
  if (downState) descend_tetromino();
  else if (leftState && !check_collision(0, -1)) move_tetromino(LEFT);
  else if (rightState && !check_collision(0, 1)) move_tetromino(RIGHT);
  // if (upState && !check_rotation_collision(RIGHT)) rotate_tetromino(RIGHT); // Bug here
}

void tetrisLoop() {
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
