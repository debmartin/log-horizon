#include <iostream>

#include "matriz.h"
#include "../../libs/SDL2/SDL.h"

#define FRACCION 10

/* ********************************************************************
 *                                CELDA
 * ********************************************************************/

// Constructor de clase
Celda::Celda(){
  animacion = new AnimacionMovil;
  seleccion = NULL;
}

// Destructor de clase
Celda::~Celda(){
  delete animacion;
}

// asigna una animacion y textura a la celda, que sera dibujada
// al aplicarse el metodo dibujar de la celda
void Celda::asignar_animacion(Textura *tex, Animacion *anim){
  animacion->asignar_animacion(anim, tex);
}

// Indica a la celda que esta seleccionada, las celdas seleccionadas
// dibujaran una textura de seleccion, ademas de la animacion.
void Celda::seleccionar(Textura *tex){
  seleccion = tex;
}

// Indica a la cenda que no esta seleccionada, para asi dejar
// de dibujar la textura de seleccion
void Celda::quitar_seleccion(){
  seleccion = NULL;
}

// Asigna la posicion en la ventana en la que estara la celda.
void Celda::asignar_posicion(SDL_Rect &rect){
  animacion->posicion_actual(rect);
}

// Intercambia el contenido de las celdas, utilizando los metodos
// mover de sus respectivas animaciones
bool Celda::intercambiar(Celda* otra_celda, int salto){
  if (!otra_celda) return false;
  
  AnimacionMovil* temp = otra_celda->animacion;
  
  SDL_Rect origen = animacion->ver_posicion_destino();
  SDL_Rect destino = temp->ver_posicion_destino();
  
  if (animacion) animacion->mover(destino, salto);
  if (temp) temp->mover(origen, salto);
  
  otra_celda->animacion = animacion;
  animacion = temp;
  seleccion = NULL;
  otra_celda->seleccion = NULL;
  return true;
}

// Dibuja la celda en la ventana, en la ubicacion preestablecida
bool Celda::dibujar(Ventana* ventana){
  if (seleccion){
    SDL_Rect pos_actual = animacion->ver_posicion_actual();
    seleccion->dibujar(pos_actual, ventana);
  }
  return animacion->dibujar(ventana);
}

// Quita la textura de la celda, para asi dibujarla vacia
void Celda::quitar_textura(){
  animacion->quitar_textura();
}

 // Mueve el contenido de la celda a la ubicacion destino ingresada
void Celda::mover(SDL_Rect &destino, int salto){
  animacion->mover(destino, salto);
}

// Devuelve true si la animacion de la celda aun no llego a destino
// caso contrario retorna false
bool Celda::movimientos_pendientes(){
  return animacion->movimientos_pendientes();
}

// Devuelve true si la celda esta seleccionada, caso contrario retorna
// false
bool Celda::seleccionada(){
  return (seleccion != NULL);
}

/* ********************************************************************
 *                            MATRIZ
 * ********************************************************************/

// Constructor de clase
Matriz::Matriz(){
  celdas = NULL;
  filas = 0;
  columnas = 0;
  primer_celda.x = 0;
  primer_celda.y = 0;
  primer_celda.w = 0;
  primer_celda.h = 0;
  ocupada = false;
  celda_seleccionada = NULL;
}

// Destructor de clase
Matriz::~Matriz(){
  if (celdas){
    for (int x = 0; x < columnas; x++){
      for (int y = 0; y < filas; y++){
        if (celdas[x][y]) delete celdas[x][y];
      }
      delete[] celdas[x];
    }
    delete[] celdas;
  }
}

// Define la forma de la matriz, se tomaran como celdas los puntos
// en donde el parametro estructura contenga '1' y huecos para otro
// caracter.
// Recibe la cantidad de celdas y columnas en el parametro dimension.
// Y el formato de la celda correspondiente a los subindices 0,0
void Matriz::definir_forma(char **estructura, coordenada_t &dimension, SDL_Rect &celda){
  primer_celda = celda;
  columnas = dimension.x;
  filas = dimension.y;
  celdas = new Celda**[columnas];
  for (int x = 0; x < columnas; x++){
    celdas[x] = new Celda*[filas];
    for (int y = 0; y < filas; y++){
      celdas[x][y] = NULL;
      if (estructura[x][y] == 1){
        celdas[x][y] = new Celda;
      }
    }
  }
}

// Inserta la Animacion con su correspondiente textura en la celda
// con la coordenada ingresada por parametro
void Matriz::insertar(Textura *tex, Animacion *anim, coordenada_t &celda){
  if (!celdas[celda.x][celda.y]) return;
  SDL_Rect destino;
  destino.x = primer_celda.x + celda.x * primer_celda.w;
  destino.y = primer_celda.y + celda.y * primer_celda.h;
  destino.w = primer_celda.w;
  destino.h = primer_celda.h;
  celdas[celda.x][celda.y]->asignar_animacion(tex, anim);
  celdas[celda.x][celda.y]->asignar_posicion(destino);
}

// Indica a la celda cuya ubicacion es recibida por parametro
// que esta seleccionada
void Matriz::seleccionar(Textura *tex, coordenada_t &celda){
  if (celda.x < 0 || celda.x > columnas || celda.y < 0 || celda.y > filas) return;
  
  if (celdas[celda.x][celda.y]){
    if (celda_seleccionada){
      celdas[celda_seleccionada->x][celda_seleccionada->y]->quitar_seleccion();
    }else{
      celda_seleccionada = new coordenada_t;
    }
    celda_seleccionada->x = celda.x;
    celda_seleccionada->y = celda.y;
    celdas[celda.x][celda.y]->seleccionar(tex);
  }
}

// Quita la seleccion a la selda previamente seleccionada.
void Matriz::quitar_seleccion(){
  if (!celda_seleccionada) return;
  celdas[celda_seleccionada->x][celda_seleccionada->y]->quitar_seleccion();
  delete celda_seleccionada;
  celda_seleccionada = NULL;
}

// Intercambia el contenido entre las celdas de coordenadas ingresadas
// por parametro, originando un movimiento de las animaciones
// de las celdas, hasta llegar a su destino.
bool Matriz::intercambiar(coordenada_t &celda1, coordenada_t &celda2){
  Celda *primera_celda = celdas[celda1.x][celda1.y];
  Celda *segunda_celda = celdas[celda2.x][celda2.y];
  if (!primera_celda || !segunda_celda) return false;
  bool exito = primera_celda->intercambiar(segunda_celda, primer_celda.w / FRACCION);
  if (exito && celda_seleccionada){
    delete celda_seleccionada;
    celda_seleccionada = NULL;
  }
  return exito;
}

// Apila la animacion en la columna ingresada, corriendo
// todas las animaciones de la columna hasta la fila ingresada.
bool Matriz::apilar(Textura *tex, Animacion* anim, coordenada_t &celda){
  if (!celdas[celda.x][celda.y]) return false;
  celdas[celda.x][celda.y]->quitar_textura();
  
  int primer_fila = 0;
  while (primer_fila <= celda.y && !celdas[celda.x][primer_fila]){
    primer_fila++;
  }
  
  for (int i = celda.y; i >= primer_fila; i--){
    if (celdas[celda.x][i]){
      int anterior = i - 1;
      while (anterior >= 0 && !celdas[celda.x][anterior])
        anterior--;
      if (anterior < 0)
        anterior = primer_fila;
      if (celdas[celda.x][anterior])
        celdas[celda.x][i]->intercambiar(celdas[celda.x][anterior], primer_celda.w / FRACCION);
    }
  }
  
  SDL_Rect destino;
  destino.x = celda.x * primer_celda.w + primer_celda.x;
  destino.y = primer_celda.y + primer_fila * primer_celda.h;
  destino.h = primer_celda.h;
  destino.w = primer_celda.w;
  SDL_Rect origen;
  origen.x = celda.x * primer_celda.w + primer_celda.x;
  origen.y = primer_celda.y - primer_celda.h;
  origen.h = primer_celda.h;
  origen.w = primer_celda.w;
  
  celdas[celda.x][primer_fila]->asignar_animacion(tex, anim);
  celdas[celda.x][primer_fila]->asignar_posicion(origen);
  celdas[celda.x][primer_fila]->mover(destino, primer_celda.w / FRACCION);
  return true;
}

// Dibuja en el espacio de cada celda, la superficie fondo celda
// sobre la superficie fondo
bool Matriz::dibujar_fondo_celdas(Superficie *fondo_celda, SDL_Rect *sourc, Superficie *fondo, coordenada_t &celda){
  if (!celdas) return false;
  
  SDL_Rect destino;
  destino.x = primer_celda.x + primer_celda.w * celda.x;
  destino.y = primer_celda.y + primer_celda.h * celda.y;
  destino.w = primer_celda.w;
  destino.h = primer_celda.h;
  SDL_BlitScaled(fondo_celda->ver_surface(), sourc, fondo->ver_surface(), &destino);
  /*
  for (int x = 0; x < columnas; x++){
    for (int y = 0; y < filas; y++){
      if (celdas[x][y]){
        SDL_BlitScaled(fondo_celda->ver_surface(), sourc, fondo->ver_surface(), &destino);
      }
      destino.y += primer_celda.h;
    }
    destino.y = primer_celda.y;
    destino.x += primer_celda.w;
  }
  */
  return true;
}

// Dibuja la matriz en la ventana ingresada
bool Matriz::dibujar(Ventana *ventana){
  if (!celdas || !ventana) return false;
  
  SDL_Rect destino;
  destino.x = primer_celda.x;
  destino.y = primer_celda.y;
  destino.w = primer_celda.w;
  destino.h = primer_celda.h;
  ocupada = false;
  for (int x = 0; x < columnas; x++){
    for (int y = 0; y < filas; y++){
      if (celdas[x][y]){
        celdas[x][y]->dibujar(ventana);        
        ocupada = ocupada || celdas[x][y]->movimientos_pendientes();
      }
      destino.y += primer_celda.h;
    }
    destino.y = primer_celda.y;
    destino.x += primer_celda.w;
  }
  return true;
}

// Devuelve true si se encuentran animaciones en movimiento, caso
// contrario retorna false
bool Matriz::esta_ocupada(){
  return ocupada;
}

// devuelve true si alguna celda adyacente a la celda ingresada por parametro
// esta seleccionada, de ser así, ingresa sus coordenadas en el parametro
// celda_adyacente. Caso contrario retorna false.
bool Matriz::adyacente_seleccionado(coordenada_t &celda, coordenada_t &celda_adyacente){
  if (!celda_seleccionada || celda.x < 0 || celda.x > columnas || celda.y < 0 || celda.y > filas) return false;
  if (celda.x + 1 == celda_seleccionada->x && celda.y == celda_seleccionada->y){
    celda_adyacente.x = celda.x + 1;
    celda_adyacente.y = celda.y;
    return true;
  }
  if (celda.x - 1 == celda_seleccionada->x && celda.y == celda_seleccionada->y){
    celda_adyacente.x = celda.x - 1;
    celda_adyacente.y = celda.y;
    return true;
  }
  if (celda.x == celda_seleccionada->x && celda.y + 1 == celda_seleccionada->y){
    celda_adyacente.x = celda.x;
    celda_adyacente.y = celda.y + 1;
    return true;
  }
  if (celda.x == celda_seleccionada->x && celda.y - 1 == celda_seleccionada->y){
    celda_adyacente.x = celda.x;
    celda_adyacente.y = celda.y - 1;
    return true;
  }
  return false;
}

// devuelve true si existe la celda de coordenadas ingresadas
// por parametro, caso contrario retorna false
bool Matriz::celda_existente(coordenada_t &celda){
  if (celda.x < 0 || celda.x >= columnas || celda.y < 0 || celda.y >= filas) return false;
  return celdas[celda.x][celda.y] != NULL;
}
