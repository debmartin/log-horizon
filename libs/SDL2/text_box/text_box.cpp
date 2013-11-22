#include <iostream>

#include "text_box.h"

//
TextBox::TextBox(){
  fondo = NULL;
  origen = NULL;
  cadena = "";
  activada = false;
  texto = new Textura;
  modificacion = true;
}

//
TextBox::~TextBox(){
  if (dibujador_texto) delete dibujador_texto;
  delete texto;
}

//
bool TextBox::asignar_fuente(std::string &path, int font_size){
  dibujador_texto = new Texto;
  return dibujador_texto->asignar_fuente(path, font_size);
}

//
bool TextBox::asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha){
  if(!dibujador_texto) return false;
  dibujador_texto->asignar_color(r, g, b, alpha);
  return true;
}

//
void TextBox::asignar_fondo(SDL_Rect &seleccion, Textura *fondo_nuevo){
  TextBox::asignar_fondo(fondo_nuevo);
  *origen = seleccion;
}

//
void TextBox::asignar_fondo(Textura *fondo_nuevo){
  fondo = fondo_nuevo;
}

//
void TextBox::analizar_evento(SDL_Event &evento){
  if (!activada) return;
  if (evento.type == SDL_KEYDOWN && evento.key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
    cadena = cadena.substr(0, cadena.length() - 1);
    modificacion = true;
  }else if (evento.type == SDL_TEXTINPUT){
    char c = *evento.text.text;
    cadena += c;
    modificacion = true; 
  }
}

//
bool TextBox::dibujar(unsigned int cantidad_caracteres, SDL_Rect &destino, Ventana *ventana){
  if (!dibujador_texto) return false;
  // dibujo del fondo
  if (fondo && origen && !fondo->dibujar(*origen, destino, ventana)){
    return false;
  }else if(fondo && !fondo->dibujar(destino, ventana)){
    return false;
  }
  if (cadena.length() == 0) return true;
  // dibujo de los caracteres
  SDL_Rect destino_aux = destino;
  int ancho_caracter = destino.w / cantidad_caracteres;
  if (cadena.length() < cantidad_caracteres)
    destino_aux.w = ancho_caracter * cadena.length();
  if (modificacion){
    Superficie *sup = new Superficie;
    std::string mostrar = cadena;
    if (cadena.length() > cantidad_caracteres)
      mostrar = cadena.substr(cadena.length() - cantidad_caracteres, cadena.length());
    dibujador_texto->copiar_texto(mostrar, sup);
    
    texto->cargar_textura(sup, ventana);
    delete sup;
    modificacion = false;
  }
  return texto->dibujar(destino_aux, ventana);
}

//
void TextBox::activar(){
  activada = true;
}

//
void TextBox::desactivar(){
  activada = false;
}

//
bool TextBox::esta_activada(){
  return activada;
}
