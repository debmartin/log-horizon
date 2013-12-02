#ifndef EDITOR_NIVEL_H
#define EDITOR_NIVEL_H

#include "../libs/SDL2/SDL.h"
#include "matriz_editor.h"


class EditorNivel{
  public:
    //
    EditorNivel();
  
    //
    virtual ~EditorNivel();
    
    //
    bool correr(Ventana *ventana, const std::string &path_nivel);
    
    //
    int inicializar(const std::string &path, size_t columnas, size_t filas, Ventana *ventana);
  
  private:
    bool datos_inicializados;
    Textura *fondo;
    Boton *cambiar_fondo;
    Texto *escritor;
    TextBox *entrada;
    Mensaje *mensaje;
    SDL_Rect destino_mensaje;
    std::string nivel_path;
    Ventana *ventana;
    
    CheckBoxDisjuntos *accion;
    
    MatrizEditor *matriz;
    
  protected:
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void copiar_archivo(const std::string &path_src, const std::string &path_dest);
    
    //
    void cargar_fondo();
};

#endif // EDITOR_NIVEL_H
