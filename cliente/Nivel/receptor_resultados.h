#ifndef RECEPTOR_RESULTADOS_H
#define RECEPTOR_RESULTADOS_H

#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include "../../libs/TDA/thread/thread.h"
#include "../../libs/TDA/lista/lista.h"
#include <arpa/inet.h>

typedef struct resultado{
  char tipo;
  uint32_t mensaje;
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
    void agregar_socket(SocketPrefijo *nuevo_socket, int stop);
  
    // devuelve true si se esta recibiendo datos, caso contrario false
    bool recibiendo_datos();
    
    // devuelve true si no hay datos en la cola, caso contrario false
    bool cola_vacia();
    
    // Elimina el siguiente dato de la cola y almacena los datos
    // en los parametros ingresados y retorna el tipo de mensaje
    // desencolado
    char borrar_siguiente(uint32_t &mensaje);
    
    // Devuelve el tipo del siguiente mensaje
    char ver_siguiente();
    
    // Finaliza el hilo
    void finalizar();
    
  private:
    Lista<resultado_t> *cola;
    bool recibiendo;
    SocketPrefijo *socket;
    Mutex *mutex;
    bool seguir;
    int stop_tipo;
  protected:
    // Funcion a realizar en otro hilo, recibe datos que almacenara
    // en la lista, hasta recibir una señal de finalizacion por el metodo
    // "finalizar"
    void funcion_a_correr();
};

#endif // RECEPTOR_RESULTADOS_H
