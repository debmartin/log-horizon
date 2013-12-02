#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Nivel.h"
#include "Tablero.h"
#define SEPARADORES ",\n"

using std::string;


int main(){
    Servidor server(8000, "usuarios.dat");
    server.cargarNiveles("niveles.dat");

    return 0;
}
