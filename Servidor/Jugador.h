#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador{
    public:
        Jugador(std::string& nombre);
        virtual ~Jugador();
        int getPuntaje() { return puntaje; }
        bool contarPuntos(int largo_linea, bool conEstrella);
    protected:
    private:
        std::string* id;
        int puntaje;
};

#endif // JUGADOR_H
