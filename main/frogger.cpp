#include "frogger.h"
#include "global_vars.h"

void defineMap(){
  
  for(char row=0; row<32; row++){
    for(char col=0; col<8; col++){
      if(col==0||col==7){
        ledStates[pgm_read_byte(&led_map[row][col])] = 5; //verificar porque não é necessário ler a ledMAP sem precisar usar pgm_read_byte 
        pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[5]);
      }
    }
  }
  
}

char defineIndice(Direction direction){
  if(direction==RIGHT)
    return 0;
  else
    return 31;
}

char defineLimit(Direction direction){
  if(direction==RIGHT)
    return 32;
  else
    return -1;
}

char defineIncremento(Direction direction){
  if(direction==RIGHT)
    return 1;
  else
    return -1;
}

void initializeArrays(){
  tFaixa *faixa;
  char indiceCor;
  if(gameMode==STREET){
    faixa = street;
    indiceCor = 9;
  }else if(gameMode==RIVER){
    faixa = river;
    indiceCor = 3;
  }

  for(char line=0; line<6; line++){ //colunas do mapa vulfo linhas da rua ou do rio
    char espacamentoAcolocar = faixa[line].space;
    char tamElementosAColocar = 0;
    for(char rowMap=defineIndice(faixa[line].direction); rowMap!=defineLimit(faixa[line].direction); rowMap+=defineIncremento(faixa[line].direction)){
      if(tamElementosAColocar>0){
        ledStates[pgm_read_byte(&led_map[rowMap][line+1])] = faixa[line].colorIndice;
        pixels.setPixelColor(pgm_read_byte(&led_map[rowMap][line+1]), colors[faixa[line].colorIndice]);
        tamElementosAColocar--;
        if(tamElementosAColocar==0)
          espacamentoAcolocar = faixa[line].space;
      }else if(espacamentoAcolocar>0){
        ledStates[pgm_read_byte(&led_map[rowMap][line+1])] = indiceCor;
        pixels.setPixelColor(pgm_read_byte(&led_map[rowMap][line+1]), colors[indiceCor]);
        espacamentoAcolocar--;
        if(espacamentoAcolocar==0)
          tamElementosAColocar = faixa[line].tamElements;
      }
    }
  }
}

void naoSeiQualNomeColoco(){ //função que define que inicialmente os carros ou troncos vao ter que ser gerados nas extremidades do mapa
  char line;
  for(char i=0; i<2; i++){
    if(i==0){
      for(line=0; line<6; line++){
        street[line].qtdElementosAAdicionar = street[line].tamElements;
      }
    }else{
      for(line=0; line<6; line++){
        river[line].qtdElementosAAdicionar = river[line].tamElements;
      }
    }
  }
}

void naoSeiQualNomeColoco2(){
  char line;
  for(char i=0; i<2; i++){
    if(i==0){
      for(line=0; line<6; line++){
        street[line].qtdEspacosAAdicionar = 0;
      }
    }else{
      for(line=0; line<6; line++){
        river[line].qtdEspacosAAdicionar = 0;
      }
    }
  }
}

uint32_t getColorInColors(char indice){
  return colors[indice];
} 

// void drawOnPixels(Mode gameMode){
//   for(int row=0; row<32; row++){
//     for(int col=0; col<8; col++){
//       pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[ledStates[pgm_read_byte(&led_map[row][col])]]);
//     }
//   }
// }

void beginFrog(){
  frog = {{matrixNumRow/2, 0}, 0, 0, 80};
  frog.lastMovementTime = millis() - frog.movementInterval;
}

bool frogCanMove(){
  unsigned long currentTime = millis();

  if (currentTime - frog.lastMovementTime >= frog.movementInterval) {
    return true;
  }

  return false;
}

void moveFrog(Direction direction){
  pixels.setPixelColor(pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col]), colors[ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]]);
  switch(direction){
    case LEFT : frog.posAtual.row--; break;
    case RIGHT: frog.posAtual.row++; break;
    case UP:    frog.posAtual.col++; break;
    case DOWN:  frog.posAtual.col--; break;
  }

  frog.lastMovementTime = millis();
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
  initializeArrays();
  naoSeiQualNomeColoco();
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

void frogDeath(){
  for(char row=frog.posAtual.row-1; row<=frog.posAtual.row+1; row++){
    for(char col=frog.posAtual.col-1; col<=frog.posAtual.col+1; col++){
      if(row!=-1&&row!=32&&col!=-1&&col!=8){
        ledStates[pgm_read_byte(&led_map[row][col])] = 6;
        pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), red);
      }
    }
  }
  pixels.show();
  delay(4000);
  froggerSetup();
}

bool faixaCanMove(tFaixa faixa){
  if(millis()-faixa.lastMovementTime >= faixa.movementInterval){
    return true;
  }
  return false;
}

void moveElements(){
  tFaixa *faixa;
  char indiceColor;
  switch(gameMode){
    case STREET: faixa = street; indiceColor = 9; break;
    case RIVER:  faixa = river ; indiceColor = 3; break;
  }

  for(char col=0; col<6; col++){
    if(faixaCanMove(faixa[col])){
      faixa[col].lastMovementTime = millis();
      if(faixa[col].direction==RIGHT){
        for(char row=31; row>=0; row--){
          if(row!=0){
            if(gameMode==RIVER){
              if(((frog.posAtual.col==col+1)&&(frog.posAtual.row==row))&&(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]==faixa[col].colorIndice)){
                if(frog.posAtual.row==31)
                  frogDeath();
                else
                  frog.posAtual.row++;
              }
            }
            ledStates[pgm_read_byte(&led_map[row][col+1])] = ledStates[pgm_read_byte(&led_map[row - 1][col+1])];
            pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[ledStates[pgm_read_byte(&led_map[row - 1][col+1])]]);
          }else{
            if(faixa[col].qtdElementosAAdicionar>0){
              ledStates[pgm_read_byte(&led_map[row][col+1])] = faixa[col].colorIndice;
              pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[faixa[col].colorIndice]);
              faixa[col].qtdElementosAAdicionar--;
              if(faixa[col].qtdElementosAAdicionar==0)
                faixa[col].qtdEspacosAAdicionar = faixa[col].space;
            }else if(faixa[col].qtdEspacosAAdicionar>0){
              ledStates[pgm_read_byte(&led_map[row][col+1])] = indiceColor;
              pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[indiceColor]);
              faixa[col].qtdEspacosAAdicionar--;
              if(faixa[col].qtdEspacosAAdicionar==0)
                faixa[col].qtdElementosAAdicionar = faixa[col].tamElements;
            }
          }
        }
      }else if(faixa[col].direction==LEFT){
        for(char row=0; row<32; row++){
          if(row!=31){
            if(gameMode==RIVER){
              if(((frog.posAtual.col==col+1)&&(frog.posAtual.row==row))&&(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]==faixa[col].colorIndice)){
                if(frog.posAtual.row==0)
                  frogDeath();
                else
                  frog.posAtual.row--;;
              }
            }
            ledStates[pgm_read_byte(&led_map[row][col+1])] = ledStates[pgm_read_byte(&led_map[row + 1][col+1])];
            pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[ledStates[pgm_read_byte(&led_map[row + 1][col+1])]]);
          }else{
            if(faixa[col].qtdElementosAAdicionar>0){
              ledStates[pgm_read_byte(&led_map[row][col+1])] = faixa[col].colorIndice;
              pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[faixa[col].colorIndice]);
              faixa[col].qtdElementosAAdicionar--;
              if(faixa[col].qtdElementosAAdicionar==0)
                faixa[col].qtdEspacosAAdicionar = faixa[col].space;
            }else if(faixa[col].qtdEspacosAAdicionar>0){
              ledStates[pgm_read_byte(&led_map[row][col+1])] = indiceColor;
              pixels.setPixelColor(pgm_read_byte(&led_map[row][col+1]), colors[indiceColor]);
              faixa[col].qtdEspacosAAdicionar--;
              if(faixa[col].qtdEspacosAAdicionar==0)
                faixa[col].qtdElementosAAdicionar = faixa[col].tamElements;
            }
          }
        }
      }
    }
  }
}

void verificaDerrota(){
  if(frog.posAtual.col!=0&&frog.posAtual.col!=7){
    if(gameMode == STREET){
      if(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]!=9)
        frogDeath();
    }else if(gameMode == RIVER){
      if(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]==3)
        frogDeath();
    }
  }
}

void froggerSetup() {
  gameMode = STREET;
  score = 0;
  beginFrog();
  defineMap();
  initializeArrays();
  naoSeiQualNomeColoco();
  naoSeiQualNomeColoco2();
  Serial.println("Frogger Setup");
}

void froggerLoop() {
  // drawOnPixels(gameMode);
  defineMap();

  // movimento dos carros
  moveElements();

  if(frogCanMove()){
    frog_movement();
    Serial.println("Frog moved");
  }

  verificaDerrota();

  drawFrogOnMap();

  //Serial.println("Frogger Loop");
  pixels.show();
}
