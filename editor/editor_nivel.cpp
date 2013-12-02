#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


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
  escritor = new Texto;
  mensaje = new Mensaje;
  entrada = new TextBox;
  datos_inicializados = false;
  accion = new CheckBoxDisjuntos;
  matriz = new MatrizEditor;
  ventana = NULL;
}

//
EditorNivel::~EditorNivel(){
  delete fondo;
  delete cambiar_fondo;
  delete escritor;
  delete entrada;
  delete mensaje;
  delete accion;
  delete matriz;
}

//
int EditorNivel::inicializar(const std::string &path, size_t columnas, size_t filas, Ventana *nueva_ventana){
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
  
  // BOTON CAMBIAR FONDO
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  SDL_Rect destino_boton;
  destino_boton.y = alto * 3 / 20;
  destino_boton.w = ancho / 8;
  destino_boton.x = 10;
  destino_boton.h = alto / 10;
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  cambiar_fondo->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  Superficie sup;
  escritor->asignar_color(255, 255, 255, 255);
  escritor->copiar_texto("Cambiar Fondo", &sup);
  escritor->asignar_color(0, 0, 0, 255);
  
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  cambiar_fondo->agregar_texto(&sup, destino_texto, ventana, 1);
  
  datos_inicializados = true;
  
  // MATRIZ
  SDL_Rect destino_matriz;
  destino_matriz.x = 150;
  destino_matriz.y = 100;
  destino_matriz.w = ancho - 300;
  destino_matriz.h = alto - 200;
  matriz->inicializar(path + "imagenes/", columnas, filas, destino_matriz, ventana);
  
  // CHECKBOX ACCIONES
  SDL_Rect destino;
  destino.y = 40;
  destino.w = 30;
  destino.h = 20;

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
  
  return 0;
}

//
bool EditorNivel::correr(Ventana *ventana, const std::string &path_nivel){
  if (!datos_inicializados) return false;
  nivel_path = path_nivel;
  std::cout << nivel_path << std::endl;
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
  size_t fila, columna;
  if (matriz->analizar_evento(evento, columna, fila)){
    matriz->cambiar_estado(columna, fila);
  }
  accion->analizar_evento(evento);
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
    mensaje->asignar_mensaje("Archivo invalido", destino_mensaje, ventana);
  }
}
