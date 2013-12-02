#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "../libs/TDA/socket/socket.h"
#include <string>

using std::string;

bool recibirMsjPrefijo(Socket& sock_cli, string& mensaje);

void enviarMsjPrefijo(Socket& sock_cli, const void* mensaje, int len_msj);



#endif  // FUNCIONES_AUXILIARES_H
