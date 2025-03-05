#include "frogger.h"
#include "global_vars.h"

uint32_t getColorInColors(char indice){
  return colors[indice];
} 

void drawOnLedMap(Mode gameMode){
  char k;
  switch(gameMode){
    case STREET: k = 0; break;
    case RIVER: k = 1; break;
  }

  for(int row=0; row<32; row++){
    for(int col=0; col<8; col++){
      uint32_t color = getColorInColors(pgm_read_byte(&matrixMap[k][row][col]));
      pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), color);
    }
  }
}

void beginFrog(){
  frog = {{matrixNumRow/2, 0}, 0, 0, 80};
  frog.lastMovementTimeFrogger = millis() - frog.movementIntervalFrogger;
}

bool frogCanMove(){
  unsigned long currentTime = millis();

  if (currentTime - frog.lastMovementTimeFrogger >= frog.movementIntervalFrogger) {
    return true;
  }

  return false;
}

void moveFrog(Direction direction){
  switch(direction){
    case LEFT : frog.posAtual.row--; break;
    case RIGHT: frog.posAtual.row++; break;
    case UP:    frog.posAtual.col++; break;
    case DOWN:  frog.posAtual.col--; break;
  }

  frog.lastMovementTimeFrogger = millis();
}

bool verficaColisao(Direction direction){
  switch(direction){
    case LEFT:  if(frog.posAtual.row == 0 ) return true; break;
    case RIGHT: if(frog.posAtual.row == 31) return true; break;
    case DOWN:  if(frog.posAtual.col == 0 ) return true; break;
    case UP:    if(frog.posAtual.col == 7 ) return true; break;
  }
  return false;
}

void ReposicionaFrog(){
  frog.posAtual.col = 0;
  frog.posMaxY = 0;
}

void changeGameMode(){
  switch(gameMode){
    case STREET: gameMode = RIVER ; break;
    case RIVER : gameMode = STREET; break;
  }
}

void IncreaseScore(unsigned char value){
  score += value;
}

void frog_movement(){
  leftState  = !digitalRead(leftStick );
  rightState = !digitalRead(rightStick);
  downState  = !digitalRead(downStick );
  upState    = !digitalRead(upStick   );

  if (leftState){
    if(!verficaColisao(LEFT)){
      moveFrog(LEFT);
    }
  }

  if (rightState){
    if(!verficaColisao(RIGHT)){
      moveFrog(RIGHT);
    }
  }

  if (downState){
    if(!verficaColisao(DOWN)){
      moveFrog(DOWN);
    }
  }

  if (upState){
    if(verficaColisao(UP)){
      ReposicionaFrog();
      changeGameMode();
      IncreaseScore(200);
    }else{
      moveFrog(UP);
      if(frog.posMaxY < frog.posAtual.col){
        IncreaseScore(10);
        frog.posMaxY = frog.posAtual.col;
      }
    }
  }
}

void drawFrogOnMap(){
  pixels.setPixelColor(pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col]), emerald);
}

void froggerSetup() {
  score = 0;
  beginFrog();
  Serial.println("Frogger Setup");
}

void froggerLoop() {
  drawOnLedMap(gameMode);

  if(frogCanMove()){
    frog_movement();
    Serial.println("Frog moved");
  }

  drawFrogOnMap();

  //Serial.println("Frogger Loop");
  pixels.show();
}
