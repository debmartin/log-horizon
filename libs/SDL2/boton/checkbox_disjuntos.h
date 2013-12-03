#ifndef CHECKBOX_DISJUNTOS_H
#define CHECKBOX_DISJUNTOS_H

#include "checkbox.h"
#include <vector>

class CheckBoxDisjuntos{
  public:
    //
    CheckBoxDisjuntos();
    
    //
    ~CheckBoxDisjuntos();
  
    //
    void agregar_checkbox(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana);
    
    //
    void dibujar(Ventana *ventana);
    
    //
    void analizar_evento(SDL_Event &evento);
  
    //
    int subindice_activado(){return activado;}
  
    //
    bool agregar_texto(int subindice, Superficie *sup, SDL_Rect &destino, Ventana *ventana);
  
  private:
    std::vector<CheckBox*> vector;
    int activado;
};

#endif // CHECKBOX_DISJUNTOS_H
