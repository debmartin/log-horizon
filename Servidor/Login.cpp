#include "Login.h"
#include "funciones_auxiliares.h"
#include "../libs/json/include/json/json.h"
#include <sstream>

#define OK 0
#define ERROR 1

#define PUERTO_MAX 65535
#define TAG_USUARIO "usuario"
#define TAG_CLAVE "clave"

using std::string;


Login::Login(Socket* cliente, usuario_t* usuario) :
    nuevo_usuario(usuario), cliente_actual(cliente) {
}

Login::~Login(){
}

int Login::asignarPuerto(Socket& sockfd, int& proximo_puerto, Mutex* mutex_prox_puerto){
    bool asociado = false;
    mutex_prox_puerto->bloquear();
    while(! asociado){
        sockfd.asignar_direccion(proximo_puerto);
        if (sockfd.reusar() == -1) return 1;
        asociado = (sockfd.asociar() != -1);
        proximo_puerto += 1;

        if (proximo_puerto == PUERTO_MAX)
            proximo_puerto = 8010;
    }
    if (sockfd.escuchar() == -1) return 2;
    mutex_prox_puerto->desbloquear();
    return 0;
}

void Login::enviarPuertos(int& proximo_puerto, Mutex* mutex_prox_puerto){
    nuevo_usuario->sockets->enviar = new Socket();
    nuevo_usuario->sockets->recibir = new Socket();

    Json::Value mensaje;
    Json::StyledWriter escritor;
    asignarPuerto(*(nuevo_usuario->sockets->enviar), proximo_puerto, mutex_prox_puerto);
    asignarPuerto(*(nuevo_usuario->sockets->recibir), proximo_puerto, mutex_prox_puerto);
//    std::cout<<"recibir"<<nuevo_usuario->sockets->recibir->ver_puerto();
//    std::cout<<"enviar"<<nuevo_usuario->sockets->enviar->ver_puerto();
    mensaje["recibir"] = htonl((uint32_t) nuevo_usuario->sockets->enviar->ver_puerto());
    mensaje["enviar"] = htonl((uint32_t) nuevo_usuario->sockets->recibir->ver_puerto());
    std::string envio = escritor.write(mensaje);
//    std::cout << "recibir: " << nuevo_usuario->sockets->enviar->ver_puerto() << ", enviar: " << nuevo_usuario->sockets->recibir->ver_puerto() << std::endl;

    enviarMsjPrefijo(*cliente_actual, envio.c_str(), envio.length());
//    cliente_actual->cerrar_enviar_recibir();
}

bool Login::aceptarSubConexiones(){
    nuevo_usuario->sockets->enviar_cli = new Socket();
    nuevo_usuario->sockets->recibir_cli = new Socket();
    Socket* sockfds[] = {nuevo_usuario->sockets->enviar, nuevo_usuario->sockets->recibir};
    Socket* socks_cli[] = {nuevo_usuario->sockets->enviar_cli, nuevo_usuario->sockets->recibir_cli};

    for(int i=0; i < 2 ; i++){
        if (sockfds[i]->aceptar(*(socks_cli[i])) == -1) {
//            std::cout << "sale";
//            delete nuevo_usuario->sockets->enviar_cli;
//            delete nuevo_usuario->sockets->recibir_cli;
            return false;
        }
    }
//    nuevo_usuario->sockets->enviar->cerrar_enviar_recibir();
//    nuevo_usuario->sockets->recibir->cerrar_enviar_recibir();
//    delete nuevo_usuario->sockets->enviar;
//    delete nuevo_usuario->sockets->recibir;
    return true;
}

int Login::recibirUsuarioContrasenia(){
    string temp;
    if (!recibirMsjPrefijo(*(nuevo_usuario->sockets->recibir_cli), temp))
        return CONEXION_ABORTADA;
    std::istringstream ss(temp);
    Json::Reader reader;
    Json::Value aux;
    reader.parse(ss, aux, false);

    nuevo_usuario->nombre = new string(aux["usuario"].asString());
    nuevo_usuario->contrasenia = new string(aux["clave"].asString());
    return 0;
}

bool Login::verificarUsuario(string& arch_usuarios){
    std::fstream arch;
    arch.open(arch_usuarios.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);
    Json::Value valores;
    Json::Reader reader;
    Json::Value aux;

    reader.parse(arch, valores, false);
    aux = valores[*(nuevo_usuario->nombre)];

    int res;
    if (! aux){
        Json::StyledStreamWriter escritor;
        valores[*(nuevo_usuario->nombre)] = *(nuevo_usuario->contrasenia);
        escritor.write(arch, valores);
        res = OK;
    } else if(*(nuevo_usuario->contrasenia) == aux.asString()){
        res = OK;
    } else {
        delete nuevo_usuario->nombre;
        delete nuevo_usuario->contrasenia;
        res = ERROR;
    }
    nuevo_usuario->sockets->enviar_cli->enviar(&res, sizeof(res));
    arch.close();
    return (res == OK);
}

