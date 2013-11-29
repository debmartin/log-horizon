#ifndef HDA_ONLINE
#define HDA_ONLINE

class HDA_Online{
  public:
    //
    HDA_Online(int ancho, int alto, Socket *socket_emisor, Socket *socket_receptor);
  
    //
    ~HDA_Online();
  private:
    Socket *emisor;
    Socket *receptor;
    Ventana *ventana;
};

#endif // HDA_ONLINE
