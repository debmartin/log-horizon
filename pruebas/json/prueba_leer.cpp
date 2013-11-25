#include <iostream>
#include <fstream>

// This is the JSON header
#include "../../libs/json/include/json/json.h"

// HAY QUE CORRER PRIMERO ./prueba PARA PODER REALIZAR ESTA OTRA PRUEBA

#define COL 3
#define FIL 4

int main(int argc, char **argv){
  Json::Value estructura;
  Json::Value aux;
  Json::Reader reader;
  
  std::ifstream test("estructura.dat", std::ifstream::binary);
  if (!reader.parse(test, estructura, false)) return 1;
  aux = estructura.get("filas",aux );
  int filas = aux.asInt();
  std::cout << "filas = " << filas << std::endl;
  aux = estructura.get("columnas",aux );
  int columnas = aux.asInt();
  std::cout << "columnas = " << columnas << std::endl;
  
  aux = estructura.get("matriz",aux );
  
  for (int x = 0; x < filas; x++){
    for (int y = 0; y < columnas; y++){
      std::cout << aux[x][y].asInt();
    }
    std::cout << std::endl;
  }
  
  return 0;
}
