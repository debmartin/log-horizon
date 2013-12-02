#ifndef LOGIN_H
#define LOGIN_H

#include "Constantes.h"
#include "estructuras.h"
#include "../libs/TDA/thread/thread.h"
#include "ArchivoDirecto.h"

class Login{
    public:
        Login(Socket* cliente, usuario_t* usuario, Mutex* mutex_puertos);
        virtual ~Login();
        int asignarPuerto(Socket& sockfd);
        void enviarPuertos();
        bool aceptarSubConexiones();
        void recibirUsuarioContrasenia();
        bool verificarUsuario(ArchivoDirecto& arch_usuarios);
    protected:
    private:
        static int proximo_puerto;
        usuario_t* nuevo_usuario;
        Mutex* mutex;
        Socket* cliente_actual;
};

#endif // LOGIN_H
