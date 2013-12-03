#include "checkbox_disjuntos.h"

//
CheckBoxDisjuntos(){
  activado = -1;
}

//
~CheckBoxDisjuntos(){
  for (size_t i = 0; i < vector.size(); i++){
    delete vector[i]
  }
  delete vector;
}

//
bool agregar_checkbox(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana){
  CheckBox *nuevo = new CheckBox;
  nuevo->asignar_texturas(path, estructura, ventana);
  vector->push_back(nuevo);
}

//
void dibujar(Ventana *ventana){
  for (size_t i = 0; i < vector.size(); i++){
    CheckBox *actual = vector[i];
    actual->dibujar(ventana);
  }
}

//
void analizar_evento(SDL_Event &evento){
  for (size_t i = 0; i < vector.size(); i++){
    CheckBox *actual = vector[i];
    actual->analizar_evento(evento);
    if (actual->activado() && i != activado){
      vector[activado]->desactivar();
      activado = i;
    }
  }
}
