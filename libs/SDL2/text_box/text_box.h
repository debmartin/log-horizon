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
    ~TextBox();
    
    //
    bool asignar_fuente(std::string &path, int font_size);
    
    //
    bool asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
    
    //
    void asignar_fondo(SDL_Rect &seleccion, Textura *fondo);
    
    //
    void asignar_fondo(Textura *fondo);
    
    //
    void analizar_evento(SDL_Event &evento);
    
    //
    bool dibujar(unsigned int cantidad_caracteres, SDL_Rect &destino, Ventana *ventana);
    
    //
    void activar();
    
    //
    void desactivar();
    
    //
    bool esta_activada();
  private:
    std::string cadena;
    Textura *fondo;
    SDL_Rect *origen;
    bool activada;
    Texto *dibujador_texto;
    Textura *texto;
    bool modificacion;
};
