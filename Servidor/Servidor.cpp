#include "Servidor.h"

#include  <cstring>
#include "funciones_auxiliares.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define MILISEGUNDOS 10

#define ARCH_NIVELES "./niveles.json"


using std::string;
using std::vector;
using std::map;

ServidorUsuario::ServidorUsuario(Servidor* nuevo_servidor) :
    Thread(), servidor(nuevo_servidor), seguir(true) {
}

ServidorUsuario::~ServidorUsuario(){
}

void ServidorUsuario::terminar(){
    seguir = false;
}

void ServidorUsuario::funcion_a_correr(){
    servidor->correrUsuario();
}





Servidor::Servidor() : seguir(true), cant_partidas(0) {
    socket_escucha = new Socket();
    socket_escucha->asignar_direccion(PUERTO_ESCUCHA);
    if (socket_escucha->reusar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->asociar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->escuchar() == -1) throw ServidorCrearSocket();

    aceptados = new vector<Socket*>();
    en_espera_login = new vector<usuario_t*>();
    niveles = new vector<nivel_t*>();
    mutex_login = new Mutex();
    conectados = new map<string, usuario_t*>;
    partidas = new map<int, partida_t*>();
}

Servidor::~Servidor(){
    delete socket_escucha;
}

void Servidor::cargarNiveles(){
    //FALTA
}

void Servidor::aceptarConexion(){
    Socket* cliente_actual = new Socket();
    socket_escucha->aceptar(*cliente_actual);
    aceptados->push_back(cliente_actual);
}

string Servidor::generarUsuario(){
    int largo = aceptados->size();
    if(largo == 0) return string();

    Socket* cliente_actual = (*aceptados)[largo-1];
    aceptados->pop_back();
    usuario_t* nuevo_usuario = new usuario_t();
    Login* nuevo_login = new Login(cliente_actual, nuevo_usuario, mutex_login);

    nuevo_login->enviarPuertos();

    delete cliente_actual;
    cliente_actual = NULL;

    nuevo_login->aceptarSubConexiones();
    do{
        nuevo_login->recibirUsuarioContrasenia();
    } while (! nuevo_login->verificarUsuario());

    delete nuevo_login;
    conectados->insert(std::pair<string, usuario_t*>(*(nuevo_usuario->nombre), nuevo_usuario));
    return *(nuevo_usuario->nombre);
}

int Servidor::elegirPartida(string& nombre_usuario){
    usuario_t* usuario = (*conectados)[nombre_usuario];
    SalaPartidas* nueva_sala = new SalaPartidas(usuario, niveles, partidas);
    int nro_partida = -1;

    while(true){
        int tipo_partida = nueva_sala->definirTipoPartida();

        bool volver = true;
        while(volver) {
            if(tipo_partida == CREAR){
                nro_partida = nueva_sala->crearPartida(cant_partidas);
                if(nro_partida == -1) break;
                volver = ! nueva_sala->iniciarPartida(nro_partida);
            } else {
                nro_partida = nueva_sala->unirsePartida();
                if(nro_partida == -1) break;
                volver = ! nueva_sala->esperarInicio(nro_partida);
            }
        }
        if(! volver) break;
    }
    delete nueva_sala;
    return nro_partida;
}

bool Servidor::iniciarPartida(int nro_partida){
    std::ifstream aux_tablero;
    aux_tablero.open((*partidas)[nro_partida]->nivel->archivo_tablero->c_str(), std::ifstream::in);
    std::ifstream aux_proba;
    aux_proba.open((*partidas)[nro_partida]->nivel->archivo_probabilidades->c_str(), std::ifstream::in);
    if (! aux_proba.is_open() || ! aux_tablero.is_open())
        return false;
    std::istream* arch_tablero = &aux_tablero;
    std::istream* arch_proba = &aux_proba;

    Nivel* nuevo_juego = new Nivel(arch_tablero, arch_proba, (*partidas)[nro_partida]->nivel->puntaje);
    for(vector<usuario_t*>::iterator it = (*partidas)[nro_partida]->jugadores->begin(); it != (*partidas)[nro_partida]->jugadores->end(); ++it)
        nuevo_juego->agregarJugador(**it);
    nuevo_juego->enviarTablero();

    nuevo_juego->jugar();
    nuevo_juego->cerrarJugador();
    delete nuevo_juego;

    aux_tablero.close();
    aux_proba.close();

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
    usuario_t* usuario = (*conectados)[nombre];
    conectados->erase(nombre);
    delete usuario->nombre;

    if(usuario->contrasenia) delete usuario->contrasenia;

    usuario->sockets->enviar_cli->cerrar_enviar_recibir();
    delete usuario->sockets->enviar_cli;
    usuario->sockets->recibir_cli->cerrar_enviar_recibir();
    delete usuario->sockets->recibir_cli;
    delete usuario->sockets;
}

void Servidor::correrUsuario(){
    string nombre = generarUsuario();
    while (nombre.length() == 0){
        usleep(MILISEGUNDOS * 1000);
        continue;
    }
    bool salir = false;
    while(seguir && ! salir){
        int nro_partida = elegirPartida(nombre);
        if(nombre == *((*partidas)[nro_partida]->creador)){
            iniciarPartida(nro_partida);
            vector<usuario_t*>* jugadores = NULL;
            cerrarPartida(nro_partida, jugadores);
            for(vector<usuario_t*>::iterator it = jugadores->begin(); it != jugadores->end(); ++it)
                en_espera_login->push_back(*it);        //agregarSalida
            delete jugadores;
        }
    }
    cerrarUsuario(nombre);
    usuario_t* usuario = (*conectados)[nombre];
    conectados->erase(nombre);
    delete usuario;
}

void Servidor::funcion_a_correr(){
    while(seguir){
        aceptarConexion();
        ServidorUsuario* server = new ServidorUsuario(this);
        servidores->push_back(server);
    }
}

void Servidor::cerrar(){
    seguir = false;
    for(vector<ServidorUsuario*>::iterator it = servidores->begin(); it != servidores->end(); ++it){
        (*it)->terminar();
        (*it)->join();
        delete (*it);
    }
}
