#include "Nivel.h"


using std::string;
using std::ifstream;

Nivel::Nivel(ifstream& arch_estructura, ifstream& arch_probabilidades, int max_puntos) :
    puntaje_objetivo(max_puntos) {
    srand(time(NULL));

    Json::Value valores;
    Json::Value aux;
    Json::Reader reader;

    if (!reader.parse(arch_estructura, valores, false))
        throw NivelJsonError();
    aux = valores.get("filas", aux);
    int filas = aux.asInt();
    aux = valores.get("columnas", aux);
    int columnas = aux.asInt();
    Dimension tam(filas, columnas);

    aux = valores.get("matriz", aux);
    Matriz<int> estructura(tam);
    for (int x = 0; x < columnas; x++){
        for (int y = 0; y < filas; y++){
            estructura[x][y] = aux[x][y].asInt();
        }
    }
    tablero = new Tablero(tam);
    tablero->establecerProbabilidades(estructura, arch_probabilidades);
    mutex_recibir = new Mutex();
    jugadores = new ConjuntoJugadores(mutex_recibir);
}

Nivel::~Nivel(){
    delete tablero;
    delete jugadores;
    delete mutex_recibir;
}

void Nivel::agregarJugador(usuario_t& usuario){
    jugadores->agregar(usuario);
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
    jugadores->enviarGanador(puntaje_max >= puntaje_objetivo);
}

void Nivel::cerrarJugador(){
    jugadores->cerrar();
}
