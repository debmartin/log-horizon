#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include <iostream>
#include <string>

// Pasa el numero del string al parametro numero, si encuentra un caracter
// no numerico en el string, retorna false, en caso de ser solo numeros
// copia el valor en el parametro numero y retorna true
bool validar_numeros(const std::string &str, unsigned int &numero);

// Devuelve un string con el nombre archivo de la imagen de la celda
std::string archivo_celda(unsigned int x, unsigned int y);

#endif // FUNCIONES_AUXILIARES_H
