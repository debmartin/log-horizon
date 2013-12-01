#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "../libs/TDA/thread/thread.h"
#include "../libs/TDA/socket/socket.h"
#include <exception>
#include <string>
#include "estructuras.h"
#include <vector>
#include <map>
#include "Constantes.h"
#include "Login.h"
#include "SalaPartidas.h"
#include "Nivel.h"


class ServidorCrearSocket : public std::exception{};

class ServidorUsuario;

class Servidor : public Thread{
    public:
        Servidor();
        virtual ~Servidor();
        void cargarNiveles();
        void aceptarConexion();
        std::string generarUsuario();
        int elegirPartida(std::string& nombre_usuario);
        bool iniciarPartida(int nro_partida);
        void correrUsuario();
        void cerrarUsuario(std::string& nombre);
        void cerrar();
    protected:
        void cerrarPartida(int nro_partida, std::vector<usuario_t*>*& jugadores_partida);
        void funcion_a_correr();
    private:
        Mutex* mutex_login;
        Socket* socket_escucha;
        std::vector<nivel_t*>* niveles;
        std::vector<Socket*>* aceptados;
        std::vector<usuario_t*>* en_espera_login;
        std::map<std::string, usuario_t*>* conectados;
        std::map<int, partida_t*>* partidas;
        std::vector<ServidorUsuario*>* servidores;
        bool seguir;
        int cant_partidas;
};

class ServidorUsuario : public Thread {
    public:
        ServidorUsuario(Servidor* nuevo_servidor);
        ~ServidorUsuario();
        void terminar();
    protected:
        void funcion_a_correr();
    private:
        Servidor* servidor;
        bool seguir;
};

#endif // SERVIDOR_H
