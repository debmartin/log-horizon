#include <iostream>

#include "boton.h"

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
bool Boton::asignar_texturas(Superficie *sup, estructura_boton_t &estructura, Ventana *ventana){
  if (!sup || !ventana) return false;
  textura->cargar_textura(sup, ventana);
  src_normal = estructura.normal;
  src_actual = src_normal;
  src_resaltado = estructura.resaltado;
  src_apretado = estructura.apretado;
  destino = estructura.destino;
  return true;
}

//
bool Boton::asignar_texturas(const std::string &path, estructura_boton_t &estructura, Ventana *ventana){
  if (!ventana) return false;
  Superficie sup;
  sup.cargar(path);

  SDL_Color color;
  sup.color_pixel(0, 0, color);
  sup.color_clave(color);

  return Boton::asignar_texturas(&sup, estructura, ventana);
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
void Boton::analizar_evento(SDL_Event &evento){
  if (evento.type != SDL_MOUSEMOTION && 
      ((evento.type != SDL_MOUSEBUTTONDOWN && 
       evento.type != SDL_MOUSEBUTTONUP) ||
       evento.button.button != SDL_BUTTON_LEFT))
    return;
  if (evento.button.x >= destino.x && 
      evento.button.x <= destino.x + destino.w &&
      evento.button.y >= destino.y &&
      evento.button.y <= destino.y + destino.h){
    if (evento.type == SDL_MOUSEBUTTONDOWN){
      src_actual = src_apretado;
      apretado = true;
    }else if(evento.type == SDL_MOUSEBUTTONUP){
      apretado = false;
    }
    if(!apretado){
      src_actual = src_resaltado;
    }
  }else{
    src_actual = src_normal;
    apretado = false;
  }
}

//
bool Boton::dibujar(Ventana *ventana){
  if (!textura->dibujar(src_actual, destino, ventana)) return false;
  if (texto){
    SDL_Rect destino_aux = destino_texto;
    if (apretado){
      destino_aux.x = destino_aux.x + desp;
      destino_aux.y = destino_aux.y + desp;
    }
    if (!texto->dibujar(destino_aux, ventana)) return false;
  }
  return true;
}
