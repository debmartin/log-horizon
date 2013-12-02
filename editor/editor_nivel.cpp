#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../libs/funciones_auxiliares.h"
#include "editor_nivel.h"

#define FONDO_TEXTO 175
#define ALPHA_MENSAJE 255
#define DELAY 16

#define AGUJEROS 0
#define IMAGEN_CELDA 1
#define PROB_CELDA 2
#define PROB_COL 3

//
EditorNivel::EditorNivel(){
  fondo = new Textura;
  cambiar_fondo = new Boton;
  cant_jugadores = new Boton;
  puntaje_ganar = new Boton;
  escritor = new Texto;
  mensaje = new Mensaje;
  entrada = new TextBox;
  datos_inicializados = false;
  matriz = new MatrizEditor;
  accion = new CheckBoxDisjuntos;
  tipos = new checkbox_nombres_t;
  tipos->checkbox = new CheckBoxDisjuntos;
  tipos->nombres = new std::vector<std::string>;
  colores = new checkbox_nombres_t;
  colores->checkbox = new CheckBoxDisjuntos;
  colores->nombres = new std::vector<std::string>;
  ventana = NULL;
}

//
EditorNivel::~EditorNivel(){
  delete fondo;
  delete cambiar_fondo;
  delete cant_jugadores;
  delete puntaje_ganar;
  delete escritor;
  delete entrada;
  delete mensaje;
  delete accion;
  delete matriz;
  delete tipos->nombres;
  delete tipos->checkbox;
  delete tipos;
  delete colores->nombres;
  delete colores->checkbox;
  delete colores;
}

//
int EditorNivel::inicializar(const std::string &path, unsigned int columnas, unsigned int filas, Ventana *nueva_ventana){
  if (columnas == 0 || filas == 0) return -1;
  ventana = nueva_ventana;
  
  // FONDO
  fondo->cargar_textura(path + "imagenes/fondo_editor.png", ventana);
  
  unsigned int ancho = ventana->ver_ancho();
  unsigned int alto = ventana->ver_alto();
  
  // TEXT BOX ENTRADA
  std::string fuente_mono = path + "fuentes/mono.ttf";
  std::string fondo_texto = path + "imagenes/fondo_texto.png";
  unsigned int max_caracteres = ancho / 20;
  entrada->asignar_fuente(fuente_mono, 50, 3, max_caracteres);
  entrada->asignar_fondo(fondo_texto, ventana);
  entrada->alpha_fondo(FONDO_TEXTO);
  entrada->activar();
  entrada->asignar_color(255,255,255,255);
  SDL_Rect destino_entrada;
  destino_entrada.h = 30;
  destino_entrada.w = ancho - 10;
  destino_entrada.y = alto - destino_entrada.h - 5;
  destino_entrada.x = 5;
  entrada->asignar_destino(destino_entrada);
  
  // MENSAJE
  SDL_Rect destino_mensaje_fondo = destino_entrada;
  destino_mensaje_fondo.y = destino_mensaje_fondo.y - destino_mensaje_fondo.h - 2;
  mensaje->asignar_fondo(path + "imagenes/sub_ventana.png", destino_mensaje_fondo, ventana);
  mensaje->establecer_alpha_fondo(ALPHA_MENSAJE);
  mensaje->asignar_escritor(path + "fuentes/mono.ttf", 30);
  destino_mensaje.w = destino_mensaje_fondo.w - 6;
  destino_mensaje.x = destino_mensaje_fondo.x + 3;
  destino_mensaje.h = destino_mensaje_fondo.h - 2;
  destino_mensaje.y = destino_mensaje_fondo.y + 1;
  mensaje->asignar_mensaje("MENSAJE DE PRUEBA", destino_mensaje, ventana);

  // TEXTO
  escritor->asignar_fuente(path + "fuentes/mono.ttf", 30);
  escritor->asignar_color(0,0,0,255);
  
  // BOTON SPRITE
  SDL_Rect normal, apretado, sobre, destino_boton;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  destino_boton.w = ancho / 8;
  destino_boton.x = 10;
  destino_boton.h = alto / 10;
  
  SDL_Rect destino_texto;
  escritor->asignar_color(255, 255, 255, 255);
  Superficie sup;
  
  // BOTON CAMBIAR FONDO
  destino_boton.y = alto * 3 / 20;
  estructura.destino = destino_boton;
  cambiar_fondo->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  escritor->copiar_texto("Cambiar Fondo", &sup);
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  cambiar_fondo->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON CANTIDAD DE JUGADORES
  
  destino_boton.y += (alto - 200) / 3; 
  estructura.destino = destino_boton;
  cant_jugadores->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  escritor->copiar_texto("Jugadores Max", &sup);
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  cant_jugadores->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON CANTIDAD DE PUNTOS PARA GANAR
  destino_boton.y += (alto - 200) / 3; 
  estructura.destino = destino_boton;
  puntaje_ganar->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  escritor->copiar_texto("Puntaje Max", &sup);
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  puntaje_ganar->agregar_texto(&sup, destino_texto, ventana, 1);
  
  escritor->asignar_color(0, 0, 0, 255);
  
  
  datos_inicializados = true;
  
  // MATRIZ
  SDL_Rect destino_matriz;
  destino_matriz.x = 150;
  destino_matriz.y = 100;
  destino_matriz.w = ancho - 300;
  destino_matriz.h = alto - 200;
  matriz->inicializar(path + "imagenes/", columnas, filas, destino_matriz, ventana);
  
  
  // SPRITE CHECKBOX
  SDL_Rect apretado_sobre;
  normal.x = 0;
  normal.h = 37;
  normal.w = 42;
  
  normal.y = 74;
  apretado = normal;
  apretado.y = 0;
  sobre = normal;
  sobre.y = 111;
  apretado_sobre = sobre;
  apretado_sobre.y = 37;
 
  estructura_checkbox_t estructura_check;
  estructura_check.normal = normal;
  estructura_check.apretado = apretado;
  estructura_check.resaltado = sobre;
  estructura_check.resaltado_apretado = apretado_sobre;
  
  // CHECKBOX ACCIONES
  SDL_Rect destino;
  destino.y = 40;
  destino.w = 30;
  destino.h = 20;
  
  destino.x = 185;
  for (int i = 0; i < 4; i++){
    estructura_check.destino = destino;
    accion->agregar_checkbox("../recursos/imagenes/checkbox.png", estructura_check, ventana);
    destino.x += (ancho - 300) / 4;
  }
  
  destino_texto.y = 20;
  destino_texto.x = 160;
  destino_texto.w = 80;
  destino_texto.h = 20;

  escritor->asignar_color(0, 0, 0, 255);
  
  escritor->copiar_texto("Agujeros", &sup);
  accion->agregar_texto(AGUJEROS, &sup, destino_texto, ventana);
  
  destino_texto.x += (ancho - 300) / 4;
  escritor->copiar_texto("Fondo Celda", &sup);
  accion->agregar_texto(IMAGEN_CELDA, &sup, destino_texto, ventana);
  
  destino_texto.x += (ancho - 300) / 4;
  escritor->copiar_texto("Prob. Celda", &sup);
  accion->agregar_texto(PROB_CELDA, &sup, destino_texto, ventana);
  
  destino_texto.x += (ancho - 300) / 4;
  escritor->copiar_texto("Prob. Columna", &sup);
  accion->agregar_texto(PROB_COL, &sup, destino_texto, ventana);
  
  // COLORES
  colores->nombres->push_back("rojo");
  colores->nombres->push_back("azul");
  colores->nombres->push_back("verde");
  colores->nombres->push_back("amarillo");
  colores->nombres->push_back("violeta");
  
  destino.x = ancho - 60;
  destino_texto.x = destino.x - 15;
  destino_texto.w = 60;
  
  destino.y = 125;
  destino_texto.y = destino.y - 20;
  for (size_t i = 0; i < colores->nombres->size(); i++){
    estructura_check.destino = destino;
    colores->checkbox->agregar_checkbox("../recursos/imagenes/checkbox.png", estructura_check, ventana);
    std::string nombre = (*colores->nombres)[i];
    escritor->copiar_texto(nombre, &sup);
    colores->checkbox->agregar_texto(i, &sup, destino_texto, ventana);
    destino.y += (alto - 200) / 5;
    destino_texto.y += (alto - 200) / 5;
  }
  
  // TIPOS
  tipos->nombres->push_back("button");
  tipos->nombres->push_back("minibar H");
  tipos->nombres->push_back("minibar V");
  tipos->nombres->push_back("golden star");
  
  destino.x = ancho - 120;
  destino_texto.x = destino.x - 15;
  destino_texto.w += 5;
  
  destino.y = 165;
  destino_texto.y = destino.y - 20;
  for (size_t i = 0; i < tipos->nombres->size(); i++){
    estructura_check.destino = destino;
    tipos->checkbox->agregar_checkbox("../recursos/imagenes/checkbox.png", estructura_check, ventana);
    std::string nombre = (*tipos->nombres)[i];
    escritor->copiar_texto(nombre, &sup);
    tipos->checkbox->agregar_texto(i, &sup, destino_texto, ventana);
    destino.y += (alto - 200) / 5;
    destino_texto.y += (alto - 200) / 5;
  }
  
  return 0;
}

//
bool EditorNivel::correr(Ventana *ventana, const std::string &path_nivel){
  if (!datos_inicializados) return false;
  nivel_path = path_nivel;
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = DELAY;
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = this->analizar_evento(evento);
    }
    // Dibujado
    ventana->limpiar();
    this->dibujar(ventana);
    
    // actualizamos los fps
    frames.obtener_delay(tiempo_actual, delay);
    
    // Presentar en ventana
    ventana->presentar(delay);
  }
  return false;
}

//
int EditorNivel::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  cambiar_fondo->dibujar(ventana);
  entrada->dibujar(ventana);
  mensaje->dibujar(ventana);
  matriz->dibujar(ventana);
  accion->dibujar(ventana);
  colores->checkbox->dibujar(ventana);
  tipos->checkbox->dibujar(ventana);
  cant_jugadores->dibujar(ventana);
  puntaje_ganar->dibujar(ventana);
  return 0;
}

//
bool EditorNivel::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT) return false;
  cambiar_fondo->analizar_evento(evento);
  if (cambiar_fondo->activado()){
    this->cargar_fondo();
  }
  entrada->analizar_evento(evento);
  unsigned int fila, columna;
  accion->analizar_evento(evento);
  colores->checkbox->analizar_evento(evento);
  tipos->checkbox->analizar_evento(evento);
  cant_jugadores->analizar_evento(evento);
  puntaje_ganar->analizar_evento(evento);
  
  if (matriz->analizar_evento(evento, columna, fila)){
    if (accion->subindice_activado() == AGUJEROS){
      this->accion_agujero(columna, fila);
    }else if(accion->subindice_activado() == IMAGEN_CELDA){
      this->cambiar_fondo_celda(columna, fila);
    }
  }
  return true;
}

//
void EditorNivel::copiar_archivo(const std::string &path_src, const std::string &path_dest){
  std::ifstream  src(path_src.c_str(), std::ios::binary);
  std::ofstream  dst(path_dest.c_str(), std::ios::binary);
  dst << src.rdbuf();
}

//
void EditorNivel::cargar_fondo(){
  std::string path_fondo = entrada->ver_contenido();
  struct stat buffer;
  if (stat(path_fondo.c_str(), &buffer) == 0){
    std::string path_destino = nivel_path + "imagenes/fondo.png";
    this->copiar_archivo(path_fondo, path_destino);
    fondo->cargar_textura(path_destino, ventana);
    mensaje->asignar_mensaje("Fondo cargado correctamente", destino_mensaje, ventana);
  }else{
    mensaje->asignar_mensaje("Archivo del fondo invalido", destino_mensaje, ventana);
  }
}

//
void EditorNivel::accion_agujero(unsigned int col, unsigned int fila){
  if (matriz->celda_especial(col, fila)){
    std::string celda_especial = nivel_path + archivo_celda(col, fila);
    std::cout << celda_especial << std::endl;
    struct stat buffer;
    if (stat(celda_especial.c_str(), &buffer) == 0){
      remove(celda_especial.c_str());
    }
  }
  matriz->cambiar_estado(col, fila);
}

//
void EditorNivel::cambiar_fondo_celda(unsigned int col, unsigned int fila){
  std::string path_fondo = entrada->ver_contenido();
  struct stat buffer;
  if (stat(path_fondo.c_str(), &buffer) == 0){
    matriz->insertar_textura(path_fondo, col, fila, ventana);
    std::string celda_especial = nivel_path + archivo_celda(col, fila);
    this->copiar_archivo(path_fondo, celda_especial);
    mensaje->asignar_mensaje("Fondo de celda cargado correctamente", destino_mensaje, ventana);
  }else{
    mensaje->asignar_mensaje("Archivo del fondo de celda invalido", destino_mensaje, ventana);
  }
}
