#ifndef ARCHIVODIRECTO_H
#define ARCHIVODIRECTO_H

#include <string>
#include <fstream>

class ArchivoDirecto{
    public:
        ArchivoDirecto(std::string& ruta);
        virtual ~ArchivoDirecto();
        bool abrir();
        void cerrar();
        bool obtener(std::string& clave, std::string& dato);
        void agregar(std::string& clave, std::string& dato);
    protected:
    private:
        std::string* nombre;
        std::ifstream aux;
        std::istream* arch_abierto;
};

#endif // ARCHIVODIRECTO_H
