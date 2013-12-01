#include "partida.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//
Partida::Partida(){
  escritor = new Texto;
  nombre = new Textura;
  puntos = new Textura;
  jugadores = new Textura;
  creador = new Textura;
}

//
Partida::~Partida(){
  delete escritor;
  delete nombre;
  delete puntos;
  delete jugadores;
  delete creador;
  ventana = NULL;
}

//
void Partida::inicializar(SocketPrefijo *socket_receptor, const std::string &fuente, 
                          SDL_Rect &nuevo_destino, Ventana* nueva_ventana){
  receptor_partidas.inicializar(socket_receptor);
  destino = nuevo_destino;
  escritor->asignar_fuente(fuente, 50);
  escritor->asignar_color(0,0,0,255);
  ventana = nueva_ventana;
}


//
int Partida::dibujar(Ventana *ventana){
  this->actualizar(ventana);
  SDL_Rect aux = destino;
  aux.h = destino.h / 4;
  nombre->dibujar(aux, ventana);
  aux.y += aux.h;
  puntos->dibujar(aux, ventana);
  aux.y += aux.h;
  jugadores->dibujar(aux, ventana);
  aux.y += aux.h;
  if (receptor_partidas.esta_creada())
    creador->dibujar(aux, ventana);
  return 0;
}


//
void Partida::actualizar(Ventana *ventana){
  if (!receptor_partidas.datos_recibidos() || !ventana) return;
  
  std::string texto;
  Superficie sup;
  
  nivel_info_t info;
  receptor_partidas.ver_info(info);
  receptor_partidas.datos_leidos();
  
  // NOMBRE DE LA PARTIDA
  texto = "Nivel: " + info.nombre_partida;
  escritor->copiar_texto(texto, &sup);
  nombre->cargar_textura(&sup, ventana);
  
  // PUNTOS MAXIMOS
  texto = "Puntaje para Ganar: " + this->itostr(info.puntaje_max);  
  escritor->copiar_texto(texto, &sup);
  puntos->cargar_textura(&sup, ventana);  
   
   
  // CANTIDAD DE JUGADORES Y JUGADORES MAXIMA
  std::string numero_max_jugadores = this->itostr(info.jugadores_max);
  
  std::string jugadores_conectados = this->itostr(info.jugadores);
  texto = jugadores_conectados + " de " + numero_max_jugadores + " jugadores";  
  escritor->copiar_texto(texto, &sup);
  jugadores->cargar_textura(&sup, ventana);  
  
  // CREADOR DE LA PARTIDA
  if (receptor_partidas.esta_creada()){
    texto = "Partida de " + info.nombre_creador;
    escritor->copiar_texto(texto, &sup);
    creador->cargar_textura(&sup, ventana);
  }
}

//
void Partida::recibir_datos(bool es_creada){
  receptor_partidas.es_creada(es_creada);
  receptor_partidas.correr();
}

//
std::string Partida::itostr(uint32_t valor){
  size_t largo_clave = (int)log10((double)valor) + 1;
  char *clave = new char[largo_clave + 1];
  snprintf(clave, largo_clave + 1, "%ud", valor);
  std::string str(clave);
  delete[] clave;
  return str;
}
