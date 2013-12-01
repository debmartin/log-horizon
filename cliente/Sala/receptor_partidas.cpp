#include "receptor_partidas.h"
#include <string>

#define OK 6
#define ERROR 7

ReceptorPartidas::ReceptorPartidas(){
  creada = false;
  datos_nuevos = false;
  seguir = true;
  info.nombre_creador = "";
  info.nombre_partida = "";
  info.jugadores = 0;
  info.puntaje_max = 0;
  info.jugadores_max = 0;
}

//
void ReceptorPartidas::inicializar(SocketPrefijo *socket_receptor){
  receptor = socket_receptor;
}

//
ReceptorPartidas::~ReceptorPartidas(){
}

//
void ReceptorPartidas::funcion_a_correr(){
  uint32_t max_pjs, pjs, max_puntos, largo;
  char *partida_aux, *creador_aux;
  
  while (seguir){
    char finalizar;
    receptor->recibir(&finalizar, sizeof(finalizar));
    
    std::cout << "se recibio de finalizar " << (int) finalizar << std::endl;
    if (finalizar == ERROR) return;
    std::cout << "va a recibir datos de la partida" << std::endl;
    
    receptor->recibir_largo(largo);
    partida_aux = new char[largo+1];
    partida_aux[largo] = '\0';
    receptor->recibir(partida_aux, largo);
    receptor->recibir(&max_puntos, sizeof(max_puntos));
    receptor->recibir(&max_pjs, sizeof(max_pjs));
    receptor->recibir(&pjs, sizeof(pjs));
    std::cout << "largo = " << largo << " partida = " << partida_aux << " puntos = " << max_puntos << " max pjs = " << max_pjs << " pjs = " << pjs << std::endl;
    if (creada){
      receptor->recibir_largo(largo);
      creador_aux = new char[largo+1];
      creador_aux[largo] = '\0';
      receptor->recibir(creador_aux, largo);
      std::cout << " largo2 = " << largo << " creador = " << creador_aux << std::endl;
    }
    
    mutex.bloquear();
    info.nombre_partida = std::string(partida_aux);
    delete[] partida_aux;
    info.puntaje_max = max_puntos;
    info.jugadores_max = max_pjs;
    info.jugadores = pjs;
    if (creada){
      info.nombre_creador = std::string(creador_aux);
      delete[] creador_aux;
    }
    datos_nuevos = true;
    mutex.desbloquear();
  }
}

//
void ReceptorPartidas::ver_info(nivel_info_t &nivel){
  nivel.nombre_partida = info.nombre_partida;
  nivel.puntaje_max = info.puntaje_max;
  nivel.jugadores_max = info.jugadores_max;
  nivel.jugadores = info.jugadores;
  if (creada){
    nivel.nombre_creador = info.nombre_creador;
  }
}

//
bool ReceptorPartidas::datos_recibidos(){
  mutex.bloquear();
  bool aux = datos_nuevos;
  mutex.desbloquear();
  return aux;
}

//
void ReceptorPartidas::datos_leidos(){
  mutex.bloquear();
  datos_nuevos = false;
  mutex.desbloquear();
}
