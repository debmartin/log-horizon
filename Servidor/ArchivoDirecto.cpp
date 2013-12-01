#include "ArchivoDirecto.h"
#define SEPARADOR ':'
#define LIBRE 'L'
#define OCUPADO 'O'
#define BORRADO 'B'

using std::string;

ArchivoDirecto::ArchivoDirecto(string& ruta){
    nombre = new string(ruta);
}

ArchivoDirecto::~ArchivoDirecto(){
    delete nombre;
}

bool ArchivoDirecto::abrir(){
    aux.open(nombre->c_str(), std::ifstream::in);
    if (! aux.is_open())
        return false;
    arch_abierto = &aux;
    return true;
}

void ArchivoDirecto::cerrar(){
    aux.close();
}

int funHash(string& cadena){
    return 0;
}

bool ArchivoDirecto::obtener(string& clave, string& dato){
    int pos_clave = funHash(clave);
    arch_abierto->seekg(pos_clave);
    string linea;
    getline(*arch_abierto, linea);

    if (linea[0] == LIBRE) return false;
    int pos_dato = linea.find_first_of(SEPARADOR) +1;
    string aux_clave =
    dato = linea.substr(pos_dato);
    return true;
}

void ArchivoDirecto::agregar(std::string& clave, std::string& dato){
}
