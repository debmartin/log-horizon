#include "funciones_auxiliares.h"
#include <cstring>
#define TAM_UINT32 sizeof(uint32_t)
#define TAM_BUFFER 30


using std::string;

bool recibirMsjPrefijo(Socket& sock_cli, string& mensaje){
    char prefijo[TAM_UINT32];

    sock_cli.recibir(prefijo, TAM_UINT32);

    uint32_t tam;
    memcpy(&tam, prefijo, TAM_UINT32);

    int largo = ntohl(tam), total = 0;
    char datos[TAM_BUFFER];
    while (total < largo) {
        int recibidos = sock_cli.recibir(datos, TAM_BUFFER-1);
        if (recibidos == -1) break;
        if (recibidos == 0) return false;
        datos[recibidos] = '\0';
        mensaje += datos;
        total += recibidos;
    }
}

void enviarMsjPrefijo(Socket& sock_cli, const void* mensaje, int len_msj){
    int largo = len_msj + TAM_UINT32;
    char* msj = new char[largo];
    uint32_t len = htonl((uint32_t) len_msj);
    memcpy(msj, &len, TAM_UINT32);
    memcpy(msj + TAM_UINT32, mensaje, len_msj);

    int enviados = 0;
    while (enviados < largo) {
        int cant = sock_cli.enviar(msj + enviados, largo - enviados);
        if (cant == -1) break;
        enviados += cant;
    }
    delete msj;
}
