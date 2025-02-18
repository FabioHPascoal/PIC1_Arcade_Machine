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

void initializeFrog{

}

beginTimeVariables(){
  lastMovementTime = milis() - movementIntervalFrogger;
}

frog_movement(){

}

void froggerSetup(){
  streetMode = true; // definicao do primeiro mapa como a rua
  drawTerrain();
  //posicionar sapo
  // posicionar carros
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