#include <iostream>

#include "matriz.h"
#include "SDL/SDL.h"

#define FRACCION 10

/* ********************************************************************
 *                                CELDA
 * ********************************************************************/

//
Celda::Celda(){
  animacion = new AnimacionMovil;
  seleccion = NULL;
}

//
Celda::~Celda(){
  delete animacion;
}

//
void Celda::asignar_animacion(Textura *tex, Animacion *anim){
  animacion->asignar_animacion(anim, tex);
}

//
void Celda::seleccionar(Textura *tex){
  seleccion = tex;
}

//
void Celda::quitar_seleccion(){
  seleccion = NULL;
}

//
void Celda::asignar_posicion(SDL_Rect &rect){
  animacion->posicion_actual(rect);
}

//
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

//
bool Celda::dibujar(Ventana* ventana){
  if (seleccion){
    SDL_Rect pos_actual = animacion->ver_posicion_actual();
    seleccion->dibujar(pos_actual, ventana);
  }
  return animacion->dibujar(ventana);
}

//
void Celda::quitar_textura(){
  animacion->quitar_textura();
}

void Celda::mover(SDL_Rect &destino, int salto){
  animacion->mover(destino, salto);
}

bool Celda::movimientos_pendientes(){
  return animacion->movimientos_pendientes();
}

//
bool Celda::seleccionada(){
  return (seleccion != NULL);
}

/* ********************************************************************
 *                            MATRIZ
 * ********************************************************************/

//
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

//
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

//
void Matriz::definir_forma(char **estructura, coordenada_t &dimension, SDL_Rect &celda){
  primer_celda = celda;
  columnas = dimension.x;
  filas = dimension.y;
  celdas = new Celda**[columnas];
  for (int x = 0; x < columnas; x++){
    celdas[x] = new Celda*[filas];
    for (int y = 0; y < filas; y++){
      celdas[x][y] = NULL;
      if (estructura[x][y] == '1'){
        celdas[x][y] = new Celda;
      }
    }
  }
}

//
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

//
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

//
void Matriz::quitar_seleccion(){
  if (!celda_seleccionada) return;
  celdas[celda_seleccionada->x][celda_seleccionada->y]->quitar_seleccion();
  delete celda_seleccionada;
  celda_seleccionada = NULL;
}

//
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

//
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

//
bool Matriz::dibujar_fondo_celdas(Superficie *fondo_celda, SDL_Rect *sourc, Superficie *fondo){
  if (!celdas) return false;
  
  SDL_Rect destino;
  destino.x = primer_celda.x;
  destino.y = primer_celda.y;
  destino.w = primer_celda.w;
  destino.h = primer_celda.h;
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
  return true;
}

//
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

//
bool Matriz::esta_ocupada(){
  return ocupada;
}

//
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
