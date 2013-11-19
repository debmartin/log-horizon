#include "Tablero.h"
#include "dimension.h"
#include <iostream>
#include <fstream>

int main(void){
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
  Tablero tablero(dim, estructura);
  while(true){
    tablero.imprimir();
    std::getline(std::cin, linea);
    if (linea[0] == 'q') return 0;
    int origen_x = linea[0] - '0';
    int origen_y = linea[1] - '0';
    Dimension origen(origen_x, origen_y);
    int destino_x = linea[2] - '0';
    int destino_y = linea[3] -'0';
    Dimension destino(destino_x, destino_y);
    Jugada j(origen, destino);
    tablero.intercambiar(&j);
    tablero.estabilizar();
  }
  return 0;
}
