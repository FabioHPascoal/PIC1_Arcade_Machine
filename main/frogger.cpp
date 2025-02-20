#include "frogger.h"
#include "global_vars"

void drawTerrain(){
  if(streetMode){
    for(int row=0;row<32;row++){
      for(int col=0;col<8;col++){
        if(col==0||col==7)
          pixels.setPixelColor(led_map[row][col], emerald);
        else
          pixels.setPixelColor(led_map[row][col], gray);
      }
    }
  }else{
     for(int row=0;row<32;row++){
      for(int col=0;col<8;col++){
        if(col==0||col==7)
          pixels.setPixelColor(led_map[row][col], emerald);
        else
          pixels.setPixelColor(led_map[row][col], blue);
      }
    } 
  }
}

void initializeFrog(){
  frogPosition.row = (matrixNumRow/2) - 1;
  frogPosition.col = 0;
  pixels.setPixelColor(led_map[frogPosition.row][frogPosition.col], green);
}

void beginTimeVariables(){
  lastMovementTimeFrogger = milis() - movementIntervalFrogger;
}

bool verficaColisaoEsquerda(){
  if(frogPosition.row == 0)
    return true;
  return false;
}

bool verficaColisaoDireita(){
  if(frogPosition.row == matrixNumRow -1)
    return true;
  return false;
}

bool verficaColisaoInferior(){
  if(frogPosition.col == 0)
    return true;
  return false;
}

bool verficaColisaoSuperior(){
  if(frogPosition.col == matrixNumCol -1)
    return true;
  return false;
}

void moveFrog(char direction, char value){
  if(direction == 'V')
    frogPosition.col+= value;
  else if(direction == 'H')
    frogPosition.row+= value;

}

void ReposicionaFrog(){
  frogPosition.col = 0;
}

void changeGameMode(){
  streetMode = !streetMode;
}

void NewGame(){
  if(streetMode){
    // redesenhar a rua e posicionar os carros da rua
  }else{
    // redesenhar o rio e posicionar os troncos do rio
  }
}

void frog_movement(){
  leftState = !digitalRead(downStick);
  rightState = !digitalRead(upStick);
  downState = !digitalRead(rightStick);
  upState = !digitalRead(leftStick);

  if (leftState){
    if(!verficaColisaoEsquerda()){
      moveFrog('H',-1);
    }
  }

  if (rightState){
    if(!verficaColisaoDireita()){
      moveFrog('H',1);
    }
  }

  if (downState){
    if(!verficaColisaoInferior()){
      moveFrog('V',-1);
    }
  }

  if (upState){
    if(verficaColisaoSuperior()){
      ReposicionaFrog();
      changeGameMode();
      NewGame();
    }else{
      moveFrog('V',1);
    }
  }
}

void froggerSetup(){
  streetMode = true; // definicao do primeiro mapa como a rua
  drawTerrain();
  initializeFrog();
  carsPosition();
  beginTimeVariables();
}

void froggerLoop(){

  //adicionar os dois modos de jogo
  if(streetMofe){
    // jogo para o asfalto
  }else{
    // jogo para o rio
  }
  // verificar as funções abaixo para ver o melhor posicinamento delas no código
  // unsigned long currentTime = millis();

  // if (currentTime - lastMovementTimeFrogger >= movementIntervalFrogger) {
  //   frog_movement();
  //   lastMovementTimeFrogger = currentTime;
  // }

  // pixels.show();
}