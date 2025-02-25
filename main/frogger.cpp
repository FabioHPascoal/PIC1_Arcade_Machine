#include "frogger.h"
#include "global_vars.h"

void drawTerrain(){
  for(int row=0;row<32;row++){
    for(int col=0;col<8;col++){
      if(col==0||col==7){
        matrixMapStreet[row][col] = emerald;
        matrixMapRiver[row][col] = emerald;
      }else{
        matrixMapStreet[row][col] = gray;
        matrixMapRiver[row][col] = blue;
      }
    }
  }
}

void initializeFrog(){
  frog.posAtual.row = (matrixNumRow/2);
  frog.posAtual.col = 0;
  pixels.setPixelColor(led_map[frog.posAtual.row][frog.posAtual.col], green);
}

void beginTimeVariables(){
  frog.lastMovementTimeFrogger = millis() - frog.movementIntervalFrogger;
}

bool verficaColisaoEsquerda(){
  if(frog.posAtual.row == 0)
    return true;
  return false;
}

bool verficaColisaoDireita(){
  if(frog.posAtual.row == matrixNumRow -1)
    return true;
  return false;
}

bool verficaColisaoInferior(){
  if(frog.posAtual.col == 0)
    return true;
  return false;
}

bool verficaColisaoSuperior(){
  if(frog.posAtual.col == matrixNumCol -1)
    return true;
  return false;
}

void moveFrog(char direction, char value){
  if(direction == 'V')
    frog.posAtual.col+= value;
  else if(direction == 'H')
    frog.posAtual.row+= value;

}

void ReposicionaFrog(){
  frog.posAtual.col = 0;
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

void defineMatrixFaixa(){
  //street
    //faixa 6
    street[5].type = streetMode;
    street[5].direction = true;
    street[5].tamElements = 2;
    street[5].space = 4;
    street[5].speed = 2;
    street[5].cor = white;
    //faixa 5
    street[4].type = streetMode;
    street[4].direction = false;
    street[4].tamElements = 1;
    street[4].space = 8;
    street[4].speed = 8;
    street[4].cor = red;
    //faixa 4
    street[3].type = streetMode;
    street[3].direction = true;
    street[3].tamElements = 1;
    street[3].space = 5;
    street[3].speed = 2;
    street[3].cor = magenta;
    //faixa 3
    street[2].type = streetMode;
    street[2].direction = false;
    street[2].tamElements = 1;
    street[2].space = 3;
    street[2].speed = 1;
    street[2].cor = blue;
    //faixa 2
    street[1].type = streetMode;
    street[1].direction = true;
    street[1].tamElements = 1;
    street[1].space = 3;
    street[1].speed = 1;
    street[1].cor = yellow;
    //faixa 1
    street[0].type = streetMode;
    street[0].direction = false;
    street[0].tamElements = 3;
    street[0].space = 5;
    street[0].speed = 2;
    street[0].cor = brown;
  
  //river
    //faixa 6
    river[5].type = !streetMode;
    river[5].direction = true;
    river[5].tamElements = 5;
    river[5].space = 12;
    river[5].speed = 10;
    river[5].cor = brown;
    //faixa 5
    river[4].type = !streetMode;
    river[4].direction = false;
    river[4].tamElements = 2;
    river[4].space = 3;
    river[4].speed = 4;
    river[4].cor = red;
    //faixa 4
    river[3].type = !streetMode;
    river[3].direction = true;
    river[3].tamElements = 8;
    river[3].space = 14;
    river[3].speed = 8;
    river[3].cor = brown;
    //faixa 3
    river[2].type = !streetMode;
    river[2].direction = false;
    river[2].tamElements = 3;
    river[2].space = 4;
    river[2].speed = 6;
    river[2].cor = brown;
    //faixa 2
    river[1].type = !streetMode;
    river[1].direction = true;
    river[1].tamElements = 3;
    river[1].space = 2;
    river[1].speed = 5;
    river[1].cor = red;
    //faixa 1
    river[0].type = !streetMode;
    river[0].direction = false;
    river[0].tamElements = 2;
    river[0].space = 4;
    river[0].speed = 4;
    river[0].cor = brown;
}

bool verificaAdicaoDeElemento(struct faixaTroncoOuRua faixa, char col, const uint32_t **matrixMap){
  if(faixa.verificadorDeContinuidade != 0){
    return true;
  }

  unsigned char space = 0;
  
  if(faixa.direction){
    for(int row=0; matrixMap[row][col+1] != faixa.cor ; row++)
      space++;
    if(space==faixa.space)
      return true;
  }else{
    for(int row=31; matrixMap[row][col+1] != faixa.cor ; row--)
      space++;
    if(space==faixa.space)
      return true;
  }

  return false;
}

unsigned short getInterval(unsigned short speed, char divisor){
  return (1000/speed)/divisor;
}

bool verificaSePodeAndar(struct faixaTroncoOuRua *faixas, char col, char divisor){
  unsigned long currentTime = millis();
  if(currentTime - faixas[col].lastMovementTime >= getInterval(faixas[col].speed, divisor)){
    return true;
  }
  return false;
}

void MoveFaixa(struct faixaTroncoOuRua *faixas, char col, const uint32_t **matrixMap, char type){
  if(faixas[col].direction){ //se for pra direita
    for(int row=31; row>=0; row--){
      if(row==0){
        if(type=='s')
          matrixMap[row][col+1] = gray;
        else
          matrixMap[row][col+1] = blue;
      }
      matrixMap[row][col+1] = matrixMap[row-1][col+1];
    }
  }else{
    for(int row=0; row<32; row++){ //se for para esquerda
      if(row==31){
        if(type=='s')
          matrixMap[row][col+1] = gray;
        else
          matrixMap[row][col+1] = blue;
      }
      matrixMap[row][col+1] = matrixMap[row+1][col+1];
    }
  }
}

void AdicionaElemento(struct faixaTroncoOuRua *faixas, char col, const uint32_t **matrixMap){
  if(faixas[col].direction)
    matrixMap[0][col+1] = faixas[col].cor;
  else
    matrixMap[31][col+1] = faixas[col].cor;
  if(faixas[col].verificadorDeContinuidade != 0)
    faixas[col].verificadorDeContinuidade--;
  else
    faixas[col].verificadorDeContinuidade = faixas[col].tamElements - 1;
}

void prepareTerrain(){
  bool fristTime = true;
  char cont = 0;
  while(true){
    for(int col=0; col<6; col++){
      if(verificaSePodeAndar(street, col, 10)){ //para rua
        bool i;
        if(cont==0)
          i = true;
        else
          i = verificaAdicaoDeElemento(street[col], col, matrixMapStreet);
        if(i){
          MoveFaixa(street, col, matrixMapStreet, 's');
          AdicionaElemento(street, col, matrixMapStreet);
        }else{
          MoveFaixa(street, col, matrixMapStreet, 's');
        }
      }
    }

    for(int col=0; col<6; col++){
      if(verificaSePodeAndar(river, col, 10)){ //para rua
        bool i;
        if(cont==0)
          i = true;
        else
          i = verificaAdicaoDeElemento(river[col], col, matrixMapRiver);
        if(i){
          MoveFaixa(river, col, matrixMapRiver, 'r');
          AdicionaElemento(river, col, matrixMapRiver);
        }else{
          MoveFaixa(river, col, matrixMapRiver, 'r');
        }
      }
    }

    if(matrixMapStreet[0][3]==street[2].cor){
      break;
    }
    i++;
  }
}

void froggerSetup(){
  streetMode = true; // definicao do primeiro mapa como a rua
  drawTerrain();
  initializeFrog();
  defineMatrixFaixa();
  prepareTerrain();
  beginTimeVariables();
}

void froggerLoop(){

  //adicionar os dois modos de jogo
  if(streetMode){
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