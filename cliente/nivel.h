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
    // Constructor de clase
    Nivel();
    
    // Destructor de clase
    ~Nivel();
    
    // Metodo principal de la clase, inicializa todos los datos necesarios
    // para la ejecucion del nivel, y lo pone en marcha.
    void correr(const std::string &path, Ventana* ventana, int ancho, int alto, Socket* enviar, Socket* recibir);
  
    // Inicializa los datos necesarios para la ejecucion del nivel
    void inicializar_datos(const std::string &path, Ventana* ventana, int ancho, int alto, Socket* enviar, Socket* recibir);
    
    // Dibuja en la ventana todos los elementos del nivel
    void dibujar(Ventana *ventana);
  
    // Analiza el evento ingresado por parametro, realizando diferentes
    // acciones en base al tipo de evento.
    // Si el evento es un click izquierdo, seleccionara la celda en
    // la ubicacion del mouse, en caso de haber una celda adyacente seleccionada,
    // intercambiara contenidos con dicha celda.
    bool analizar_evento(SDL_Event &evento);
    
    // Actualiza todas las animaciones utilizadas por el nivel.
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
    // Calcula el delay necesario para que el nivel se ejecute a 
    // 60 fps
    int calcular_delay(FPS &frames);
    
    // Intercambia el contenido de las celdas cuyas coordenadas
    // son recibidas por parametro.
    void intercambiar(coordenada_t &origen, coordenada_t &destino);
    
    // Apila en la columna de la celda, una animacion del tipo y color
    // ingresado, hasta la fila indicada por la celda.
    void apilar(int tipo, int color, coordenada_t &celda);
    
    // Realiza una explosion en la coordenada ingresada, y almacena
    // el tipo y color del nuevo producto a ser apilado en la columna,
    void explotar(coordenada_t &celda, int tipo, int color);
    
    // Actualiza el nivel con los datos recibidos desde el servidor, 
    // en caso de no recibirse ningun dato, el metodo no realiza accion
    // alguna.
    void actualizar_receptor();
    
    // Envia el movimiento realizado al servidor.
    void enviar_movimiento(coordenada_t &celda, coordenada_t &celda_adyacente);
    
    // Devuelve true si la coordenada ingresada es valida.
    // Si la coordenada se va del rango de la matriz o la celda
    // es hueca, se considerara invalida.
    bool validar_coordenada(coordenada_t &origen);
    
    // Inserta un producto del tipo y color en la celda de coordenada
    // ingresada por parametro
    void insertar(coordenada_t &celda, int tipo, int color);
    
};

#endif // NIVEL_H
