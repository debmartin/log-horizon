#ifndef RECEPTOR_RESULTADOS_H
#define RECEPTOR_RESULTADOS_H

#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/thread/thread.h"
#include "../../libs/TDA/lista/lista.h"

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
    
    // Asigna un socket para poder recibir los datos que seran
    // encolados.
    // Pre: el socket debe existir
    void agregar_socket(Socket *nuevo_socket, int stop);
    
    // Funcion a realizar en otro hilo, recibe datos que almacenara
    // en la lista, hasta recibir una señal de finalizacion por el metodo
    // "finalizar"
    void funcion_a_correr();
  
    // devuelve true si se esta recibiendo datos, caso contrario false
    bool recibiendo_datos();
    
    // devuelve true si no hay datos en la cola, caso contrario false
    bool cola_vacia();
    
    // Elimina el siguiente dato de la cola y almacena los datos
    // en los parametros ingresados y retorna el tipo de mensaje
    // desencolado
    char borrar_siguiente(dato_t &primer_dato, dato_t &segundo_dato);
    
    // Devuelve el tipo del siguiente mensaje
    char ver_siguiente();
    
    // Finaliza el hilo
    void finalizar();
    
  private:
    Lista<resultado_t> *cola;
    bool recibiendo;
    Socket *socket;
    Mutex *mutex;
    bool seguir;
    int stop_tipo;
};

#endif // RECEPTOR_RESULTADOS_H
