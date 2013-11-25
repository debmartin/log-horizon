#include <iostream>
#include <fstream>

// This is the JSON header
#include "../../libs/json/include/json/json.h"

#define COL 3
#define FIL 4

int main(int argc, char **argv){
  Json::Value estructura;
  estructura["filas"] = FIL;
  estructura["columnas"] = COL;
  
  Json::Value vec(Json::arrayValue);
  Json::Value matriz(Json::arrayValue);
  for(int x = 0; x < FIL; x++){
    for (int y = 0; y < COL; y++){
      vec.append(Json::Value(x));
    }
    matriz.append(vec);
    vec.clear();
  }
  
  estructura["matriz"] = matriz;
  
  std::filebuf fb;
  fb.open ("estructura.dat",std::ios::out);
  std::ostream os(&fb);
  
  os << estructura;
  
 return 0;
}
