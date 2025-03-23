#include "snake.h"
#include "global_vars.h"

void initSnake() {
    // Define a posição inicial da cobra (tamanho 1)
    byte ledIndex = pgm_read_byte(&led_map[snake.head.row][snake.head.col]);
    pixels.setPixelColor(ledIndex, green);
    ledStates[ledIndex] = 5; // 5 = corpo da cobra
}

void changeDirection(Direction newDir) {
  // Verifica se a nova direção não é oposta à direção atual
  if ((snake.headDir == UP && newDir == DOWN) ||
      (snake.headDir == DOWN && newDir == UP) ||
      (snake.headDir == LEFT && newDir == RIGHT) ||
      (snake.headDir == RIGHT && newDir == LEFT)) {
    return; // Direção inválida
  }
  
  // Registra a posição atual da cabeça como ponto de virada
  if (snake.turningPointCount < 50) {
    snake.turningPoints[snake.turningPointCount].row = snake.head.row;
    snake.turningPoints[snake.turningPointCount].col = snake.head.col;
    snake.turningPoints[snake.turningPointCount].newDir = newDir;
    snake.turningPointCount++;
  }
  
  // Atualiza imediatamente a direção da cabeça
  snake.headDir = newDir;
}

void updateTail() {
  // Verifica pontos de virada ANTES de mover a cauda
  if (snake.turningPointCount > 0) {
    if (snake.tail.row == snake.turningPoints[0].row &&
        snake.tail.col == snake.turningPoints[0].col) {
      // Atualiza a direção da cauda
      snake.tailDir = snake.turningPoints[0].newDir;
      
      // Remove o ponto de virada
      for (uint8_t i = 0; i < snake.turningPointCount - 1; i++) {
        snake.turningPoints[i] = snake.turningPoints[i + 1];
      }
      snake.turningPointCount--; // Decrementa APÓS o loop
    }
  }

  // Move a cauda após verificar os pontos
  switch (snake.tailDir) {
    case UP:    snake.tail.col += 1; break;
    case DOWN:  snake.tail.col -= 1; break;
    case LEFT:  snake.tail.row -= 1; break;
    case RIGHT: snake.tail.row += 1; break;
  }
}

void spawnApple() {
    byte randomNum;
    do {
        randomNum = random(0, 256);
    } while (ledStates[randomNum] != 0); // Só spawna em posições vazias
    
    pixels.setPixelColor(randomNum, red);
    ledStates[randomNum] = 6; // 6 = maçã
}

bool checkCollisionEdge(byte rowInc, byte colInc){
  byte row = snake.head.row + rowInc;
  byte col = snake.head.col + colInc;

  if (row < 0 || col < 0 || row >= 32 || col >= 8) {
    return true;
  }
  return false;
}

bool checkCollisionApple(byte rowInc, byte colInc){
  byte row = snake.head.row + rowInc;
  byte col = snake.head.col + colInc;

  if (ledStates[pgm_read_byte(&led_map[row][col])] == 6){
    return true;
  }
  return false;
}

bool checkSelfCollision(byte rowInc, byte colInc) {
    byte nextRow = snake.head.row + rowInc;
    byte nextCol = snake.head.col + colInc;
    
    // Verifica se a próxima posição está dentro do corpo
    for (uint8_t i = 0; i < snake.turningPointCount; i++) {
        if (nextRow == snake.turningPoints[i].row && nextCol == snake.turningPoints[i].col) {
            return true;
        }
    }
    return false;
}


void moveSnake(Direction direction, bool apple) {
    byte rowInc = 0, colInc = 0;

    // Direção da cabeça
    switch (direction) {
        case UP:    colInc = 1;  break;
        case DOWN:  colInc = -1; break;
        case LEFT:  rowInc = -1; break;
        case RIGHT: rowInc = 1;  break;
    }

    // Atualiza posição da CABEÇA
    snake.head.row += rowInc;
    snake.head.col += colInc;

    if (!apple) {
        // Apaga a CAUDA ANTIGA (apenas se não comeu maçã)
        byte tailLed = pgm_read_byte(&led_map[snake.tail.row][snake.tail.col]);
        pixels.setPixelColor(tailLed, black);
        ledStates[tailLed] = 0;

        // Move a CAUDA
        updateTail();
    } else {
        score += 10;
        spawnApple(); // Gera nova maçã
    }

    // Desenha a NOVA CABEÇA
    byte headLed = pgm_read_byte(&led_map[snake.head.row][snake.head.col]);
    pixels.setPixelColor(headLed, green);
    ledStates[headLed] = 5;
}

void resetGame() {
    // Resetar todas as variáveis do jogo
    snake.head = {18, 4};
    snake.tail = {18, 4};
    snake.headDir = RIGHT;
    snake.tailDir = RIGHT;
    snake.turningPointCount = 0;
    
    score = 0;
    memset(ledStates, 0, sizeof(ledStates));
    pixels.clear();
    
    // Nova maçã
    spawnApple();
    restarting = false;
}

void gameOver() {
    // Piscar LEDs vermelhos
    for (int i = 0; i < 3; i++) {
        pixels.fill(red);
        pixels.show();
        delay(150);
        pixels.clear();
        pixels.show();
        delay(150);
    }

    // Flag de reinício
    restarting = true;
    gameOverTime = millis();
}

void playerMovement(){
    // Leitura suavizada dos controles
    upState = !digitalRead(upStick);
    downState = !digitalRead(downStick);
    leftState = !digitalRead(leftStick);
    rightState = !digitalRead(rightStick);

    Direction newDir = snake.headDir;
    
    // Prioridade de direções com buffer de entrada
    if(upState && snake.headDir != DOWN) newDir = UP;
    else if(downState && snake.headDir != UP) newDir = DOWN;
    else if(leftState && snake.headDir != RIGHT) newDir = LEFT;
    else if(rightState && snake.headDir != LEFT) newDir = RIGHT;

    // Verificação de colisão antecipada
    byte rowInc = 0, colInc = 0;
    switch(newDir) {
        case UP: colInc = 1; break;
        case DOWN: colInc = -1; break;
        case LEFT: rowInc = -1; break;
        case RIGHT: rowInc = 1; break;
    }

    if(checkCollisionEdge(rowInc, colInc) || checkSelfCollision(rowInc, colInc)) {
        gameOver();
        return;
    }

    bool appleEaten = checkCollisionApple(rowInc, colInc);
    changeDirection(newDir);
    moveSnake(newDir, appleEaten);
}

// Função de configuração inicial do jogo
void snakeSetup() {
    initSnake();
    spawnApple();
}

void snakeLoop() {
    if (restarting) {
        if (millis() - gameOverTime > 2000) { // Reinicia após 2 segundos
            resetGame();
        }
        return;
    }

    // Movimento suave e responsivo
    unsigned long currentTime = millis();
    if (currentTime - lastMovementTime >= snakeMovementInterval) {
        playerMovement();
        lastMovementTime = currentTime;
    }
    pixels.show();
}
