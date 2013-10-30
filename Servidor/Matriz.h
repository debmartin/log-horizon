#ifndef MATRIZ_H
#define MATRIZ_H
#include "dimension.h"

class Celda {
    public:
        Celda();
        Celda(char cel_color, char cel_tipo);
        ~Celda();
        Celda& operator=(const Celda& otra);
        void setColor(char n_color) { color = n_color; }
        void setTipo(char n_tipo) { tipo = n_tipo; }
        char getColor() { return color; }
        char getTipo() { return tipo; }
        bool esEstrella() { return (tipo == ESTRELLA); }
    private:
        static char rand;
        friend char random(Celda* This);
        char color;
        char tipo;
};

class Matriz{
    public:
        Matriz();
        virtual ~Matriz();
        int getTamanio() { return tamanio; }
        Celda getCelda(int i, int j);
        int intercambiar(Dimension& una, Dimension& otra);
        Celda* operator[](int i);
        Celda& operator[](Dimension& pos);
        void imprimir();
    protected:
    private:
        Celda** tabla;
        int tamanio;
};

#endif // MATRIZ_H
