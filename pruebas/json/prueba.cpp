#include <iostream>
#include <fstream>
#include <sstream>
// This is the JSON header
#include "../../libs/json/include/json/json.h"

#define COL 9
#define FIL 8

int main(int argc, char **argv){
  Json::Value estructura;
  estructura["filas"] = FIL;
  estructura["columnas"] = COL;
  
  Json::Value vec(Json::arrayValue);
  Json::Value matriz(Json::arrayValue);
  for(int x = 0; x < COL; x++){
    for (int y = 0; y < FIL; y++){
      vec.append(Json::Value(1));
    }
    matriz.append(vec);
    vec.clear();
  }
  matriz[1][2] = 0;
  matriz[3][2] = 0;
  matriz[5][2] = 0;
  matriz[7][2] = 0;
  estructura["matriz"] = matriz;
  
  std::filebuf fb;
  fb.open ("estructura.dat",std::ios::out);
  std::ostream os(&fb);
  
 // os << estructura;
  
 
  Json::StyledWriter escritor;
  std::string a = escritor.write(estructura);
  
  std::cout << a << std::endl;
  
  std::istringstream ss(a);
  Json::Reader reader;
  Json::Value aux;
  reader.parse(ss, aux, false);
  
  os << aux;
  
  return 0;
}
