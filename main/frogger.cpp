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

void initializeFrog

void froggerSetup(){
  streetMode = true;
  drawTerrain();
  
  // posicionar carros
}

void froggerLoop(){
    
}