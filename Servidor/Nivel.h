#ifndef NIVEL_H
#define NIVEL_H
#include "Lista.h"
#include "Tablero.h"
#include "Hash.h"
#include "Jugador.h"
#include <string>

class Nivel{
    public:
        Nivel();
        virtual ~Nivel();
        void agregarJugador(std::string& nombre);
        //void Nivel::jugar();
        void imprimirTablero();
        void recibirJugada(Dimension& una, Dimension& otra, std::string& id_jugador);
    protected:
        bool intentarJugada(Dimension& una, Dimension& otra);
        //void recibirJugada();
    private:
        int puntaje_objetivo;
        int puntaje_max;
        Lista<Dimension*>* modificados;
        Tablero* tablero;
        Hash<Jugador*>* jugadores;
};

#endif // NIVEL_H
