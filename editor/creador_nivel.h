#ifndef CREADOR_NIVEL_H
#define CREADOR_NIVEL_H

#include "../libs/SDL2/SDL.h"
#include "../libs/TDA/socket/socket.h"
#include "../libs/TDA/socket/socket_prefijo.h"

class CreadorNivel{
  public:
    //
    CreadorNivel();
  
    //
    virtual ~CreadorNivel();
    
    //
    bool correr(Ventana *ventana, std::string &nombre, unsigned int &columnas, unsigned int &filas);
    
    //
    int inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir);
    
  private:
    
    bool datos_inicializados; //
    Textura *fondo;
    Boton *crear_nivel;
    TextBox *nombre;
    TextBox *columnas;
    TextBox *filas;
    Texto *escritor;
    Textura *textura_nombre;
    Textura *textura_columnas;
    Textura *textura_filas;
    SDL_Rect destino_text_nombre;
    SDL_Rect destino_text_columnas;
    SDL_Rect destino_text_filas;
    Mensaje *mensaje;
    bool enviando_datos;
    
  protected:
    
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
        
    //
    //void enviar_datos();
};

#endif // CREADOR_NIVEL_H
