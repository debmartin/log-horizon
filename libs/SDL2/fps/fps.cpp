#include <iostream>

#include "fps.h"
 
// Constructor de clase
FPS::FPS(){
    OldTime = 0;
    Frames = 60;
    NumFrames = 60;
}
 
// Actualiza la cantidad de frames contados en el ultimo segundo
void FPS::actualizar(){
  if(OldTime + 1000 < (unsigned int)SDL_GetTicks()){
    OldTime = SDL_GetTicks(); 
    NumFrames = Frames;    
    Frames = 0;
  }
  Frames++;
}
 
// Devuelve la cantidad de frames en el ultimo segundo
int FPS::ver_fps(){
  return NumFrames;
}

//
void FPS::obtener_delay(int tiempo_actual, int &delay){
  if (SDL_GetTicks() - tiempo_actual < 1000){
    this->actualizar();
    delay =  (1000/60.0f) * (this->ver_fps()/60.0f);
  }
}
