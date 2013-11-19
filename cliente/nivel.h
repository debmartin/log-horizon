#ifndef NIVEL_H
#define NIVEL_H

#include "SDL/SDL.h"
#include "matriz.h"
#include "lista.h"
#include "explosion.h"
#include "celdas_vacias.h"
#include "productos.h"
#include "socket.h"
#include "receptor_resultados.h"


class Nivel{
  public:
    //
    Nivel();
    
    //
    ~Nivel();
    
    //
    void correr(const std::string &path, Ventana* ventana, int ancho, int alto, Socket* enviar, Socket* recibir);
  
    //
    void inicializar_datos(const std::string &path, Ventana* ventana, int ancho, int alto, Socket* enviar, Socket* recibir);
    
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
    ReceptorResultados *receptor;
    Socket *socket_enviar;
    Lista<reemplazo_t> *celdas_a_explotar;
    
    Mix_Chunk *sonido;
    
  protected:
    //
    int calcular_delay(FPS &frames);
    
    //
    void intercambiar(coordenada_t &origen, coordenada_t &destino);
    
    //
    void apilar(int tipo, int color, coordenada_t &celda);
    
    //
    void explotar(coordenada_t &celda, int tipo, int color);
    
    //
    void actualizar_receptor();
    
    //
    void enviar_movimiento(coordenada_t &celda, coordenada_t &celda_adyacente);
    
    //
    bool validar_coordenada(coordenada_t &origen);
    
    //
    void insertar(coordenada_t &celda, int tipo, int color);
    
};

#endif // NIVEL_H
