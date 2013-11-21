#include "Nivel.h"
#define PUNT_INICIAL 0

#define PUERTO_ENVIAR 8000
#define PUERTO_RECIBIR 8001


using std::string;

Nivel::Nivel(Dimension& tam, char** estructura, int max_puntos) :
    puntaje_objetivo(max_puntos), puntaje_max(PUNT_INICIAL), jugador(NULL) {
    srand(time(NULL));
    tablero = new Tablero(tam, estructura);


}

Nivel::~Nivel(){
    delete tablero;
    if(jugador) delete jugador;
}

void Nivel::agregarJugador(string& nombre){
    jugador = new Jugador(nombre, PUERTO_ENVIAR, PUERTO_RECIBIR);
    jugador->prepararSocketEnviar();
    jugador->prepararSocketRecibir();
    jugador->enviarBorrados();
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}

void Nivel::enviarTablero(){
    Dimension tam = tablero->getTamanio();
    for(int i=0; i<tam.x(); i++){
        for(int j=0; j<tam.y(); j++){
            celda_t celda;
            celda.col = j;
            celda.fila = i;
            celda.tipo = (*tablero)[i][j].getTipo();
            celda.color = (*tablero)[i][j].getColor();
            jugador->enviarCelda(celda);
        }
    }
}

void Nivel::jugar(){
    while(tablero->hayMovimientos() && (puntaje_max < puntaje_objetivo)){
        tablero->imprimir();
        Jugada* nueva_jugada = jugador->obtenerJugada();
        tablero->intercambiar(nueva_jugada);
        tablero->estabilizar();
        jugador->sumarPuntos();
        int puntos_jug = jugador->getPuntaje();
        if (puntaje_max < puntos_jug)
            puntaje_max = puntos_jug;
        jugador->encolarBorrados(tablero);
        jugador->terminarJugada();
    }
    jugador->enviarPuntaje();
}

void Nivel::cerrarJugador(){
    jugador->cerrarJugador();
}
