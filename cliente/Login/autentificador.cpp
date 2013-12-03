#include "autentificador.h"

#define OK 0

//
Autentificador(){
  emisor = NULL;
  receptor = NULL;
  enviando = false;
  usuario = "";
  clave = "";
  validacion = false;
}

//
~Autentificador(){}

//
void asignar_sockets(SocketPrefijo *enviar, SocketPrefijo *recibir){
  if (!enviar || !recibir) return;
  emisor = enviar;
  receptor = recibir;
}

//
void autentificar(const std::string &_usuario, const std::string &_clave){
  usuario = _usuario;
  clave = _clave;
}

//
bool en_curso(){
  return enviando;
}

//
bool usuario_valido(){
  return validacion;
}

//
void funcion_a_correr(){
  if (!emisor || !receptor || usuario == "" || clave == "") return;
  mutex.bloquear();
  enviando = true;
  mutex.desbloquear();
  Json::Value mensaje;
  Json::StyledWriter escritor;
  mensaje["usuario"] = usuario;
  mensaje["clave"] = clave;
  std::string envio = escritor.write(mensaje);
  emisor->enviar_con_prefijo(envio, envio.size());
  char resultado;
  receptor->recibir(&resultado, sizeof(resultado));
  mutex.bloquear();
  if (resultado == OK) validacion = true;
  else validacion = false;
  enviando = false;
  mutex.desbloquear();
}
