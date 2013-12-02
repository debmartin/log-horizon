#include "ArchivoDirecto.h"
#include <cstring>
#include <sstream>

#define SEPARADOR '~'
#define LIBRE 'L'
#define OCUPADO 'O'
#define BORRADO 'B'
#define TAM_BUFFER 30
#define TAM_LEN sizeof(int)
#define SIZE_CHAR sizeof(char)
using std::string;
using std::ifstream;
using std::ios;

ArchivoDirecto::ArchivoDirecto(string& ruta) : largo(-1){
    nombre = new string(ruta);
}

ArchivoDirecto::~ArchivoDirecto(){
    delete nombre;
}

bool ArchivoDirecto::abrir(){
    istream.open(nombre->c_str(), std::ifstream::binary);
    ostream.open(nombre->c_str(), std::ifstream::binary);
    istream.seekg(0, ios::end);
    largo = istream.tellg();
    return (istream.is_open() && ostream.is_open());
}

void ArchivoDirecto::cerrar(){
    istream.close();
    ostream.close();
}

int funHash(const std::string clave, int tamanio){
    int hash = 0, largo = clave.length();

    for (int i=0; i < largo; i++){
        hash = clave[i] + (hash << 6) + (hash << 8) - hash;
    }
    return hash % tamanio;
}

bool ArchivoDirecto::obtener(string& clave, string& dato){
    int pos_clave = funHash(clave, largo);

    istream.seekg(pos_clave, ios::beg);
    string clave_aux, dato_aux;
    bool encontrado = false;
    char estado;
    do {
        estado = obtenerLinea(clave_aux, dato_aux);
        encontrado = (clave_aux == clave);
    } while(istream.good() && !encontrado && (LIBRE != estado));

    if(encontrado) dato = dato_aux;
    return encontrado;
}

char ArchivoDirecto::obtenerLinea(string& clave, string& dato){
    char estado;
    istream.read(&estado, SIZE_CHAR);
    if(estado != OCUPADO) return estado;

    char buffer[TAM_BUFFER +1];
    buffer[TAM_BUFFER] = '\0';

    buffer[TAM_LEN] = '\0';
    istream.read(buffer, TAM_LEN * SIZE_CHAR);
    int len_linea;
    memcpy(&len_linea, buffer, TAM_LEN);

    int leidos = 0;
    string nomb_cont;
    while(leidos < len_linea){
        istream.read(buffer, TAM_BUFFER * SIZE_CHAR);
        nomb_cont += buffer;
        leidos += istream.gcount();
    }
    int pos_separador = nomb_cont.find_first_of(SEPARADOR);
    dato = nomb_cont.substr(pos_separador +1);
    clave = nomb_cont.substr(0, pos_separador);

    return estado;
}

bool ArchivoDirecto::agregar(string& clave, string& dato){
    int len = clave.length() + dato.length() + 1, len_linea;

    int pos = funHash(clave, largo);
    istream.seekg(pos, ios::beg);

    string clave_aux, dato_aux;
    bool encontrado;
    char estado;
    do {
        estado = obtenerLinea(clave_aux, dato_aux);
        encontrado = (clave_aux == clave);
    } while(istream.good() && !encontrado && (OCUPADO == estado));
    if (encontrado) return false;

    pos = istream.tellg();
    ostream.seekp(pos-1);

    char ocupado = OCUPADO;
    ostream.write(&ocupado, SIZE_CHAR);

    std::stringstream ss;
    ss << len;
    ostream.write(ss.str().c_str(), ss.str().length());

    string entrada(clave);
    entrada += SEPARADOR;
    entrada += dato;
    ostream.write(entrada.c_str(), len);

    istream.seekg(0, ios::end);
    largo = istream.tellg();
    return true;
}
