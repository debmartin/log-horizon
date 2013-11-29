#ifndef HDA_ONLINE
#define HDA_ONLINE

#include <iostream>

#include "../libs/TDA/socket/socket_prefijo.h"
#include "../libs/TDA/socket/socket.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"


class HDA_Online{
  public:
    //
    HDA_Online(int ancho, int alto, SocketPrefijo *socket_emisor, SocketPrefijo *socket_receptor);
  
    //
    ~HDA_Online();
    
    //
    void correr();
  private:
    SocketPrefijo *emisor;
    SocketPrefijo *receptor;
    Ventana *ventana;
    SDL libreria;
    
  protected:
    //
    bool login();
    
    //
    bool sala();
    
    //
    bool nivel();
};

#endif // HDA_ONLINE
