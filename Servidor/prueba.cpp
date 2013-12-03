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
    char salida;
    do{
        std::cin >> salida;
    } while(salida != 'q');

    server.cerrar();
    coordinador.terminar();
    server.join();
    return 0;
}
