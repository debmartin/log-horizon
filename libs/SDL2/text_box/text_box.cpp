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
  cantidad_caracteres = 0;
  oculta = false;
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
bool TextBox::asignar_fuente(std::string &path, int font_size, unsigned int dist_y, unsigned int cant_caracteres){
  dibujador_texto = new Texto;
  distancia_borde = dist_y;
  cantidad_caracteres = cant_caracteres;
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
  if (!destino) return;
  if (evento.type == SDL_MOUSEBUTTONDOWN){
    if (evento.button.x >= destino->x && evento.button.x <= destino->x + destino->w && evento.button.y >= destino->y && evento.button.y <= destino->y + destino->h){
      TextBox::activar();
    }else{
      TextBox::desactivar();
    }
  }else if (activada){
    if (evento.type == SDL_KEYDOWN && evento.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && cadena.length() > 0){
      cadena.erase(cadena.length() - 1, 1);
      mostrar.erase(mostrar.length() - 1, 1);
      mostrar_oculto.erase(mostrar.length() - 1, 1);
      if (cadena.length() >= cantidad_caracteres)
        this->borrar_primero_mostrar();
        
      modificacion = true;
    }else if (evento.type == SDL_TEXTINPUT){
      char c = *evento.text.text;
      cadena += c;
      this->append_mostrar(c);
      if (mostrar.length() > cantidad_caracteres){
        mostrar.erase(0,1);
        mostrar_oculto.erase(0,1);
      }
      modificacion = true; 
    }
  }
}

//
bool TextBox::dibujar(Ventana *ventana){
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
  destino_aux.w = ancho_caracter * (mostrar.length() + 1);
  if (modificacion){
    Superficie *sup = new Superficie;
    if (activada){
      mostrar += '|';
      mostrar_oculto += '|';
    }else{
      mostrar += ' ';
      mostrar_oculto += ' ';
    }
    if (oculta)
        dibujador_texto->copiar_texto(mostrar_oculto, sup);
    else
        dibujador_texto->copiar_texto(mostrar, sup);
    mostrar.erase(mostrar.length() - 1, 1);
    mostrar_oculto.erase(mostrar_oculto.length() - 1, 1);
    
    texto->cargar_textura(sup, ventana);
    delete sup;
    modificacion = false;
  }
  destino_aux.y = destino_aux.y + distancia_borde;
  destino_aux.h = destino_aux.h - distancia_borde * 2;
  return texto->dibujar(destino_aux, ventana);
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

//
void TextBox::append_mostrar(const char c){
  mostrar += c;
  mostrar_oculto += '*';
}

//
void TextBox::borrar_primero_mostrar(){
  mostrar.insert(0, 1, cadena[cadena.length() - cantidad_caracteres]);
  mostrar.insert(0,1,'*');
}

//
void TextBox::ocultar(){ 
  modificacion = true;
  oculta = true;
}

//
void TextBox::no_ocultar(){ 
  modificacion = true;
  oculta = false;
}
