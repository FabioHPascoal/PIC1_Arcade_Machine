#include "frogger.h"
#include "global_vars.h"

void drawOnLedMap(Mode gameMode){
  char k;
  switch(gameMode){
    case STREET: k = 0; break;
    case RIVER: k = 1; break;
  }

  for(int row=0; row<32; row++){
    for(int col=0; col<8; col++){
      uint32_t color = pgm_read_dword(&matrixMap[k][row][col]);
      pixels.setPixelColor(pgm_read_byte(&led_map[row][col]), color);
    }
  }
}

void froggerSetup() {
  Serial.println("Frogger Setup");
  drawOnLedMap(gameMode);
  Serial.println("Terrain drew");
}

void froggerLoop() {
  //Serial.println("Frogger Loop");
  pixels.show();
}
