#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define CHUNK_SIZE 4096

int main(void){
  bool Running = true;
  
  // inisaliza SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return 1;
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  // creamos la ventana
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &window, &renderer);
  if (window == NULL || renderer == NULL) return 2;
  SDL_SetWindowTitle(window,"Hora de Aventura");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(renderer, 800, 600);

  // agregamos un fondo
  SDL_Surface *image = IMG_Load("fondo1_hda.jpg");
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  // agregamos a marceline
  image = IMG_Load("sprites_hda.png");
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 154, 235, 105));
  SDL_Texture *sprites = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  SDL_Rect SrcR;
  SDL_Rect DestR;
  SrcR.x = 460;
  SrcR.y = 103;
  SrcR.w = 27;
  SrcR.h = 58;
  DestR.x = 350;
  DestR.y = 250;
  DestR.w = 27;
  DestR.h = 58;
  
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, tex, NULL, NULL); // copia el fondo
  SDL_RenderCopy(renderer, sprites, &SrcR, &DestR); // copia a marceline
  
  image = IMG_Load("explosion_verde.png");
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 154, 235, 105));
  SDL_Texture *explosion = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  SDL_Rect SrcE;
  SDL_Rect DestE;
  SrcE.x = 0;
  SrcE.y = 0;
  SrcE.w = 73;
  SrcE.h = 90;
  DestE.x = 250;
  DestE.y = 250;
  DestE.w = 73;
  DestE.h = 90;
  
  SDL_RenderCopy(renderer, explosion, &SrcE, &DestE);
  
  SDL_RenderPresent(renderer);
  
  image = IMG_Load("ice_house.jpg");
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 150, 223, 255));
  SDL_Texture *fondo2 = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  SDL_Event Event;
  int contador = 0;
  int signo = 1;
  int contador_explosion = 0;
  Mix_Chunk *sound = Mix_LoadWAV("sound1.wav");
  Mix_Chunk *fondo = Mix_LoadWAV("adventure_time_theme.wav");
  Mix_PlayChannel(-1, fondo, -1);
  int alpha = SDL_ALPHA_OPAQUE;
  std::cout << SDL_ALPHA_OPAQUE << "," << SDL_ALPHA_TRANSPARENT << std::endl;
  while (Running){
    while (SDL_PollEvent(&Event)){
      if (Event.type == SDL_QUIT) Running = false;
      if (Event.type == SDL_MOUSEBUTTONDOWN && Event.button.button == SDL_BUTTON_LEFT){
        Mix_PlayChannel(-1, sound, 0);
        std::cout << "x = " << Event.button.x << std::endl;
        std::cout << "y = " << Event.button.y << std::endl;
        std::cout << std::endl;
      }else if(Event.type == SDL_MOUSEBUTTONUP && Event.button.button == SDL_BUTTON_LEFT){
        std::cout << "x = " << Event.button.x << std::endl;
        std::cout << "y = " << Event.button.y << std::endl;
        std::cout << std::endl;
      }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo2, NULL, NULL);
    if (alpha > 0){
      alpha -= 5;
      SDL_SetTextureAlphaMod(tex, alpha);
      SDL_RenderCopy(renderer, tex, NULL, NULL); // copia el fondo
    }
        
    if (contador >= 2) signo = -1;
    if (contador <= 0) signo = 1;
    contador +=signo;
    SrcR.x += signo * SrcR.w;
    
    SDL_RenderCopy(renderer, sprites, &SrcR, &DestR); // copia a marceline
    
    if (contador_explosion >= 14){
      contador_explosion = 0;
      SrcE.x = 0;
    }
    contador_explosion++;
    SrcE.x += 73;
    
    SDL_RenderCopy(renderer, explosion, &SrcE, &DestE); // copia la explosion
    SDL_RenderPresent(renderer);
    SDL_Delay(80);
  }
  
  // cerramos SDL
  Mix_FreeChunk(sound);
  Mix_FreeChunk(fondo);
  Mix_CloseAudio();
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
 }
