#ifndef LOGIN_H
#define LOGIN_H

#include "Constantes.h"
#include "estructuras.h"
#include "../libs/TDA/thread/thread.h"
#include "ArchivoDirecto.h"

class Login{
    public:
        Login(Socket* cliente, usuario_t* usuario);
        virtual ~Login();
        int asignarPuerto(Socket& sockfd, int& proximo_puerto, Mutex* mutex_prox_puerto);
        void enviarPuertos(int& prox_puerto, Mutex* mutex_prox);
        bool aceptarSubConexiones();
        int recibirUsuarioContrasenia();
        bool verificarUsuario(ArchivoDirecto& arch_usuarios);
    protected:
    private:
        usuario_t* nuevo_usuario;
        Socket* cliente_actual;
};

#endif // LOGIN_H
