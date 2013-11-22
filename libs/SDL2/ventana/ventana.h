#ifndef VENTANA_H
#define VENTANA_H

#include <SDL2/SDL.h>
#include "../superficie/superficie.h"

class Textura;
 
class Ventana{
  public:
    // Constructor de la clase
    Ventana();
    
    // Destructor de la clase
    ~Ventana();
  
    // Abre una ventana con la dimension ingresada, en pixeles
    // Pre: ancho y alto deben ser positivos
    // Post: se abrio una ventana de las dimensiones ingresadas.
    bool abrir(const int ancho, const int alto);
    
    // Establece una dimension logica a la ventana, del alto y ancho
    // ingresado.
    bool dimension_logica(const int ancho, const int alto);
    
    // Establece el titulo de la ventana
    void titulo(std::string &titulo);
    
    // Quita de la ventana todas las texturas dibujadas.
    bool limpiar();
    
    // Actualiza la ventana con las nevas texturas previamente dibujadas.
    bool presentar(const unsigned int delay);
    
    // Devuelve el renderizador de la ventana
    SDL_Renderer *ver_renderer(){return renderer;}
  private:
    SDL_Renderer *renderer;
    SDL_Window *ventana;
};


class Textura{
  public:
    // Constructor de la clase
    Textura();

    // Destructor de la clase
    ~Textura();

    // Abre el archivo ingresado y crea la textura en base a su contenido
    // Pre: el archivo debe existir.
    // Post: se cargo la textura con el contenido del archivo y se la asocio
    //       a la ventana ingresada
    bool cargar_textura(const std::string &archivo, Ventana *ventana);
    
    // Crea la textura en base al contenido de la superficie ingresada
    // Pre: la superficie debe existir.
    // Post: se cargo la textura con el contenido de la superficie y se la asocio
    //       a la ventana ingresada
    bool cargar_textura(Superficie *sup , Ventana *ventana);
      
    // dibuja el rectangulo trazado por el parametro "origen" de la textura,
    // en la ubicacion "destino" de la ventana.
    // Pre: la ventana debe existir.
    // Post: se dibujo la textura en la ventana.
    bool dibujar(SDL_Rect &origen, SDL_Rect &destino, Ventana *ventana);

    // dibuja toda la textura
    // en la ubicacion "destino" de la ventana.
    // Pre: la ventana debe existir.
    // Post: se dibujo la textura en la ventana. 
    bool dibujar(SDL_Rect &destino, Ventana *ventana);

    // Dibuja toda la textura en la ventana, a partir de la ubicacion
    // 0,0.
    // Pre: la ventana debe existir
    // Post: se dibujo la textura en la ventana.
    bool dibujar(Ventana *ventana);
    
    // Setea el nivel alpha de la textura
    bool establecer_alpha(Uint8 alpha);
    
  private:
    SDL_Texture *tex;
};

#endif // VENTANA_H
