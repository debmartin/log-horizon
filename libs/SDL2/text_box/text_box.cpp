#include <iostream>

#include "text_box.h"

//
TextBox::TextBox(){
  fondo = new Textura;
  texto = new Textura;
  dibujador_texto = NULL;
  origen = NULL;
  cadena = "";
  activada = false;
  modificacion = true;
  destino = NULL;
  oculto = false;
}

//
TextBox::~TextBox(){
  if (dibujador_texto) delete dibujador_texto;
  if (origen) delete origen;
  if (destino) delete destino;
  delete fondo;
  delete texto;
}

//
bool TextBox::asignar_fuente(std::string &path, int font_size, unsigned int dist_y){
  dibujador_texto = new Texto;
  distancia_borde = dist_y;
  return dibujador_texto->asignar_fuente(path, font_size);
}

//
bool TextBox::asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha){
  if(!dibujador_texto) return false;
  dibujador_texto->asignar_color(r, g, b, alpha);
  modificacion = true;
  return true;
}

//
void TextBox::asignar_fondo(SDL_Rect &seleccion, Superficie *sup, Ventana *ventana){
  TextBox::asignar_fondo(sup, ventana);
  origen = new SDL_Rect;
  *origen = seleccion;
}

//
void TextBox::asignar_fondo(Superficie *sup, Ventana *ventana){
  fondo->cargar_textura(sup, ventana);
}

//
void TextBox::asignar_fondo(const std::string &path, Ventana *ventana){
  Superficie sup;
  sup.cargar(path);
  SDL_Color color;
  sup.color_pixel(0,0, color);
  sup.color_clave(color);
  fondo->cargar_textura(&sup, ventana);
}

//
void TextBox::alpha_fondo(Uint8 alpha){
  fondo->establecer_alpha(alpha);
}

//
void TextBox::asignar_destino(SDL_Rect &dest){
  destino = new SDL_Rect;
  *destino = dest;
}

//
void TextBox::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_MOUSEBUTTONDOWN){
    if (evento.button.x >= destino->x && evento.button.x <= destino->x + destino->w &&
        evento.button.y >= destino->y && evento.button.y <= destino->y + destino->h){
      TextBox::activar();
    }else{
      TextBox::desactivar();
    }
  }else if (activada){
    if (evento.type == SDL_KEYDOWN && evento.key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
      cadena = cadena.substr(0, cadena.length() - 1);
      modificacion = true;
    }else if (evento.type == SDL_TEXTINPUT){
      char c = *evento.text.text;
      cadena += c;
      modificacion = true; 
    }
  }
}

//
bool TextBox::dibujar(unsigned int cantidad_caracteres, Ventana *ventana){
  if (!dibujador_texto || !destino) return false;
  // dibujo del fondo
  if (fondo && origen && !fondo->dibujar(*origen, *destino, ventana)){
    return false;
  }else if(fondo && !fondo->dibujar(*destino, ventana)){
    return false;
  }
  // dibujo de los caracteres
  SDL_Rect destino_aux = *destino;
  int ancho_caracter = destino->w / (cantidad_caracteres + 1);
  if (cadena.length() <= cantidad_caracteres){
    destino_aux.w = ancho_caracter * (cadena.length() + 1);
  }else{
    destino_aux.w = ancho_caracter * (cantidad_caracteres + 1);
  }
  if (modificacion){
    Superficie *sup = new Superficie;
    std::string mostrar;
    
    if (!oculto){
      mostrar= cadena;
      if (cadena.length() > cantidad_caracteres)
        mostrar = cadena.substr(cadena.length() - cantidad_caracteres, cadena.length());
    }else{
      std::string aux = "";
      for (unsigned int i = 0; i < cadena.length(); i++){
        aux += '*';
      }
      mostrar = aux;
      if (cadena.length() > cantidad_caracteres)
        mostrar = aux.substr(cadena.length() - cantidad_caracteres, cadena.length());
    }
    
    if (activada){
      mostrar += '|';
    }else{
      mostrar += ' ';
    }
    
    dibujador_texto->copiar_texto(mostrar, sup);
    
    texto->cargar_textura(sup, ventana);
    delete sup;
    modificacion = false;
  }
  destino_aux.y = destino_aux.y + distancia_borde;
  destino_aux.h = destino_aux.h - distancia_borde * 2;
  return texto->dibujar(destino_aux, ventana);
}

//
void TextBox::ocultar_texto(){
  oculto = true;
}

//
void TextBox::desocultar_texto(){
  oculto = false;
}

//
void TextBox::activar(){
  activada = true;
  modificacion = true;
}

//
void TextBox::desactivar(){
  activada = false;
  modificacion = true;
}

//
bool TextBox::esta_activada(){
  return activada;
}
