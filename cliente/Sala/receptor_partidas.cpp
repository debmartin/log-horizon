#include "receptor_partidas.h"
#include <string>
#include <fstream>
#include <sstream>


ReceptorPartidas::ReceptorPartidas(){
  datos_nuevos = false;
  seguir = true;
  tipo_finalizador = 0;
}

//
void ReceptorPartidas::inicializar(SocketPrefijo *socket_receptor, char finalizar){
  receptor = socket_receptor;
  tipo_finalizador = finalizar;
}

//
ReceptorPartidas::~ReceptorPartidas(){
}

//
void ReceptorPartidas::funcion_a_correr(){
  if (!receptor )return;  
  while (seguir){    
    uint32_t largo;
    if (receptor->recibir_largo(largo) == 0) return;
    datos_nuevos = true;
    char *mensaje_char = new char[largo + 1];
    mensaje_char[largo] = '\0';
    if (receptor->recibir(mensaje_char, largo) == 0) return;
    std::string mensaje_str = std::string(mensaje_char);
    delete[] mensaje_char;
    std::istringstream mensaje_stream(mensaje_str);
    Json::Reader reader;
    mutex.bloquear();
    reader.parse(mensaje_stream, recibido, false);
    mutex.desbloquear();
    std::cout << "json::value recibido = " << std::endl;
    std::cout << recibido << std::endl;
    
    if (recibido["tipo"] == tipo_finalizador) seguir = false;;
  }
}

//
void ReceptorPartidas::ver_info(Json::Value &mensaje_recibido){
  mutex.bloquear();
  mensaje_recibido = recibido;
  datos_nuevos = false;
  mutex.desbloquear();
}

//
bool ReceptorPartidas::datos_recibidos(){
  mutex.bloquear();
  bool aux = datos_nuevos;
  mutex.desbloquear();
  return aux;
}
