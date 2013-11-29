#ifndef HDA_ONLINE
#define HDA_ONLINE

#include <iostream>

#include "../libs/TDA/socket/socket.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"


class HDA_Online{
  public:
    //
    HDA_Online(int ancho, int alto, Socket *socket_emisor, Socket *socket_receptor);
  
    //
    ~HDA_Online();
    
    //
    void correr();
  private:
    Socket *emisor;
    Socket *receptor;
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
