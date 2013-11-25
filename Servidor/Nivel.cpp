#include "Nivel.h"

#define PUERTO_ENVIAR 8000
#define PUERTO_RECIBIR 8001


using std::string;

Nivel::Nivel(Dimension& tam, char** estructura, int max_puntos) :
    puntaje_objetivo(max_puntos) {
    srand(time(NULL));
    tablero = new Tablero(tam, estructura);
    mutex_recibir = new Mutex();
    jugadores = new ConjuntoJugadores(mutex_recibir);
}

Nivel::~Nivel(){
    delete tablero;
    delete jugadores;
    delete mutex_recibir;
}

void Nivel::agregarJugador(string& nombre){
    jugadores->agregar(nombre, PUERTO_ENVIAR, PUERTO_RECIBIR);
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}

void Nivel::enviarTablero(){
    Dimension tam = tablero->getTamanio();
    Lista<celda_t*> celdas(true);
    for(int i=0; i<tam.x(); i++){
        for(int j=0; j<tam.y(); j++){
            celda_t celda;
            celda.col = j;
            celda.fila = i;
            celda.tipo = (*tablero)[i][j].getTipo();
            celda.color = (*tablero)[i][j].getColor();
            celdas += &celda;
        }
    }
    jugadores->enviarTablero(celdas);
}

void Nivel::jugar(){
    int puntaje_max = jugadores->verPuntajeMax();
    while(tablero->hayMovimientos() && (puntaje_max < puntaje_objetivo)){
//        tablero->imprimir();
        Jugada* nueva_jugada = jugadores->obtenerJugada();
        tablero->intercambiar(nueva_jugada);
        tablero->estabilizar();
        jugadores->sumarPuntos();
        puntaje_max = jugadores->verPuntajeMax();
        jugadores->encolarBorrados(tablero);
        jugadores->enviarPuntaje(nueva_jugada->verPuntos());
        jugadores->terminarJugada();
        nueva_jugada = NULL;
    }
}

void Nivel::cerrarJugador(){
    jugadores->cerrar();
}
