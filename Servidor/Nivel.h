#ifndef NIVEL_H
#define NIVEL_H

#include "Matriz.h"
class Nivel
{
    public:
        Nivel();
        virtual ~Nivel();
        void imprimirTablero();
    protected:
        void rellenarMatriz();
    private:
        Matriz* matriz;
};

#endif // NIVEL_H
