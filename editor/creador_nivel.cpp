#include <iostream>

#include "creador_nivel.h"
#include "../libs/TDA/socket/socket_prefijo.h"

#define MAX_CARACTERES 20
#define FONDO_TEXTO 175
#define ALPHA_MENSAJE 255
#define DELAY 16

//
CreadorNivel::CreadorNivel(){
  fondo = new Textura;
  crear_nivel = new Boton;
  nombre = new TextBox;
  columnas = new TextBox;
  filas = new TextBox;
  escritor = new Texto;
  textura_nombre = new Textura;
  textura_columnas = new Textura;
  textura_filas = new Textura;
  mensaje = new Mensaje;
  enviando_datos = false;
  datos_inicializados = false;
}

//
CreadorNivel::~CreadorNivel(){
  delete fondo;
  delete crear_nivel;
  delete nombre;
  delete columnas;
  delete filas;
  delete escritor;
  delete textura_nombre;
  delete textura_columnas;
  delete textura_filas;
  delete mensaje;
}

//
int CreadorNivel::inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir){
  // FONDO
  fondo->cargar_textura(path + "imagenes/fondo_editor.png", ventana);
  
  unsigned int ancho = ventana->ver_ancho();
  unsigned int alto = ventana->ver_alto();
  
  // usado como punto de referencia para los demas destinos
  SDL_Rect destino;
  destino.x = ancho / 2 - ancho / 8;
  destino.w = ancho * 2 / 8;
  destino.h = alto / 20;
  destino.y = alto / 2 - destino.h * 2;
  
  // NOMBRE
  std::string fuente_mono = path + "fuentes/mono.ttf";
  std::string fondo_texto = path + "imagenes/fondo_texto.png";
  nombre->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  nombre->asignar_fondo(fondo_texto, ventana);
  nombre->alpha_fondo(FONDO_TEXTO);
  SDL_Rect destino_nombre = destino;
  nombre->asignar_destino(destino_nombre);
  nombre->activar();
  nombre->asignar_color(255,255,255,255);   
  
  destino_text_nombre.y = destino.y - destino.h;
  destino_text_nombre.x = destino.x;
  destino_text_nombre.h = destino.h;
  destino_text_nombre.w = destino.w;
  
  // COLUMNAS
  columnas->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  columnas->asignar_fondo(fondo_texto, ventana);
  columnas->alpha_fondo(FONDO_TEXTO);
  SDL_Rect destino_columnas = destino;
  destino_columnas.x -= destino_columnas.w/2 + 5; 
  destino_columnas.y = destino.y + destino.h * 3;
  columnas->asignar_destino(destino_columnas);
  columnas->asignar_color(255,255,255,255);

  destino_text_columnas.y = destino_columnas.y - destino_columnas.h;
  destino_text_columnas.x = destino_columnas.x;
  destino_text_columnas.h = destino_columnas.h;
  destino_text_columnas.w = destino_columnas.w;

  // FILAS
  filas->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  filas->asignar_fondo(fondo_texto, ventana);
  filas->alpha_fondo(FONDO_TEXTO);
  SDL_Rect destino_filas = destino;
  destino_filas.x += destino_filas.w/2 + 5; 
  destino_filas.y = destino.y + destino.h * 3;
  filas->asignar_destino(destino_filas);
  filas->asignar_color(255,255,255,255);

  destino_text_filas.y = destino_filas.y - destino_filas.h;
  destino_text_filas.x = destino_filas.x;
  destino_text_filas.h = destino_filas.h;
  destino_text_filas.w = destino_filas.w;

  // TEXTO
  Superficie sup;
  
  escritor->asignar_fuente(path + "fuentes/mono.ttf", 30);
  escritor->asignar_color(0,0,0,255);
  
  escritor->copiar_texto("Nombre", &sup);
  textura_nombre->cargar_textura(&sup, ventana);
  escritor->copiar_texto("Columnas", &sup);
  textura_columnas->cargar_textura(&sup, ventana);
  escritor->copiar_texto("Filas", &sup);
  textura_filas->cargar_textura(&sup, ventana);
  
  // BOTON  
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  SDL_Rect destino_boton = destino;
  destino_boton.y = destino.y + destino.h * 5;
  destino_boton.w = destino.w;
  destino_boton.x = destino.x;
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  crear_nivel->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->asignar_color(255,255,255,255);
  escritor->copiar_texto("Crear Nivel", &sup);
  escritor->asignar_color(0,0,0,255);
  
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  crear_nivel->agregar_texto(&sup, destino_texto, ventana, 1);
  
 
  // MENSAJE
  destino.w = ancho / 3;
  destino.h = alto / 4;
  destino.x = ancho / 2 - destino.w / 2;
  destino.y = alto / 2 - destino.h / 2;
  mensaje->asignar_fondo(path + "imagenes/sub_ventana.png", destino, ventana);
  mensaje->establecer_alpha_fondo(ALPHA_MENSAJE);
  mensaje->asignar_escritor(path + "fuentes/mono.ttf", 30);
  destino.w = destino.w - destino.w / 5;
  destino.h = destino.h / 4;
  destino.y = destino.y + destino.h * 2 - destino.h / 2;
  destino.x = destino.x + destino.w / 8;
  mensaje->asignar_mensaje("Validando Datos...", destino, ventana);
  
  datos_inicializados = true;
  return 0;
}

//
bool CreadorNivel::correr(Ventana *ventana, std::string &nombre_leido){
  if (!datos_inicializados) return false;
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
    if (enviando_datos){
      nombre_leido = nombre->ver_contenido();
      return true;
    }
  }
  return false;
}

//
int CreadorNivel::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  crear_nivel->dibujar(ventana);
  nombre->dibujar(ventana);
  columnas->dibujar(ventana);
  filas->dibujar(ventana);
  textura_nombre->dibujar(destino_text_nombre, ventana);
  textura_columnas->dibujar(destino_text_columnas, ventana);
  textura_filas->dibujar(destino_text_filas, ventana);
  if (enviando_datos) mensaje->dibujar(ventana);
  return 0;
}

//
bool CreadorNivel::analizar_evento(SDL_Event &evento){
  if (enviando_datos) return true;
  if (evento.type == SDL_QUIT) return false;
  nombre->analizar_evento(evento);
  columnas->analizar_evento(evento);
  filas->analizar_evento(evento);
  crear_nivel->analizar_evento(evento);
  if (crear_nivel->activado())
    enviando_datos = true;
  return true;
}

/*
//
void CreadorNivel::enviar_datos(){
  conectando = true;
  autentificador->autentificar(usuario->ver_contenido(), clave->ver_contenido());
  autentificador->correr();
}

*/
