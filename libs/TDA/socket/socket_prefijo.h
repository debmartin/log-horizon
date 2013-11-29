#ifndef SOCKET_PREFIJO
#define SOCKET_PREFIJO

#include <string.h>

#include "common-socket.h"

class SocketPrefijo : public Socket{
  public:
    // Envia una cantidad de bytes igual a len, de msg. indicando antes
    // un prefijo de 4 bytes, la longitud del mensaje.
    // Pre: ~
    // Post: Se enviaron datos y se devolvio la cantidad de datos enviados.
    int enviar_con_prefijo(const void* msg, const int len){
      uint32_t tam = htonl((uint32_t)len);
      char *envio = new char[sizeof(tam) + len]();
      memcpy(envio, &tam, sizeof(tam));
      memcpy(&envio[sizeof(tam)], msg, len);
      int datos_enviados = Socket::enviar(envio, sizeof(tam) + len);
      delete[] envio;
      return datos_enviados;
    }
    
    // Recibe el largo del proximo mensaje a recibir.
    // Pre: ~
    // Post: Se devolvio la cantidad de bytes que se recibiran y se
    //       almaceno el largo en el parametro recibido.
    int recibir_largo(uint32_t &largo){
      int verificacion = Socket::recibir(&largo, sizeof(largo));
      largo = ntohl(largo);
      return verificacion;
    }
};

#endif // SOCKET_PREFIJO
