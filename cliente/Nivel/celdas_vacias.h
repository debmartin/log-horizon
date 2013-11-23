#ifndef CELDAS_VACIAS_H
#define CELDAS_VACIAS_H

#include "../../libs/TDA/lista/lista.h"
#include "matriz.h"

class ColumnaInvalida: public std::exception{};

typedef struct reemplzado{
  coordenada_t celda;
  int color;
  int tipo;
}reemplazo_t;

class CeldasVacias{
  public:
    // Constructor de clase
    CeldasVacias();
    
    // Destructor de clase
    ~CeldasVacias();
    
    // Inicializa la estructura, con los datos ingresados.
    void inicializar(int cantidad_columnas);
    
    // Agrega una coordenada que indica una celda vacia en el tablero,
    // y el tipo y color de la nueva pieza a ser apilada en la columna
    void agregar(coordenada_t &celda, int tipo, int color);
    
    // Devuelve true si no hay celdas vacias en la columna ingresada
    bool esta_vacia(int columna);

    // Elimina la proxima celda vacia correspondiente a la columna ingresada.
    // La proxima celda seria la de menor subindice
    reemplazo_t borrar_proxima(int columna)throw(ListaVacia, ColumnaInvalida);
    
    // Devuelve true si hay celdas vacias en el tablero, caso contrario
    // retorna false
    bool existentes();

  private:
    Lista<reemplazo_t> **celdas_vacias;
    int columnas;  
    int contador;
};

#endif // CELDAS_VACIAS_H
