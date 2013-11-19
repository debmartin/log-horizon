#ifndef EMISOR_RESULTADOS_H
#define EMISOR_RESULTADOS_H

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

class EmisorResultados : public Thread{
  public:
    // Constructor de la clase.
    EmisorResultados();

    // Destructor de la clase
    ~EmisorResultados();

    //
    void agregar_socket(Socket *nuevo_socket);

    //
    void agregar_mutex(Mutex *nuevo_mutex);

    //
    void funcion_a_correr();

    //
    void encolar_dato(char tipo, dato_t &primer_dato, dato_t &segundo_dato);

    //
    void finalizar();

  private:
    Lista<resultado_t> *cola;
    Socket *socket;
    Mutex *mutex;
    bool seguir;
};

#endif // EMISOR_RESULTADOS_H
