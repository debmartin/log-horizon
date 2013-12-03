#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "partida.h"
#include "../../libs/json/include/json/json.h"

#define CREAR '0'
#define UNIRSE '1'
#define RECIBIR_NIVEL '2'

//
Partida::Partida(){
  escritor = new Texto;
  nombre = new Textura;
  puntos = new Textura;
  jugadores = new Textura;
  creador = new Textura;
  creada = false;
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
  receptor_partidas.inicializar(socket_receptor, RECIBIR_NIVEL);
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
  if (creada)
    creador->dibujar(aux, ventana);
  return 0;
}


//
bool Partida::actualizar(Ventana *ventana){
  if (!receptor_partidas.datos_recibidos() || !ventana) return false;
  
  std::string texto;
  Superficie sup;
  
  Json::Value info;
  receptor_partidas.ver_info(info);
  
  // if info["tipo"].asInt() == RECIBIR_NIVEL, salir de aca y terminar todo para iniciar el juego
  
  if (info["tipo"].asInt() == CREAR && creada) return false;
  if (info["tipo"].asInt() == UNIRSE && !creada) return false;
  
  // NOMBRE DE LA PARTIDA
  texto = "Nivel: " + info["nombre"].asString();
  escritor->copiar_texto(texto, &sup);
  nombre->cargar_textura(&sup, ventana);
  
  // PUNTOS MAXIMOS
  unsigned int puntaje_int = info["puntaje"].asInt();
  puntaje_int = ntohl(puntaje_int);
  std::string puntaje_str = this->itostr(puntaje_int);
  texto = "Puntaje para Ganar: " + puntaje_str;  
  escritor->copiar_texto(texto, &sup);
  puntos->cargar_textura(&sup, ventana);
   
  // CANTIDAD DE JUGADORES Y JUGADORES MAXIMA
  unsigned int jugadores_int = info["jugadores"].asInt();
  jugadores_int = ntohl(jugadores_int);
  std::string jugadores_str = this->itostr(jugadores_int);
  unsigned int max_jugadores_int = info["max jugadores"].asInt();
  max_jugadores_int = ntohl(max_jugadores_int);
  std::string max_jugadores_str = this->itostr(max_jugadores_int);
  texto = jugadores_str + " de " + max_jugadores_str + " jugadores";
  escritor->copiar_texto(texto, &sup);
  jugadores->cargar_textura(&sup, ventana);  
  
  // CREADOR DE LA PARTIDA
  if (info["tipo"] == UNIRSE){
    texto = "Partida de " + info["creador"].asString();
    escritor->copiar_texto(texto, &sup);
    creador->cargar_textura(&sup, ventana);
  }
  return true;
}

//
void Partida::partidas_creadas(bool es_creada){
  creada = es_creada;
}

//
void Partida::recibir_datos(){
  receptor_partidas.correr();
}

//
std::string Partida::itostr(uint32_t valor){
  size_t largo_clave = 1;
  if (valor > 0) largo_clave = (int)log10((double)valor) + 1;
  char *clave = new char[largo_clave + 1];
  snprintf(clave, largo_clave + 1, "%ud", valor);
  std::string str(clave);
  delete[] clave;
  return str;
}
