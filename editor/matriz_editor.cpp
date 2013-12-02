#include "matriz_editor.h"

//
MatrizEditor::MatrizEditor(){
  matriz = NULL;
  columnas = 0;
  filas = 0;
  habilitada = new Textura;

}

//
MatrizEditor::~MatrizEditor(){
  delete habilitada;

  if (matriz){
    for (size_t x = 0; x < columnas; x++)
      delete[] matriz[x];
    delete[] matriz;
  }
}

//
bool MatrizEditor::inicializar(const std::string &path, unsigned int col, unsigned int fila, SDL_Rect &dest, Ventana *ventana){
  if (col == 0 || fila == 0) return false;
  columnas = col;
  filas = fila;
  std::cout << path << std::endl;
  habilitada->cargar_textura(path + "celda_fondo.png", ventana);

  
  matriz = new Textura**[columnas];
  for (unsigned int x = 0; x < columnas; x++){
    matriz[x] = new Textura*[filas];
    for (unsigned int y = 0; y < filas; y++){
      matriz[x][y] = habilitada;
    }
  }
  celda_inicial.x = dest.x;
  celda_inicial.y = dest.y;
  celda_inicial.w = dest.w / columnas;
  celda_inicial.h = dest.h / filas;
  return true;
}

//
bool MatrizEditor::dibujar(Ventana *ventana){
  if (columnas == 0 || filas == 0) return false;
  SDL_Rect actual = celda_inicial;
  for (unsigned int x = 0; x < columnas; x++){
    for (unsigned int y = 0; y < filas; y++){
      if (matriz[x][y]){
        actual.x = celda_inicial.x + x * actual.w;
        actual.y = celda_inicial.y + y * actual.h;
        matriz[x][y]->dibujar(actual, ventana);
      }
    }
  }
  return true;
}

//
bool MatrizEditor::analizar_evento(SDL_Event &evento, unsigned int &col, unsigned int &fila){
  if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT){
    if (evento.button.x >= celda_inicial.x && 
        evento.button.x <= celda_inicial.x + celda_inicial.w * (int)columnas &&
        evento.button.y >= celda_inicial.y &&
        evento.button.y <= celda_inicial.y + (int)filas * celda_inicial.h){
      col = (evento.button.x - celda_inicial.x) / celda_inicial.w;
      fila = (evento.button.y - celda_inicial.y) / celda_inicial.h;
      return true;
    }
  }
  return false;
}

//
bool MatrizEditor::insertar_textura(const std::string &path, unsigned int col, unsigned int fila, Ventana *ventana){
  if (col >= columnas || fila >= filas ) return false;
  Textura *aux = new Textura;
  aux->cargar_textura(path, ventana);
  matriz[col][fila] = aux;
  return true;
}

//
bool MatrizEditor::cambiar_estado(unsigned int col, unsigned int fila){
  if (col >= columnas || fila >= filas) return false;
  if (matriz[col][fila] == habilitada){
    matriz[col][fila] = NULL;
  }else if (!matriz[col][fila]){
    matriz[col][fila] = habilitada;
  }else{
    Textura *aux = matriz[col][fila];
    delete aux;
    matriz[col][fila] = habilitada;
  }
  return true;
}

//
bool MatrizEditor::celda_especial(unsigned int col, unsigned int fila){
  if (col >= columnas || fila >= filas) return false;
  if (matriz[col][fila] == habilitada || !matriz[col][fila]) return false;
  return true;
}
