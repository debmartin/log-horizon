#include <iostream>

#include "text_box_oculta.h"

//
void TextBoxOculta::append_mostrar(const char c){
  mostrar += '*';
}

//
void TextBoxOculta::borrar_primero_mostrar(){
  mostrar.insert(0, 1, '*');
}
