#include "global_vars.h"
#include "aux_functions.h"
#include "tetris.h"
#include "snake.h"
#include "frogger.h"

void setup() {
  startSystem();
}

void loop() {
  switch (currentGame) {
    case 1:
      tetrisLoop();
      break;
    case 2:
      snakeLoop();
      break;
    case 3:
      froggerLoop();
      break;
  }
  display.showNumberDec(score, false);
}