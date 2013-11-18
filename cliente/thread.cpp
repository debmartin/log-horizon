#include "server-thread.h"
#include <pthread.h>

// Deja al hilo principal a la espera, hasta que el thread 
// termine de ejecutarse.
void Thread::join(void** resultado) {
	pthread_join(hilo,resultado);
}

// Constructor de la clase
Mutex::Mutex() {
    pthread_mutex_init(&mutex, NULL);
}

// Destructor de la clase
Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

// Bloquea el mutex.
int Mutex::bloquear() {
    return pthread_mutex_lock(&mutex);
}

// Desbloquea el mutex.
int Mutex::desbloquear() {
    return pthread_mutex_unlock(&mutex);
}

// Intenta bloquear el mutex.
int Mutex::intentar_bloquear() {
    return pthread_mutex_trylock(&mutex);
}
