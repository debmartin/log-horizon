#include <iostream>

#include <string.h>
#include <stdlib.h>
#include "Juego.h"
#define SEPARADORES ",\n"

enum Celdas {BU_ROJO, BU_AMAR, BU_VERDE, BU_AZUL, BU_VIOLETA, BAR_ROJO, BAR_AMAR, BAR_VERDE, BAR_AZUL, BAR_VIOLETA, STAR};

using std::string;


void capturar_entrada(int& i, int& j){
    string entrada;
    std::cin >> entrada;
    i = atoi(entrada.c_str());
    std::cin >> entrada;
    j = atoi(entrada.c_str());
}

int main(){
    Juego juego;
    int i, j, puntaje_nivel = juego.getLimite();

    while (juego.hayMovimientos() && (juego.getPuntaje() < puntaje_nivel)){
        juego.imprimirTablero();
        capturar_entrada(i, j);
        Dimension una(i, j);
        capturar_entrada(i, j);
        Dimension otra(i, j);

        int res = juego.realizarJugada(una, otra);
        if (res == 2) {
            std::cout << "Inválida\n";
            continue;
        } else if (res == 1){
            std::cout << "No adyacentes\n";
            continue;
        }
    }
    return 0;
}
