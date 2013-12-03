#include <string>

#include "autentificador.h"
#include "../../libs/json/include/json/json.h"

#define OK '0'

//
Autentificador::Autentificador(){
  emisor = NULL;
  receptor = NULL;
  enviando = false;
  usuario = "";
  clave = "";
  validacion = false;
}

//
Autentificador::~Autentificador(){}

//
void Autentificador::asignar_sockets(SocketPrefijo *enviar, 
                                     SocketPrefijo *recibir){
  if (!enviar || !recibir) return;
  emisor = enviar;
  receptor = recibir;
}

//
void Autentificador::autentificar(const std::string &_usuario, 
                                  const std::string &_clave){
  usuario = _usuario;
  clave = _clave;
}

//
bool Autentificador::en_curso(){
  return enviando;
}

//
bool Autentificador::usuario_valido(){
  return validacion;
}

//
void Autentificador::funcion_a_correr(){
  if (!emisor || !receptor || usuario == "" || clave == "") return;
  mutex.bloquear();
  enviando = true;
  mutex.desbloquear();
  Json::Value mensaje;
  Json::StyledWriter escritor;
  mensaje["usuario"] = usuario;
  mensaje["clave"] = clave;
  std::string envio = escritor.write(mensaje);
  std::cout << "value del usuario y contraseña: " << std::endl;
  std::cout << envio << std::endl;
  emisor->enviar_con_prefijo(envio.c_str(), envio.size());
  char resultado;
  receptor->recibir(&resultado, sizeof(resultado));
  mutex.bloquear();
  if (resultado == OK) 
    validacion = true;
  else 
    validacion = false;
  enviando = false;
  mutex.desbloquear();
}
