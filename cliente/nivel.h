#ifndef NIVEL_H
#define NIVEL_H

#include "SDL/SDL.h"
#include "matriz.h"
#include "lista.h"
#include <vector>

typedef struct animacion_textura{
  Animacion *animacion;
  Textura *textura;
}animacion_t;

/* ********************************************************************
 *                            EXPLOSION
 * ********************************************************************/

class Explosion{
  public:
    //
    Explosion();
    
    //
    ~Explosion();
    
    //
    void cargar_animacion(const std::string &path, Ventana *ventana);
    
    //
    void explotar(coordenada_t &celda, Matriz* tablero);
    
    //
    bool explosion_en_curso();
    
    //
    void animar();
    
    //
    bool finalizada();
    
    //
    coordenada_t ver_primera();
  
  private:
    Textura *textura;
    Animacion *animacion;
    bool en_curso;
};

/* ********************************************************************
 *                            CELDAS VACIAS
 * ********************************************************************/

class ColumnaInvalida: public std::exception{};

typedef struct reemplzado{
  coordenada_t celda;
  int color;
  int tipo;
}reemplazo_t;

class CeldasVacias{
  public:
    //
    CeldasVacias();
    
    //
    ~CeldasVacias();
    
    //
    void inicializar(int cantidad_columnas);
    
    //
    void agregar(coordenada_t &celda);
    
    //
    bool esta_vacia(int columna);

    //
    coordenada_t borrar_proxima(int columna)throw(ListaVacia, ColumnaInvalida);
    
    //
    bool existentes();

  private:
    Lista<coordenada_t> **celdas_vacias;
    int columnas;  
    int contador;
};

/* ********************************************************************
 *                            NIVEL
 * ********************************************************************/

class Nivel{
  public:
    //
    Nivel();
    
    //
    ~Nivel();
    
    //
    void correr(const std::string &path, Ventana* ventana);
  
    //
    void inicializar_datos(const std::string &path, Ventana *ventana);
    
    //
    void dibujar(Ventana *ventana);
  
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void actualizar_animaciones();
  
  private:
    int cant_animaciones;
    animacion_t **animaciones;
    Matriz *tablero;
    Explosion *explosion;
    CeldasVacias *celdas_vacias;
    
    Textura *fondo;
    Textura *seleccion;
    Mix_Chunk *sonido;
    
  protected:
    //
    int calcular_delay(FPS &frames);
    
    //
    void intercambiar(coordenada_t &origen, coordenada_t &destino);
    
    //
    void apilar(int producto, coordenada_t &celda);
    
    //
    void explotar(coordenada_t &celda);
    
    //
    void explotar_segmento(coordenada_t &origen, coordenada_t &destino);
    
    void secuencia_prueba();
};

#endif // NIVEL_H
