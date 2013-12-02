#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "funciones_auxiliares.h"


// Pasa el numero del string al parametro numero, si encuentra un caracter
// no numerico en el string, retorna false, en caso de ser solo numeros
// copia el valor en el parametro numero y retorna true
bool validar_numeros(const std::string &str, unsigned int &numero){
  size_t aux = 0;
  for (size_t x = 0; x < str.size(); x++){
    if (str[x] < '0' || str[x] > '9') return false;
      aux = str[x] - '0' + 10 * aux;
  }
  numero = aux;
  return true;
}


// Devuelve un string con el nombre archivo de la imagen de la celda
std::string archivo_celda(unsigned int x, unsigned int y){
  size_t largo_x = 1;
  if (x!= 0) largo_x = (int)log10((double)x) + 1;
  size_t largo_y = 1;
  if (y!= 0) largo_y = (int)log10((double)y) + 1;
  char *x_char = new char[largo_x + 1];
  char *y_char = new char[largo_y + 1];
  snprintf(x_char, largo_x + 1, "%ud", x);
  snprintf(y_char, largo_y + 1, "%ud", y);
  std::string celda_especial = "imagenes/" + std::string(x_char) + "-" + std::string(y_char) + ".png";
  delete[] x_char;
  delete[] y_char;
  return celda_especial;
}
