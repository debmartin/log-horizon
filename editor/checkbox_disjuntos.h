#ifndef CHECKBOX_DISJUNTOS
#define CHECKBOX_DISJUNTOS

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
    
  private:
    std::vector<CheckBox> lista_checkbox;
    int activado;
}

#endif // CHECKBOX_DISJUNTOS
