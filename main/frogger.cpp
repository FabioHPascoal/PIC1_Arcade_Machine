#include "frogger.h"
#include "global_vars.h"


/**
 * @brief funcao que desenha o mapa (apenas a ultima e a primeira linha)
 */
void defineMap(){
  for(char row=0; row<32; row++){
    for(char col=0; col<8; col++){
      if(col==0||col==7){
        ledStates[pgm_read_byte(&led_map[row][col])] = 5;
        pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), colors[5]);
      }
    }
  }
}

/**
 * @brief funcao auxiliar da funcao initializeArrays para definir o valor de i no primeiro parametro da funcao for
 * @param direction direcao dos carros de uma linha da rua ou rio
 * @return 0 se os carros/troncos estiverem indo para a direita e 31 se os carros/troncos estiverem indo para a esquerda
 */
char defineIndice(Direction direction){
  if(direction==RIGHT)
    return 0;
  else
    return 31;
}

/**
 * @brief funcao auxiliar da funcao initializeArrays para definir o limite de i no segundo parametro da funcao for
 * @param direction direcao dos carros de uma linha da rua ou rio
 * @return 32 se os carros/troncos estiverem indo para a direita e -1 se os carros/troncos estiverem indo para a esquerda
 */
char defineLimit(Direction direction){
  if(direction==RIGHT)
    return 32;
  else
    return -1;
}

/**
 * @brief funcao auxiliar da funcao initializeArrays para definir o incremento de i no terceiro parametro da funcao for
 * @param direction direcao dos carros de uma linha da rua ou rio
 * @return 1 se os carros/troncos estiverem indo para a direita e -1 se os carros/troncos estiverem indo para a esquerda
 */
char defineIncremento(Direction direction){
  if(direction==RIGHT)
    return 1;
  else
    return -1;
}


/**
 * @brief funcap que define a posição inicial dos carros/troncos de uma linha da rua/rio de acordo com suas especificacoes
 */
void initializeArrays(){
  tFaixa *faixa;
  char indiceCor;
  if(gameMode==STREET){
    faixa = street;
    indiceCor = 9;
  }else if(gameMode==RIVER){
    faixa = river;
    indiceCor = 12;
  }

  for(char line=0; line<6; line++){ 
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

/**
 * @brief funcao que define quantas unidades (em leds) de troncos/carros é necessario adicionar em uma linha iniciamente
 */
void naoSeiQualNomeColoco(){
  // todas as linhas de rua/rio vao comecar a serem movimentadas no inicio do jogo
  // essa funcao defini a quantidade de leds que deverao ser acessos quando a linha de rua/rio andar até adicionar um carro/tronco inteiro
  // por exemplo, se o tamanho do carro/tronco da linha for 3, os proximos 3 leds que forem adicionados pelo andar da rua/rio serão da cor do carro/tronco
  char line;
  for(char i=0; i<2; i++){
    if(i==0){
      for(line=0; line<6; line++){
        street[line].qtdElementosAAdicionar = street[line].tamElements; //nesse caso, 
      }
    }else{
      for(line=0; line<6; line++){
        river[line].qtdElementosAAdicionar = river[line].tamElements;
      }
    }
  }
}

/**
 * @brief funcao que define que não havera unidades de espacamento entre carros a serem adicionadas no inicio do movimento das linha de rua/rio
 */
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

/**
 * @brief funcao que inicializa o frog com os mesmos valores da variavel em global_vars
 */
void beginFrog(){
  frog = {{matrixNumRow/2, 0}, 0, 0, 80};
  frog.lastMovementTime = millis() - frog.movementInterval;
}

/**
 * @brief funcao que verifica se o sapo pode se mover de acordo com o intervalo de movimento e o ultimo momento em que ele se moveu
 * @return true se poder e false se nao
 */
bool frogCanMove(){
  unsigned long currentTime = millis();

  if (currentTime - frog.lastMovementTime >= frog.movementInterval) {
    return true;
  }

  return false;
}

/**
 * @brief funcao que movimenta o frog
 * @param direction direcao do movimento
 */
void moveFrog(Direction direction){
  pixels.setPixelColor(pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col]), colors[ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]]);
  Serial.println(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]);
  switch(direction){
    case LEFT : frog.posAtual.row--; break;
    case RIGHT: frog.posAtual.row++; break;
    case UP:    frog.posAtual.col++; break;
    case DOWN:  frog.posAtual.col--; break;
  }

  frog.lastMovementTime = millis();
}

/**
 * @brief funcao que verifica se o sapo se colidiu com alguma extremidade do mapa
 * @param direction direcao da colisao
 */
bool verficaColisao(Direction direction){
  switch(direction){
    case LEFT:  if(frog.posAtual.row == 0 ) return true; break;
    case RIGHT: if(frog.posAtual.row == 31) return true; break;
    case DOWN:  if(frog.posAtual.col == 0 ) return true; break;
    case UP:    if(frog.posAtual.col == 7 ) return true; break;
  }
  return false;
}

/**
 * @brief funcao que muda o frog de mapa definindo a linha dele como a primeira e mantendo a posicao da coluna
 */
void ReposicionaFrog(){
  frog.posAtual.col = 0;
  frog.posMaxY = 0;
}

/**
 * @brief funcao que muda o modo de jogo
 */
void changeGameMode(){
  switch(gameMode){
    case STREET: gameMode = RIVER ; break;
    case RIVER : gameMode = STREET; break;
  }
  initializeArrays();
  naoSeiQualNomeColoco();
}

/**
 * @brief funcao que incrementa a pontuacao do jogador
 */
void IncreaseScore(unsigned char value){
  score += value;
}

/**
 * @brief funcoa que verifica a direcao de movimento do frog e o movimenta se ele estiver em uma posicoa que pode ser movimentado
 */
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

/**
 * @brief funcao que desenha o sapo na matriz de led
 */
void drawFrogOnMap(){
  pixels.setPixelColor(pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col]), red);
}

/**
 * @brief funcao que desenha a morte do sapo e inicia o jogo mais uma vez apos 4 segundos
 */
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

/**
 * @brief funcao que verifica se os carros/troncos de uma rua/rio podem se morver uma unidade
 */
bool faixaCanMove(tFaixa faixa){
  if(millis()-faixa.lastMovementTime >= faixa.movementInterval){
    return true;
  }
  return false;
}

/**
 * @brief funcao que move os elementos (carros/troncos) de uma linha da rua/rio
 */
void moveElements(){
  tFaixa *faixa;
  char indiceColor;
  switch(gameMode){
    case STREET: faixa = street; indiceColor = 9; break;
    case RIVER:  faixa = river ; indiceColor = 12; break;
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

/**
 * @brief funcao que verifica se o jogador perdeu
 */
void verificaDerrota(){
  if(frog.posAtual.col!=0&&frog.posAtual.col!=7){
    if(gameMode == STREET){
      if(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]!=9)
        frogDeath();
    }else if(gameMode == RIVER){
      if(ledStates[pgm_read_byte(&led_map[frog.posAtual.row][frog.posAtual.col])]==12)
        frogDeath();
    }
  }
}

/**
 * @brief funcao que inicializa o jogo frogger
 */
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

/**
 * @brief funcao que roda o jogo
 */
void froggerLoop() {
  // drawOnPixels(gameMode);
  defineMap();

  // movimento dos carros
  moveElements();

  if(frogCanMove()){
    frog_movement();
  }

  verificaDerrota();

  drawFrogOnMap();

  //Serial.println("Frogger Loop");
  pixels.show();
}
