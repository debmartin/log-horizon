#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread{
  public:
    // Constructor de la clase
    Thread(){}
    
    // Destructor de la clase
    virtual ~Thread(){}
    
    // Corre la funcion a correr del hilo.
    // Pre: Se debio definir la funcion_a_correr.
    // Post: Se ejecuta la funcion_a_correr previamente definida
    bool correr(){
      return (pthread_create(&hilo, NULL, funcion_interna, this) == 0);
    }
    
    // Deja al hilo principal a la espera, hasta que el thread 
    // termine de ejecutarse.
    void join(void** resultado = NULL);
    
  protected:
    // Funcion virtual pura, debe ser definida al heredarse la clase.
    virtual void funcion_a_correr() = 0;
  
  private:
    pthread_t hilo;
    
    // Funcion auxiliar, que ejecuta la funcion_a_correr.
    static void *funcion_interna(void *This){
      ((Thread*)This)->funcion_a_correr();
      return NULL;
    }
};

class Mutex{
  public:
    // Constructor de la clase
    Mutex();
    
    // Destructor de la clase
    ~Mutex();
    
    // Bloquea el mutex.
    int bloquear();
    
    // Desbloquea el mutex.
    int desbloquear();
    
    // Intenta bloquear el mutex.
    int intentar_bloquear();
  private:
    pthread_mutex_t mutex;
};

#endif // THREAD_H
