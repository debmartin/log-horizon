#include "Nivel.h"
#define PUNT_INICIAL 0

#define PUERTO_ENVIAR 8000
#define PUERTO_RECIBIR 8001


using std::string;

Nivel::Nivel(Dimension& tam, char** estructura, int max_puntos) :
    puntaje_objetivo(max_puntos), puntaje_max(PUNT_INICIAL), datos_jug(NULL) {
    tablero = new Tablero(tam, estructura);


}

Nivel::~Nivel(){
    delete tablero;
    if(datos_jug) delete datos_jug;
}

void Nivel::agregarJugador(string& nombre){
    datos_jug = new DatosJugador(PUERTO_ENVIAR, PUERTO_RECIBIR);
    datos_jug->prepararSocketEnviar();
    datos_jug->prepararSocketRecibir();
    datos_jug->crearJugador(nombre);
    datos_jug->enviarBorrados();
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}

void Nivel::jugar(){
    while(tablero->hayMovimientos() && (puntaje_max < puntaje_objetivo)){
        Jugada* nueva_jugada = datos_jug->obtenerJugada();
        tablero->intercambiar(nueva_jugada);
        tablero->estabilizar();
        datos_jug->sumarPuntos();
        int puntos_jug = datos_jug->getPuntaje();
        if (puntaje_max < puntos_jug)
            puntaje_max = puntos_jug;
        datos_jug->encolarBorrados(tablero);
        datos_jug->terminarJugada();
    }
}
