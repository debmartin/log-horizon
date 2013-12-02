#ifndef CHECKBOX_DISJUNTOS_H
#define CHECKBOX_DISJUNTOS_H

#include "../libs/SDL2/SDL.h"

class CheckBoxDisjuntos{
  public:
    //
    CheckBoxDisjuntos();
    
    //
    ~CheckBoxDisjuntos();
    
    //
    void agregar_checkbox(Ceckbox &box);
    
    //
    void analizar_evento(SDL_Event &evento);
    
    //
    void dibujar(Ventana *ventana);
    
  private:
    std::vector<CheckBox> *lista_checkbox;
    int activado;
}

#endif // CHECKBOX_DISJUNTOS_H
