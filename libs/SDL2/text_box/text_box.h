#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../texto/texto.h"
#include "../ventana/ventana.h"
#include "../superficie/superficie.h"

class TextBox{
  public:
    //
    TextBox();
    
    //
    virtual ~TextBox();
    
    //
    bool asignar_fuente(std::string &path, int font_size, unsigned int dist_y, unsigned int cant_caracteres);
    
    //
    bool asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
    
    //
    void asignar_destino(SDL_Rect &dest);
    
    //
    void asignar_fondo(SDL_Rect &seleccion, Superficie *fondo, Ventana *ventana);
    
    //
    void asignar_fondo(Superficie *fondo, Ventana *ventana);
    
    //
    void asignar_fondo(const std::string &path, Ventana *ventana);
    
    //
    void alpha_fondo(Uint8 alpha);
    
    //
    void analizar_evento(SDL_Event &evento);
    
    //
    bool dibujar(Ventana *ventana);
    
    //
    void activar();
    
    //
    void desactivar();
    
    //
    bool esta_activada();
    
    //
    std::string ver_contenido(){return cadena;}

    //
    void ocultar();
    
    //
    void no_ocultar();
    
    //
    bool esta_oculta(){ return oculta;}

  private:
    Textura *fondo;
    SDL_Rect *origen;
    bool activada;
    Texto *dibujador_texto;
    Textura *texto;
    bool modificacion;
    unsigned int distancia_borde;
    SDL_Rect *destino;
    bool oculta;
    std::string mostrar_oculto;
    std::string mostrar;
    std::string cadena;
    size_t cantidad_caracteres;
  
  protected:
  
    //
    virtual void append_mostrar(const char c);

    //
    virtual void borrar_primero_mostrar();
};

#endif // TEXT_BOX_H
