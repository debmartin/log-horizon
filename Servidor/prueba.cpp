#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Nivel.h"
#include "Tablero.h"
#define SEPARADORES ",\n"

using std::string;


int main(){
    std::string direccion_archivo = "estructura.dat";
    std::ifstream archivo_estructura(direccion_archivo.c_str(), std::ifstream::in);
    std::string linea;

    std::getline(archivo_estructura, linea);
    int columnas = atoi(linea.c_str());
    std::getline(archivo_estructura, linea);
    int filas = atoi(linea.c_str());

    char **estructura = new char*[columnas];
    char caracter;
    for (int i = 0; i < columnas; i++){
        estructura[i] = new char[filas];
        for (int z = 0; z < filas; z++){
          caracter = archivo_estructura.get();
          if (caracter == '\n')
            caracter = archivo_estructura.get();
          estructura[i][z] = caracter;
        }
    }
    Dimension dim(filas, columnas);
//    Nivel* nivel = new Nivel(dim, estructura, 300);
//    std::string nombre("yo");
//    nivel->agregarJugador(nombre);
//    nivel->enviarTablero();
//    nivel->jugar();
//    nivel->cerrarJugador();
    for(int  i=0; i<columnas; i++)
        delete[] estructura[i];
    delete[] estructura;
//    delete nivel;
    return 0;
}
