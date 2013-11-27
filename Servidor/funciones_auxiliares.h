#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "../libs/TDA/socket/socket.h"
#include <string>

void recibirMsjPrefijo(Socket& sock_cli, std::string& mensaje);

void enviarMsjPrefijo(Socket& sock_cli, void* mensaje, int len_msj);


#endif  // FUNCIONES_AUXILIARES_H
