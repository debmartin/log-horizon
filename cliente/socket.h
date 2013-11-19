#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>


class SocketError : public std::exception{};

class Socket{
  public:
    // Constructor de la clase, lanza una excepcion en caso de error
    Socket()throw(SocketError);
    
    // Destructor de la clase
    virtual ~Socket();
    
    // Asigna una direccion ip y un puerto al socket, ingresados por parametro.
    void asignar_direccion(const int puerto, const char *ip);
    
    // Asigna una direccion ip y un puerto al socket, ingresados por parametro.
    void asignar_direccion(const int puerto, in_addr_t ip = INADDR_ANY);
    
    // Intenta conectar a la direccion ip y puerto ingresados previamente
    // con el metodo 'asignar_direccion'.
    // Pre: Se debio asignar un puerto e ip previamente
    // Post: Se establecio una conexion. En caso de error se devolvio -1.
    int conectar();
    
    // Deja al socket escuchando en el puerto y direccion ip previamente 
    // asignados.
    // Pre: El socket debe tener asignado un puerto y direccion ip
    // Post: Se devolvio -1 en caso de error.
    int escuchar();
    
    // Devuelve el puerto que se le asigno al socket
    int ver_puerto();
    
    // Impide que el socket pueda enviar y recibir datos.
    void cerrar_enviar_recibir();

    // Impide que el socket pueda recibir datos.
    void cerrar_recibir();

    // Impide que el socket pueda enviar datos.
    void cerrar_enviar();
    
    // Deja al socket esperando a establecer una conexion con algun otro socket.
    // Pre: el socket ingresado debe existir.
    // Post: Se establecio una conexion con otro socket y se puede 
    //       enviar/recibir datos mediante el socket_cli ingresado por parametro
    int aceptar(Socket &socket_cli);
    
    // Envia a la direccion ip y puertos preestablecidos una cantidad de bytes
    // del mensaje igual a len
    // Pre: ~
    // Post: Se devolvio la cantidad de bytes enviados, en caso de error se
    //       devolvio -1.
    virtual int enviar(const void* msg, const int len);
    
    // Recibe del puerto preestablecido una cantidad de bytes igual a len.
    // Pre: ~
    // Post: Se devolvio la cantidad de bytes recibidos, en caso de error se
    //       se devolvio -1.
    virtual int recibir(void* msg, const int len);
    
    // Asocia el socket a la direccion ip y puerto previamente asignados.
    // Pre: Se asocio el socket a la direccion ip y puerto asignados.
    // Post: Se devolvio -1 en caso de error.
    int asociar();
    
    // Intenta reusar un puerto ya abierto.
    // Pre: ~
    // Post: Se devolvio -1 en caso de error, caso contrario el puerto se puede
    //       reusar.
    int reusar();
    
  private:
    int socketid;
    struct sockaddr_in dir;
};

#endif // SOCKET_H
