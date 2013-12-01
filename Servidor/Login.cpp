#include "Login.h"
#include "funciones_auxiliares.h"

#define OK '0'
#define ERROR '1'

#define ARCH_USUARIOS "./usuarios.json"
#define PUERTO_MAX 65535


using std::string;

int Login::proximo_puerto = PUERTO_ESCUCHA+1;

Login::Login(Socket* cliente, usuario_t* usuario, Mutex* mutex_puertos) :
    nuevo_usuario(usuario), mutex(mutex_puertos), cliente_actual(cliente) {
    string arch(ARCH_USUARIOS);
    arch_usuarios = new ArchivoDirecto(arch);
    arch_usuarios->abrir();
}

Login::~Login(){
    arch_usuarios->cerrar();
}

int Login::asignarPuerto(Socket& sockfd){
    bool asociado = false;
    mutex->bloquear();
    while(! asociado){
        sockfd.asignar_direccion(proximo_puerto);
        if (sockfd.reusar() == -1) return 1;
        asociado = (sockfd.asociar() != -1);

        proximo_puerto++;
        if (proximo_puerto == PUERTO_MAX)
            proximo_puerto = PUERTO_ESCUCHA +1;
    }
    mutex->desbloquear();
    if (sockfd.escuchar() == -1) return 2;
    return 0;
}

void Login::enviarPuertos(){
    nuevo_usuario->sockets->enviar = new Socket();
    nuevo_usuario->sockets->recibir = new Socket();
    Socket* socks[] = {nuevo_usuario->sockets->enviar, nuevo_usuario->sockets->recibir};

    for(int i = 0; i < 2; i++){
        Socket& actual = *(socks[i]);
        asignarPuerto(actual);
        uint32_t nro_puerto = htonl((uint32_t) actual.ver_puerto());
        cliente_actual->enviar(&nro_puerto, sizeof(uint32_t));
    }
    cliente_actual->cerrar_enviar_recibir();
}

bool Login::aceptarSubConexiones(){
    nuevo_usuario->sockets->enviar_cli = new Socket();
    nuevo_usuario->sockets->recibir_cli = new Socket();
    Socket* sockfds[] = {nuevo_usuario->sockets->enviar, nuevo_usuario->sockets->recibir};
    Socket* socks_cli[] = {nuevo_usuario->sockets->enviar_cli, nuevo_usuario->sockets->recibir_cli};

    for(int i=0; i < 2 ; i++){
        if (sockfds[i]->aceptar(*(socks_cli[i])) == -1) {
            delete nuevo_usuario->sockets->enviar_cli;  // REVISAR no hay que cerrar antes de borrar?
            delete nuevo_usuario->sockets->recibir_cli;
            return false;
        }
    }
    nuevo_usuario->sockets->enviar->cerrar_enviar_recibir();
    nuevo_usuario->sockets->recibir->cerrar_enviar_recibir();
    delete nuevo_usuario->sockets->enviar;
    delete nuevo_usuario->sockets->recibir;
    return true;
}

void Login::recibirUsuarioContrasenia(){
    nuevo_usuario->nombre = new string();
    recibirMsjPrefijo(*(nuevo_usuario->sockets->recibir_cli), *(nuevo_usuario->nombre));

    nuevo_usuario->contrasenia = new string();
    recibirMsjPrefijo(*(nuevo_usuario->sockets->recibir_cli), *(nuevo_usuario->contrasenia));
}

bool Login::verificarUsuario(){
    string aux_contrasenia;
    if (! arch_usuarios->obtener(*(nuevo_usuario->nombre), aux_contrasenia)) {
        arch_usuarios->agregar(*(nuevo_usuario->nombre), *(nuevo_usuario->contrasenia));
        return true;
    }
    char res = (aux_contrasenia == *(nuevo_usuario->contrasenia)) ? OK : ERROR;
    nuevo_usuario->sockets->enviar_cli->enviar(&res, sizeof(char));

    if (res == ERROR) delete nuevo_usuario->nombre;
    delete nuevo_usuario->contrasenia;

    return (res == OK);
}

