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
        bool agregar(std::string& clave, std::string& dato);
    protected:
        char obtenerLinea(std::string& clave, std::string& dato);
    private:
        std::string* nombre;
        std::ifstream istream;
        std::ofstream ostream;
        int largo;
};

#endif // ARCHIVODIRECTO_H
