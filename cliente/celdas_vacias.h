#ifndef CELDAS_VACIAS_H
#define CELDAS_VACIAS_H

#include "lista.h"
#include "matriz.h"

class ColumnaInvalida: public std::exception{};

typedef struct reemplzado{
  coordenada_t celda;
  int color;
  int tipo;
}reemplazo_t;

class CeldasVacias{
  public:
    //
    CeldasVacias();
    
    //
    ~CeldasVacias();
    
    //
    void inicializar(int cantidad_columnas);
    
    //
    void agregar(coordenada_t &celda);
    
    //
    bool esta_vacia(int columna);

    //
    coordenada_t borrar_proxima(int columna)throw(ListaVacia, ColumnaInvalida);
    
    //
    bool existentes();

  private:
    Lista<coordenada_t> **celdas_vacias;
    int columnas;  
    int contador;
};

#endif // CELDAS_VACIAS_H
