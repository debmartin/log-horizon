#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>

#include "../libs/TDA/socket/socket_prefijo.h"
#include "../libs/TDA/socket/socket.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

class Editor{
  public:
    //
    Editor(int ancho, int alto, SocketPrefijo *socket_emisor, SocketPrefijo *socket_receptor);
     
    //
    ~Editor();

    //
    void correr();
     

    //
    bool crear_nivel();
    
    //
    bool editar_nivel();
    
  private:
    SocketPrefijo *emisor;
    SocketPrefijo *receptor;
    Ventana *ventana;
    std::string nombre_nivel;
    size_t columnas;
    size_t filas;
    SDL libreria;
};

#endif // EDITOR_H
