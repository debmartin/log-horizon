#ifndef CELDA_H
#define CELDA_H
#include <cstdlib>
#include "Constantes.h"

class Celda {
    public:
        Celda();
        Celda(char cel_color, char cel_tipo);
        ~Celda();
        Celda& operator=(const Celda& otra);
        void rellenar();
        void rellenar(char cel_color, char cel_tipo);
        void setColor(char n_color) { color = n_color; }
        void setTipo(char n_tipo) { tipo = n_tipo; }
        char getColor() { return color; }
        char getTipo() { return tipo; }
        bool esEstrella();
        bool esVacia();
    private:
        char color;
        char tipo;
};

#endif // CELDA_H
