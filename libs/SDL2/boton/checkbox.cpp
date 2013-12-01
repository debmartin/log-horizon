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

/*

//
Boton::Boton(){
  textura = new Textura;
  texto = new Textura;
  apretado = false;
}

//
Boton::~Boton(){
  delete textura;
  delete texto;
}



//
bool Boton::agregar_texto(Superficie *sup, SDL_Rect &dest_texto, Ventana *ventana, int desplazamiento){
  if (!sup || !ventana) return false;
  texto->cargar_textura(sup, ventana);
  destino_texto = dest_texto;
  desp = desplazamiento;
  return true;
}

//
bool Boton::dibujar(Ventana *ventana){
  if (!textura->dibujar(src_actual, destino, ventana)) return false;
  if (texto){
    SDL_Rect destino_aux = destino_texto;
    if (src_actual.x == src_apretado.x && src_actual.y == src_apretado.y){
      destino_aux.x = destino_aux.x + desp;
      destino_aux.y = destino_aux.y + desp;
    }
    if (!texto->dibujar(destino_aux, ventana)) return false;
  }
  return true;
}

//
void Boton::establecer_alpha(Uint8 alpha){
  textura->establecer_alpha(alpha);
  texto->establecer_alpha(alpha);
}
*/
