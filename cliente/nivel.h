#ifndef NIVEL_H
#define NIVEL_H

#include "SDL/SDL.h"
#include "matriz.h"
#include "lista.h"
#include "explosion.h"
#include "celdas_vacias.h"
#include "productos.h"
#include "socket.h"


class Nivel{
  public:
    //
    Nivel();
    
    //
    ~Nivel();
    
    //
    void asignar_sockets(Socket* enviar, Socket* recibir);
    
    //
    void correr(const std::string &path, Ventana *ventana, int ancho, int alto);
  
    //
    void inicializar_datos(const std::string &path, Ventana *ventana, int ancho, int alto);
    
    //
    void dibujar(Ventana *ventana);
  
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void actualizar_animaciones();
  
  private:
    Productos *productos;
    Matriz *tablero;
    Explosion *explosion;
    CeldasVacias *celdas_vacias;
    Textura *fondo;
    Textura *seleccion;
    int ancho_celda;
    int alto_celda;
    
    Socket *socket_enviar;
    Socket *socket_recibir;
    
    Mix_Chunk *sonido;
    
  protected:
    //
    int calcular_delay(FPS &frames);
    
    //
    void intercambiar(coordenada_t &origen, coordenada_t &destino);
    
    //
    void apilar(int tipo, int color, coordenada_t &celda);
    
    //
    void explotar(coordenada_t &celda);
    
};

#endif // NIVEL_H
