#include "checkbox_disjuntos.h"

//
CheckBoxDisjuntos::CheckBoxDisjuntos(){
  activado = -1;
}

//
CheckBoxDisjuntos::~CheckBoxDisjuntos(){
  for (size_t i = 0; i < vector.size(); i++){
    delete vector[i];
  }
}

//
void CheckBoxDisjuntos::agregar_checkbox(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana){
  CheckBox *nuevo = new CheckBox;
  nuevo->asignar_texturas(path, estructura, ventana);
  vector.push_back(nuevo);
}

//
void CheckBoxDisjuntos::dibujar(Ventana *ventana){
  for (size_t i = 0; i < vector.size(); i++){
    CheckBox *actual = vector[i];
    actual->dibujar(ventana);
  }
}

//
void CheckBoxDisjuntos::analizar_evento(SDL_Event &evento){
  for (int i = 0; i < (int)vector.size(); i++){
    CheckBox *actual = vector[i];
    actual->analizar_evento(evento);
    if (actual->activado() && i != activado){
      if (activado != -1) vector[activado]->desactivar();
      activado = i;
    }else if (activado != -1 && !vector[activado]->activado()){
      activado = -1;
    }
  }
}

//
bool CheckBoxDisjuntos::agregar_texto(int subindice, Superficie *sup, SDL_Rect &destino, Ventana *ventana){
  if (subindice <  0 || subindice >= (int) vector.size() ) return false;
  vector[subindice]->agregar_texto(sup, destino, ventana, 0);
  return true;
}
