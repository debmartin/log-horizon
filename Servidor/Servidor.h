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
#include <fstream>


class ServidorCrearSocket : public std::exception{};

class ServidorUsuario;
class CoordinadorServidor;

class Servidor : public Thread{
    friend class ServidorUsuario;
    friend class CoordinadorServidor;

    public:
        Servidor(int puerto_escucha, std::string& archivo_usuarios);
        virtual ~Servidor();
        bool cargarNiveles(string& archivo);
        void aceptarConexion();
        int generarUsuario(string& nombre);
        int elegirPartida(std::string& nombre_usuario);
        bool iniciarPartida(int nro_partida);
        void correrUsuario();
        void cerrarUsuario(std::string& nombre);
        void cerrar();
    protected:
        void cerrarPartida(int nro_partida, std::vector<usuario_t*>*& jugadores_partida);
        void funcion_a_correr();
    private:
        Mutex* mutex_aceptados;
        Mutex* mutex_conectados;
        Mutex* mutex_partidas;
        Mutex* mutex_prox_puerto;
        Socket* socket_escucha;
        std::vector<nivel_t*>* niveles;
        std::vector<Socket*>* aceptados;
        std::map<std::string, usuario_t*>* conectados;
        std::map<int, partida_t*>* partidas;
        bool seguir;
        int cant_partidas;
        ArchivoDirecto* arch_usuarios;
        int proximo_puerto;
};

class ServidorUsuario : public Thread {
    public:
        ServidorUsuario(Servidor* server);
        ~ServidorUsuario();
        void terminar();
    protected:
        void funcion_a_correr();
    private:
        Servidor* servidor;
        bool seguir;
};

//class ServidorEditor : public Thread {
//    public:
//        ServidorEditor();
//        ~ServidorEditor();
//    protected:
//        void funcion_a_correr();
//    private:
//        Servidor*
//};


class CoordinadorServidor : public Thread{
    public:
        CoordinadorServidor(Servidor* server);
        ~CoordinadorServidor();
        void terminar();
    protected:
        void funcion_a_correr();
    private:
        Servidor* servidor;
        std::vector<ServidorUsuario*>* servidores_usuarios;
        bool seguir;
};


#endif // SERVIDOR_H
