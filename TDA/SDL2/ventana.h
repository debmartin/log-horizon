#ifndef VENTANA_H
#define VENGANA_H

#include <SDL2/SDL.h>
#include "superficie.h"

/*
 * Previamente debe inicializarse las librerias SDL antes de utulizar 
 * esta clase con la siguiente funcion:
 * 
 *                   int SDL_Init(Uint32 flags)
 * 
 * Así mismo, una vez finalizadas todas las tareas que involucren 
 * a las librerias SDL se debe utilizar la funcion:
 * 
 *                   void SDL_Quit()
 */
 
 
class Textura{
  public:
    // Constructor de la clase
    Textura();

    // Destructor de la clase
    ~Textura();

    // Carga la textura del archivo, cuyo nombre es ingresado por
    // parametro, retorna false en caso de error.
    // Pre: El renderer debe existir
    // Post: Se guarda la textura y se retorno true, en caso de error
    //       solo se retorno false.
    bool cargar_textura(const std::string archivo, SDL_Renderer *renderer);
    
    //
    bool cargar_textura(const std::string archivo, SDL_Renderer *renderer, SDL_Color &colores);
        
    //
    bool color_clave(SDL_Renderer *renderer, const colores_t &colores);
    
    // 
    virtual bool dibujar(SDL_Rect &origen, SDL_Rect &destino, SDL_Renderer *renderer);
    
    // 
    virtual bool dibujar(SDL_Rect &destino, SDL_Renderer *renderer);
    
    // 
    virtual bool dibujar(SDL_Renderer *renderer);
    
  private:
    SDL_Texture *tex;
};


class Ventana{
  public:
    // Constructor de la clase, asigna NULL a cada atributo que sea
    // puntero. 
    Ventana();
    
    // Destructor de la clase, libera la memoria reserbada por los
    // atributos en caso de previamente haberse utilizado el metodo
    // 'abrir'.
    ~Ventana();
  
    //
    bool abrir(const int ancho, const int alto); 
    
    //
    void titulo(std::string &titulo);
    
    //
    bool limpiar();
    
    //
    void presentar(const unsigned int delay);
    
    //
    bool dibujar(Textura *textura, SDL_Rect &origen, SDL_Rect &destino);
    
    //
    bool dibujar(Textura *textura, SDL_Rect &destino);
    
    //
    bool dibujar(Textura *textura);
    
    //
    SDL_Renderer *ver_renderer(){ return renderer;}
    
  private:
    SDL_Renderer *renderer;
    SDL_Window *ventana;
};

#endif // VENTANA_H
