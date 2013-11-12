#ifndef CELDA_H
#define CELDA_H
#include <cstdlib>

#define ROJO 'r'
#define AMARILLO 'y'
#define VERDE 'g'
#define AZUL 'b'
#define VIOLETA 'v'
#define VACIO 'n'
#define BUTTON 'c'
#define MINIBARH 'h'
#define MINIBARV 'p'
#define ESTRELLA 's'

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
