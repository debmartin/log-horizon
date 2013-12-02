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
    bool inicializar(const std::string &path, unsigned int col, unsigned int fila, SDL_Rect &dest, Ventana *ventana);
  
    //
    bool dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento, unsigned int &col, unsigned int &fila);
    
    //
    bool insertar_textura(const std::string &path, unsigned int col, unsigned int fila, Ventana *ventana);
    
    //
    bool cambiar_estado(unsigned int col, unsigned int fila);
    
    //
    bool celda_especial(unsigned int col, unsigned int fila);
    
  private:
    Textura *habilitada;
    Textura ***matriz;
    unsigned int columnas;
    unsigned int filas;
    SDL_Rect celda_inicial;
};


#endif // MATRIZ_EDITOR_H
