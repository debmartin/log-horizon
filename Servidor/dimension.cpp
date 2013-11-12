#include "dimension.h"


Dimension::Dimension(int i, int j) :
    filas(i), cols(j) {
}

Dimension::Dimension(const Dimension& origen) :
    filas(origen.filas), cols(origen.cols) {
}

Dimension::~Dimension(){
}

bool Dimension::esValida(){
    return (filas >= 0) && (cols >= 0);
}

Dimension& Dimension::operator=(Dimension& otra){
    filas = otra.filas;
    cols = otra.cols;
    return *this;
}

bool sonAdyacentesFila(Dimension& una, Dimension& otra){
    return (una.filas == otra.filas && ((una.cols + otra.cols) % 2) == 1);
}

bool sonAdyacentesCol(Dimension& una, Dimension& otra){
    return (una.cols == otra.cols && ((una.filas + otra.filas) % 2) == 1);
}

bool sonAdyacentes(Dimension& una, Dimension& otra){
    return (sonAdyacentesFila(una, otra) || sonAdyacentesCol(una, otra));
}

int operator==(const Dimension& una, const Dimension& otra){
    return ((una.filas == otra.filas) && (una.cols == otra.cols));
}
