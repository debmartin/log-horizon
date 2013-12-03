#ifndef SALAPARTIDAS_H
#define SALAPARTIDAS_H

#include <vector>
#include <map>
#include "estructuras.h"
#include "Constantes.h"
#include "funciones_auxiliares.h"


class SalaPartidas {
    public:
        SalaPartidas(usuario_t* usuario, std::vector<nivel_t*>* v_niveles, std::map<int,partida_t*>* v_partidas);
        virtual ~SalaPartidas();
        void agregarMutex(Mutex* mutex_v_partidas);
        int definirTipoPartida();
        int crearPartida(int& cant_partidas);
        int unirsePartida();
        bool esperarInicio(int nro_partida);
        bool iniciarPartida(int nro_partida);
    protected:
    private:
        usuario_t* nuevo_usuario;
        std::vector<nivel_t*>* niveles;
        std::map<int,partida_t*>* partidas;
        Mutex* mutex_partidas;
};

#endif // SALAPARTIDAS_H
