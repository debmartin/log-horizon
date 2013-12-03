#include "Servidor.h"

#include  <cstring>
#include "funciones_auxiliares.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#define MILISEGUNDOS 10

#define SEPARADOR ",\n"

using std::string;
using std::vector;
using std::map;
using std::ifstream;


/*      CoordinadorUsuario      */


CoordinadorServidor::CoordinadorServidor(Servidor* server) :
    servidor(server), seguir(true){
    servidores_usuarios = new vector<ServidorUsuario*>();
}

CoordinadorServidor::~CoordinadorServidor(){
    delete servidores_usuarios;
}

void CoordinadorServidor::funcion_a_correr(){
    while(seguir){
        while (servidor->aceptados->size() == 0)
            usleep(MILISEGUNDOS * 1000);
        ServidorUsuario* serverus = new ServidorUsuario(servidor);
        servidores_usuarios->push_back(serverus);
        serverus->correr();
    }
}

void CoordinadorServidor::terminar(){
    seguir = false;
    for(vector<ServidorUsuario*>::iterator it = servidores_usuarios->begin(); it != servidores_usuarios->end(); ++it){
        (*it)->terminar();
        (*it)->join();
        delete (*it);
    }
}


/*      ServidorUsuario     */

ServidorUsuario::ServidorUsuario(Servidor* server) :
    Thread(), servidor(server), seguir(true){
}

ServidorUsuario::~ServidorUsuario(){
}

void ServidorUsuario::terminar(){
    seguir = false;
}

void ServidorUsuario::funcion_a_correr(){
    string nombre;
    int res = servidor->generarUsuario(nombre);
    bool salir = (res == CONEXION_ABORTADA);

    while(seguir && !salir){
        int nro_partida = servidor->elegirPartida(nombre);
        if(nro_partida == CONEXION_ABORTADA) break;

//        if(nombre == *((*(servidor->partidas))[nro_partida]->creador)){
//            (*(servidor->partidas))[nro_partida]->jugando.bloquear();
//            servidor->iniciarPartida(nro_partida);
//            vector<usuario_t*>* jugadores = NULL;
//            cerrarPartida(nro_partida, jugadores);
//            (*(servidor->partidas))[nro_partida]->jugando.desbloquear();
//        } else {
//            (*(servidor->partidas))[nro_partida]->jugando.bloquear();
//            (*(servidor->partidas))[nro_partida]->jugando.desbloquear();
//        }
    }
    servidor->cerrarUsuario(nombre);
}


/*      Servidor        */


Servidor::Servidor(int puerto_escucha, string& archivo_usuarios) :
    seguir(true), cant_partidas(0), proximo_puerto(puerto_escucha+1) {
    socket_escucha = new Socket();
    socket_escucha->asignar_direccion(puerto_escucha);
    if (socket_escucha->reusar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->asociar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->escuchar() == -1) throw ServidorCrearSocket();

    mutex_aceptados = new Mutex();
    mutex_conectados = new Mutex();
    mutex_partidas = new Mutex();
    mutex_prox_puerto = new Mutex();

    aceptados = new vector<Socket*>();
    niveles = new vector<nivel_t*>();
    conectados = new map<string, usuario_t*>;
    partidas = new map<int, partida_t*>();

    arch_usuarios = new ArchivoDirecto(archivo_usuarios);
    arch_usuarios->abrir();
}

Servidor::~Servidor(){
    for(vector<nivel_t*>::iterator it = niveles->begin(); it != niveles->end(); ++it){
        delete (*it)->nombre;
        delete (*it)->archivo_tablero;
        delete (*it)->archivo_probabilidades;
        delete (*it);
    }
    delete niveles;
    delete conectados;
    delete partidas;
    delete socket_escucha;
    arch_usuarios->cerrar();
    delete arch_usuarios;
    for(vector<Socket*>::iterator it = aceptados->begin(); it != aceptados->end(); ++it){
        (*it)->cerrar_enviar_recibir();
        delete (*it);
    }
    delete aceptados;

    delete mutex_prox_puerto;
    delete mutex_aceptados;
    delete mutex_conectados;
    delete mutex_partidas;
}

bool Servidor::cargarNiveles(string& archivo){
//    string comando("ls >./niveles.dat");
//    system(comando.c_str());
//    Json::Value valores;
//    Json::Reader reader;
//
//    if (!reader.parse(archivo, valores, false))
//        return false;


    ifstream arch_niveles;
    arch_niveles.open(archivo.c_str(), std::ifstream::in);
    if(!arch_niveles.is_open()) return false;

    while(arch_niveles.good()){
        string linea_nivel;
        getline(arch_niveles, linea_nivel);
        if(linea_nivel.length() == 0) break;
        nivel_t* nivel = new nivel_t();
        int i = 0;

        while(i < 5){
            size_t pos = linea_nivel.find_first_of(SEPARADOR);
            string aux(linea_nivel.substr(0, pos));
            linea_nivel.erase(0, pos+1);

            switch (i){
                case 0:
                    nivel->nombre = new string(aux);
                    break;
                case 1:
                    nivel->puntaje = atoi(aux.c_str());
                    break;
                case 2:
                    nivel->cant_jugadores_max = atoi(aux.c_str());
                    break;
                case 3:
                    nivel->archivo_tablero = new string(aux);
                    break;
                case 4:
                    nivel->archivo_probabilidades = new string(aux);
            }
            i++;
        }
        niveles->push_back(nivel);
    }
    arch_niveles.close();
    return true;
}

void Servidor::aceptarConexion(){
    Socket* cliente_actual = new Socket();
    socket_escucha->aceptar(*cliente_actual);
    mutex_aceptados->bloquear();
    aceptados->push_back(cliente_actual);
    mutex_aceptados->desbloquear();
}

int Servidor::generarUsuario(string& nombre){
    mutex_aceptados->bloquear();
    int largo = aceptados->size();
    if(largo == 0) return -1;

    Socket* cliente_actual = aceptados->back();
    aceptados->pop_back();
    mutex_aceptados->desbloquear();
    usuario_t* nuevo_usuario = new usuario_t();
    nuevo_usuario->sockets = new sockets_jugador_t();
    Login* nuevo_login = new Login(cliente_actual, nuevo_usuario);

    nuevo_login->enviarPuertos(proximo_puerto, mutex_prox_puerto);

    delete cliente_actual;
    cliente_actual = NULL;

    nuevo_login->aceptarSubConexiones();
    do{
        if (nuevo_login->recibirUsuarioContrasenia() == CONEXION_ABORTADA)
            return CONEXION_ABORTADA;
    } while (! nuevo_login->verificarUsuario(*arch_usuarios));

    delete nuevo_login;
    mutex_conectados->bloquear();
    conectados->insert(std::pair<string, usuario_t*>(*(nuevo_usuario->nombre), nuevo_usuario));
    mutex_conectados->desbloquear();
    nombre = *(nuevo_usuario->nombre);
    return 0;
}

int Servidor::elegirPartida(string& nombre_usuario){
    mutex_conectados->bloquear();
    usuario_t* usuario = (*conectados)[nombre_usuario];
    mutex_conectados->desbloquear();

    SalaPartidas* nueva_sala = new SalaPartidas(usuario, niveles, partidas);
    nueva_sala->agregarMutex(mutex_partidas);
    int nro_partida = -1, res = -1;

    while(true){
        int tipo_partida = nueva_sala->definirTipoPartida();
        if (tipo_partida == CONEXION_ABORTADA) return CONEXION_ABORTADA;

        bool volver = true;
        while(volver) {
            if(tipo_partida == CREAR){
                nro_partida = nueva_sala->crearPartida(cant_partidas);
                if (nro_partida == CONEXION_ABORTADA) break;
                if(nro_partida == -1) break;

                res = nueva_sala->iniciarPartida(nro_partida);
                if (res == CONEXION_ABORTADA) break;
                volver = (res == VOLVER);
            } else {
                nro_partida = nueva_sala->unirsePartida();
                if (nro_partida == CONEXION_ABORTADA) break;
                if(nro_partida == -1) break;

                res = nueva_sala->esperarInicio(nro_partida);
                if (res == CONEXION_ABORTADA) break;
                volver = (res == VOLVER);
            }
        }
        if(! volver) break;
    }
    delete nueva_sala;
    if (res == CONEXION_ABORTADA || nro_partida == CONEXION_ABORTADA)
        return CONEXION_ABORTADA;
    return nro_partida;
}

bool Servidor::iniciarPartida(int nro_partida){
    std::ifstream arch_tablero;
    arch_tablero.open((*partidas)[nro_partida]->nivel->archivo_tablero->c_str(), std::ifstream::binary);
    std::ifstream arch_proba;
    arch_proba.open((*partidas)[nro_partida]->nivel->archivo_probabilidades->c_str(), std::ifstream::binary);
    if (! arch_proba.is_open() || ! arch_tablero.is_open())
        return false;

    Nivel* nuevo_juego = new Nivel(arch_tablero, arch_proba, (*partidas)[nro_partida]->nivel->puntaje);
    for(vector<usuario_t*>::iterator it = (*partidas)[nro_partida]->jugadores->begin(); it != (*partidas)[nro_partida]->jugadores->end(); ++it)
        nuevo_juego->agregarJugador(**it);
    nuevo_juego->enviarTablero();

    nuevo_juego->jugar();
    nuevo_juego->cerrarJugador();
    delete nuevo_juego;

    arch_tablero.close();
    arch_proba.close();

    return true;
}

void Servidor::cerrarPartida(int nro_partida, vector<usuario_t*>*& jugadores_partida){
    int i = 0;
    map<int, partida_t*>::iterator it = partidas->begin();
    for(; it != partidas->end() || nro_partida == i; ++it, i++);
    if(it == partidas->end()) return;

    partida_t* partida_terminada = it->second;
    partidas->erase(it);
    jugadores_partida = partida_terminada->jugadores;
    delete partida_terminada;
}

void Servidor::cerrarUsuario(string& nombre){
    mutex_conectados->bloquear();
    usuario_t* usuario = (*conectados)[nombre];
    conectados->erase(nombre);
    mutex_conectados->desbloquear();
    delete usuario->nombre;

    if(usuario->contrasenia) delete usuario->contrasenia;

    usuario->sockets->enviar_cli->cerrar_enviar_recibir();
    delete usuario->sockets->enviar_cli;
    usuario->sockets->recibir_cli->cerrar_enviar_recibir();
    delete usuario->sockets->recibir_cli;
    delete usuario->sockets;
    delete usuario;
}

//void Servidor::correrUsuario(){
//    string nombre;
//    int res = generarUsuario(nombre);
//    bool salir = (res == CONEXION_ABORTADA);
//
//    while (res == -1){
//        usleep(MILISEGUNDOS * 1000);
//        res = generarUsuario(nombre);
//    }
//    while(seguir && !salir){
//        int nro_partida = elegirPartida(nombre);
//        if(nro_partida == CONEXION_ABORTADA) break;
//
////        if(nombre == *((*partidas)[nro_partida]->creador)){
////            (*partidas)[nro_partida]->jugando.bloquear();
////            iniciarPartida(nro_partida);
////            vector<usuario_t*>* jugadores = NULL;
////            cerrarPartida(nro_partida, jugadores);
////            for(vector<usuario_t*>::iterator it = jugadores->begin(); it != jugadores->end(); ++it)
////                en_espera_login->push_back(*it);
////            delete jugadores;
////            (*partidas)[nro_partida]->jugando.desbloquear();
////        } else {
////            (*partidas)[nro_partida]->jugando.bloquear();
////            (*partidas)[nro_partida]->jugando.desbloquear();
////        }
//    }
//    cerrarUsuario(nombre);
//}

void Servidor::funcion_a_correr(){
    seguir = true;
    while(seguir){
        aceptarConexion();
    }
}

void Servidor::cerrar(){
    seguir = false;
}
