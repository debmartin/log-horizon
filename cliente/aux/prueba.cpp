#include <iostream>
#include <SDL/SDL.h>

int main(void){
  bool Running = true;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return 1; // inicializa SDL, debe ser llamada antes de todas las funciones de SDL
  SDL_Surface *Surf_Display = SDL_SetVideoMode(763, 384, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (!Surf_Display) return 2;
  
  
  //agregamos una imagen
  SDL_Surface* Surf_Temp = SDL_LoadBMP("sprites_hda.png");
  SDL_Surface* imagen = SDL_DisplayFormat(Surf_Temp);
  SDL_FreeSurface(Surf_Temp);
  //dibujamos la imagen en la ventana
  SDL_Rect destino;
  destino.x = 0;
  destino.y=0;
  SDL_BitSurface(imagen, NULL, Surf_Display, &DestR);
  
  
  
  SDL_Event Event;
  while (Running){
    while (SDL_PollEvent(&Event)){
      if (Event.type == SDL_QUIT) Running = false;
    }
    // funciones
  }
  SDL_FreeSurface(imagen);
  SDL_FreeSurface(Surf_Display);
  SDL_Quit();
  
  
  return 0;
}
