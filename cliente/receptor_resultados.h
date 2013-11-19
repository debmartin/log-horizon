#ifndef RECEPTOR_RESULTADOS_H
#define RECEPTOR_RESULTADOS_H

#include "socket.h"
#include "thread.h"
#include "lista.h"

typedef struct dato{
  char valor1;
  char valor2;
}dato_t;

typedef struct resultado{
  char tipo;
  dato_t primero;
  dato_t segundo;
}resultado_t;

class ReceptorResultados : public Thread{
  public:
    // Constructor de la clase.
    ReceptorResultados();
    
    // Destructor de la clase
    ~ReceptorResultados();
    
    //
    void agregar_socket(Socket *nuevo_socket);
    
    //
    void agregar_mutex(Mutex *nuevo_mutex);

    //
    void funcion_a_correr();
  
    //
    bool recibiendo_datos();
    
    //
    bool cola_vacia();
    
    //
    char borrar_siguiente(dato_t &primer_dato, dato_t &segundo_dato);
    
    //
    void finalizar();
    
  private:
    Lista<resultado_t> *cola;
    bool recibiendo;
    Socket *socket;
    Mutex *mutex;
    bool seguir;
};

#endif // RECEPTOR_RESULTADOS_H
