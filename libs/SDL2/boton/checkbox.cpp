#include <iostream>

#include "checkbox.h"

//
void CheckBox::analizar_evento(SDL_Event &evento){
  if (evento.type != SDL_MOUSEMOTION && 
      (evento.type != SDL_MOUSEBUTTONDOWN ||
       evento.button.button != SDL_BUTTON_LEFT))
    return;
  if (evento.button.x >= destino.x && 
      evento.button.x <= destino.x + destino.w &&
      evento.button.y >= destino.y &&
      evento.button.y <= destino.y + destino.h){
    if (evento.type == SDL_MOUSEBUTTONDOWN){
      if (apretado){
        src_actual = src_resaltado;
        apretado = false;
      }else{
        src_actual = src_apretado_resaltado;
        apretado = true;
      }
    }else if(apretado){
      src_actual = src_apretado_resaltado;
    }else{
      src_actual = src_resaltado;
    }
  }else if (!apretado){
    src_actual = src_normal;
  }else{
    src_actual = src_apretado;
  }
}


//
bool CheckBox::asignar_texturas(Superficie *sup, estructura_checkbox_t &estructura, Ventana *ventana){
  if (!sup || !ventana) return false;
  textura->cargar_textura(sup, ventana);
  src_normal = estructura.normal;
  src_actual = src_normal;
  src_resaltado = estructura.resaltado;
  src_apretado = estructura.apretado;
  destino = estructura.destino;
  src_apretado_resaltado = estructura.resaltado_apretado;
  return true;
}

//
bool CheckBox::asignar_texturas(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana){
  if (!ventana) return false;
  Superficie sup;
  sup.cargar(path);

  SDL_Color color;
  sup.color_pixel(0, 0, color);
  sup.color_clave(color);

  return this->asignar_texturas(&sup, estructura, ventana);
}

//
void CheckBox::desactivar(){
  apretado = false;
  src_actual = src_normal;
}
