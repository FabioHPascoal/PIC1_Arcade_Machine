#include "global_vars.h"
#include "aux_functions.h"
#include "tetris.h"
#include "snake.h"
#include "frogger.h"

const int currentGame = 3; // Provisório

void setup() {
  startSystem();
  froggerSetup();
}

void loop() {
  // int currentGame = selectGame();
  switch (currentGame) {
    // case 1:
    //   tetrisLoop();
    // case 2:
    //   snakeLoop();
    case 3:
      froggerLoop();
  }
}
