#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>

#include "common-socket.h"


using std::string;

// Constructor de la clase, lanza una excepcion en caso de error
Socket::Socket()throw(SocketError){
  socketid = socket(AF_INET, SOCK_STREAM, 0);
  if (socketid == -1) throw SocketError();
}

// Destructor de la clase
Socket::~Socket(){
  if (socketid != -1) close(socketid);
}

// Impide que el socket pueda enviar y recibir datos.
void Socket::cerrar_enviar_recibir(){
  shutdown(socketid,2);
}

// Impide que el socket pueda recibir datos.
void Socket::cerrar_recibir(){
  shutdown(socketid,0);
}

// Impide que el socket pueda enviar datos.
void Socket::cerrar_enviar(){
  shutdown(socketid,1);
}

// Asigna una direccion ip y un puerto al socket, ingresados por parametro.
void Socket::asignar_direccion(const int puerto, const char *ip){
  dir.sin_family = AF_INET;
  dir.sin_port = htons(puerto);
  dir.sin_addr.s_addr = inet_addr(ip);
  memset(&(dir.sin_zero), 0, sizeof(dir.sin_zero));
}

// Asigna una direccion ip y un puerto al socket, ingresados por parametro.
void Socket::asignar_direccion(const int puerto, in_addr_t ip){
  dir.sin_family = AF_INET;
  dir.sin_port = htons(puerto);
  dir.sin_addr.s_addr = ip;
  memset(&(dir.sin_zero), 0, sizeof(dir.sin_zero));
}

// Intenta conectar a la direccion ip y puerto ingresados previamente
// con el metodo 'asignar_direccion'.
// Pre: Se debio asignar un puerto e ip previamente
// Post: Se establecio una conexion. En caso de error se devolvio -1.
int Socket::conectar(){
  return connect(socketid,(struct sockaddr*) &dir, sizeof(struct sockaddr));
}

// Asocia el socket a la direccion ip y puerto previamente asignados.
// Pre: Se asocio el socket a la direccion ip y puerto asignados.
// Post: Se devolvio -1 en caso de error.
int Socket::asociar(){
  return bind(socketid, (struct sockaddr*) &dir, sizeof(struct sockaddr));
}

// Intenta reusar un puerto ya abierto.
// Pre: ~
// Post: Se devolvio -1 en caso de error, caso contrario el puerto se puede
//       reusar.
int Socket::reusar(){
  int si = 1;
  return setsockopt(socketid, SOL_SOCKET, SO_REUSEADDR, &si, sizeof(si));
}

// Deja al socket escuchando en el puerto y direccion ip previamente asignados.
// Pre: El socket debe tener asignado un puerto y direccion ip
// Post: Se devolvio -1 en caso de error.
int Socket::escuchar(){
  return listen(socketid, 5);
}

// Deja al socket esperando a establecer una conexion con algun otro socket.
// Pre: el socket ingresado debe existir.
// Post: Se establecio una conexion con otro socket y se puede enviar/recibir
//       datos mediante el socket_cli ingresado por parametro
int Socket::aceptar(Socket &socket_cli){
  socklen_t cli_len = sizeof(socket_cli.dir);
  socket_cli.socketid = accept(socketid,(struct sockaddr*) &(socket_cli.dir),
                                                                    &cli_len);
  return socket_cli.socketid;
}

// Envia a la direccion ip y puertos preestablecidos una cantidad de bytes
// del mensaje igual a len
// Pre: ~
// Post: Se devolvio la cantidad de bytes enviados, en caso de error se
//       devolvio -1.
int Socket::enviar(const void* msg, const int len){
  return send(socketid, msg, len, 0);
}

// Recibe del puerto preestablecido una cantidad de bytes igual a len.
// Pre: ~
// Post: Se devolvio la cantidad de bytes recibidos, en caso de error se
//       se devolvio -1.
int Socket::recibir(void* msg, const int len){
  return recv(socketid, msg, len, 0);
}

// Devuelve el puerto que se le asigno al socket
int Socket::ver_puerto(){
  return ntohs(dir.sin_port);
}
