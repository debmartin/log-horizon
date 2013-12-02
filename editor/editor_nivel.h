#ifndef EDITOR_NIVEL_H
#define EDITOR_NIVEL_H

#include "../libs/SDL2/SDL.h"
#include "matriz_editor.h"
#include "../libs/json/include/json/json.h"

typedef struct checkbox_nombres{
  CheckBoxDisjuntos *checkbox;
  std::vector<std::string> *nombres;
}checkbox_nombres_t;

class EditorNivel{
  public:
    //
    EditorNivel();
  
    //
    virtual ~EditorNivel();
    
    //
    bool correr(Ventana *ventana, const std::string &path_nivel);
    
    //
    int inicializar(const std::string &path, unsigned int columnas, unsigned int filas, Ventana *ventana);
  
  private:
    bool datos_inicializados;
    Textura *fondo;
    Boton *cambiar_fondo;
    Boton *cant_jugadores;
    Boton *puntaje_ganar;
    Boton *enviar_nivel;
    Texto *escritor;
    TextBox *entrada;
    Mensaje *mensaje;
    SDL_Rect destino_mensaje;
    std::string nivel_path;
    Ventana *ventana;
    CheckBoxDisjuntos *accion;
    checkbox_nombres_t *tipos;
    checkbox_nombres_t *colores;
    MatrizEditor *matriz;
    Json::Value probabilidades;
    
  protected:
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void copiar_archivo(const std::string &path_src, const std::string &path_dest);
    
    //
    void cargar_fondo();
    
    //
    void accion_agujero(unsigned int col, unsigned int fila);
    
    //
    void cambiar_fondo_celda(unsigned int col, unsigned int fila);
};

#endif // EDITOR_NIVEL_H
