#ifndef MATRIZ_EDITOR_H
#define MATRIZ_EDITOR_H

#include "../libs/SDL2/SDL.h"

class MatrizEditor{
  public:
    //
    MatrizEditor();
    
    //
    ~MatrizEditor();
 
    //
    bool inicializar(const std::string &path, size_t col, size_t fila, SDL_Rect &dest, Ventana *ventana);
  
    //
    bool dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento, size_t &col, size_t &fila);
    
    //
    bool insertar_textura(const std::string &path, size_t col, size_t fila, Ventana *ventana);
    
    //
    bool cambiar_estado(size_t col, size_t fila);
    
  private:
    Textura *habilitada;
    Textura ***matriz;
    size_t columnas;
    size_t filas;
    SDL_Rect celda_inicial;
};


#endif // MATRIZ_EDITOR_H
