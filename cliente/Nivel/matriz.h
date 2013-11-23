#ifndef MATRIZ_H
#define MATRIZ_H

#include "../../libs/SDL2/SDL.h"

class Celda{
  public:
    // Constructor de clase
    Celda();
    
    // Destructor de clase
    ~Celda();
    
    // Indica a la celda que esta seleccionada, las celdas seleccionadas
    // dibujaran una textura de seleccion, ademas de la animacion.
    void seleccionar(Textura *tex);
    
    // Indica a la cenda que no esta seleccionada, para asi dejar
    // de dibujar la textura de seleccion
    void quitar_seleccion();
    
    // Asigna la posicion en la ventana en la que estara la celda.
    void asignar_posicion(SDL_Rect &rect);
    
    // Intercambia el contenido de las celdas, utilizando los metodos
    // mover de sus respectivas animaciones
    bool intercambiar(Celda* otra_celda, int salto);
    
    // Dibuja la celda en la ventana, en la ubicacion preestablecida
    bool dibujar(Ventana* ventana);
    
    // Quita la textura de la celda, para asi dibujarla vacia
    void quitar_textura();
    
    // Mueve el contenido de la celda a la ubicacion destino ingresada
    void mover(SDL_Rect &destino, int salto);
    
    // asigna una animacion y textura a la celda, que sera dibujada
    // al aplicarse el metodo dibujar de la celda
    void asignar_animacion(Textura *tex, Animacion *anim);
    
    // Devuelve true si la animacion de la celda aun no llego a destino
    // caso contrario retorna false
    bool movimientos_pendientes();
    
    // Devuelve true si la celda esta seleccionada, caso contrario retorna
    // false
    bool seleccionada();
    
  private:
    AnimacionMovil* animacion;
    Textura *seleccion;
    SDL_Rect seleccion_src;
};

typedef struct coordenada{
  int x;
  int y;
}coordenada_t;

class Matriz{
  public:
    // Constructor de clase
    Matriz();
    
    // Destructor de clase
    ~Matriz();
    
    // Define la forma de la matriz, se tomaran como celdas los puntos
    // en donde el parametro estructura contenga '1' y huecos para otro
    // caracter.
    // Recibe la cantidad de celdas y columnas en el parametro dimension.
    // Y el formato de la celda correspondiente a los subindices 0,0
    void definir_forma(char **estructura, coordenada_t &dimension, SDL_Rect &celda);
    
    // Inserta la Animacion con su correspondiente textura en la celda
    // con la coordenada ingresada por parametro
    void insertar(Textura *tex, Animacion *anim, coordenada_t &celda);
    
    // Indica a la celda cuya ubicacion es recibida por parametro
    // que esta seleccionada
    void seleccionar(Textura *tex, coordenada_t &celda);
    
    // Intercambia el contenido entre las celdas de coordenadas ingresadas
    // por parametro, originando un movimiento de las animaciones
    // de las celdas, hasta llegar a su destino.
    bool intercambiar(coordenada_t &celda1, coordenada_t &celda2);
    
    // Apila la animacion en la columna ingresada, corriendo
    // todas las animaciones de la columna hasta la fila ingresada.
    bool apilar(Textura *tex, Animacion *anim, coordenada_t &celda);
    
    // Dibuja en el espacio de cada celda, la superficie fondo celda
    // sobre la superficie fondo
    bool dibujar_fondo_celdas(Superficie *fondo_celda, SDL_Rect *sourc, Superficie *fondo);
    
    // Dibuja la matriz en la ventana ingresada
    bool dibujar(Ventana *ventana);
        
    // Quita la seleccion a la selda previamente seleccionada.
    void quitar_seleccion();
    
    // Devuelve true si se encuentran animaciones en movimiento, caso
    // contrario retorna false
    bool esta_ocupada();
    
    // devuelve true si alguna celda adyacente a la celda ingresada por parametro
    // esta seleccionada, de ser así, ingresa sus coordenadas en el parametro
    // celda_adyacente. Caso contrario retorna false.
    bool adyacente_seleccionado(coordenada_t &celda,coordenada_t &celda_adyacente);
    
    // devuelve el numero de filas de la matriz
    int numero_filas(){return filas;}
    
    // devuelve el numero de columnas de la matriz
    int numero_columnas(){return columnas;}
    
    // devuelve true si existe la celda de coordenadas ingresadas
    // por parametro, caso contrario retorna false
    bool celda_existente(coordenada_t &celda);
    
  private:
    Celda ***celdas;
    int filas;
    int columnas;
    SDL_Rect primer_celda;
    bool ocupada;
    coordenada_t *celda_seleccionada;
};

#endif // MATRIZ_H
