#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Servidor.h"

using std::string;


int main(){
    string usu("usuarios.dat"), niv("niveles.dat");
    Servidor server(8000, usu);
    server.cargarNiveles(niv);
    server.correr();
    CoordinadorServidor coordinador(&server);
    coordinador.correr();
    usleep(10000000);
    server.cerrar();
    server.join();
    return 0;
}
