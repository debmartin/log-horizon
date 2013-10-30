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

bool sonAdyacentes(Dimension& una, Dimension& otra){
    int unaf = una.getFila(), unac = una.getCol();
    int otraf = otra.getFila(), otrac = otra.getCol();

    if(unaf == otraf && ((unac + otrac) % 2) == 1)
        return true;
    if(unac == otrac && ((unaf + otraf) % 2) == 1)
        return true;
    return false;
}

int operator==(const Dimension& una, const Dimension& otra){
    return ((una.filas == otra.filas) && (una.cols == otra.cols));
}
