#ifndef CHECKBOX_DISJUNTOS_H
#define CHECKBOX_DISJUNTOS_H

#include "checkbox.h"

class CheckBoxDisjuntos{
  public:
    //
    CheckBoxDisjuntos();
    
    //
    ~CheckBoxDisjuntos();
  
    //
    bool agregar_checkbox(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana);
    
    //
    void dibujar(Ventana *ventana);
    
    //
    void analizar_evento(SDL_Event &evento);
  
    //
    int subindice_activado(){return activado;}
  
  private:
    std::vector<CheckBox*> vector;
    int activado;
};

#endif // CHECKBOX_DISJUNTOS_H
