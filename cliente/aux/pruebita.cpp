#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

int main(void){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *s = IMG_Load("sprites_hda.png");
  SDL_Color *colores = ((SDL_Color *)s->pixels);
  int rojo = colores->r;
  int verde = colores->g;
  int azul = colores->b;
  std::cout << rojo << "," << verde << "," << azul << std::endl;
  SDL_FreeSurface(s);
  SDL_Quit();
  return 0;
}
