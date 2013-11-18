#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "SDL/fps.h"
#include "nivel.h"

#define POS_X 100 // posicion en x de la matriz
#define POS_Y 100 // posicion de y en la matriz

//
Nivel::Nivel(){
  fondo = NULL;
  seleccion = NULL;
  explosion = NULL;
  tablero = NULL;
  explosion = new Explosion;
  celdas_vacias = new CeldasVacias;
  productos = new Productos;
  socket_enviar = NULL;
  socket_recibir = NULL;
}

//
Nivel::~Nivel(){
  if (fondo) delete fondo;
  if (seleccion) delete seleccion;
  if (tablero) delete tablero;
  
  delete explosion;
  delete celdas_vacias;
  delete productos;
}

//
void Nivel::inicializar_datos(const std::string &path, Ventana *ventana, 
                              int ancho, int alto){
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar(path + "imagenes/fondo1.png");
  fondo_sup->escalar(ancho,alto);
  
  Superficie *fondo_celda = new Superficie;
  fondo_celda->cargar(path + "imagenes/celda_fondo.png");
  
  // DEFINIMOS LA MATRIZ
  tablero = new Matriz;
  std::string direccion_archivo = path + "estructura.dat";
  std::ifstream archivo_estructura(direccion_archivo.c_str(), std::ifstream::in);
  std::string linea;
  
  coordenada_t dimension;
  std::getline(archivo_estructura, linea);
  dimension.x = atoi(linea.c_str());
  std::getline(archivo_estructura, linea);
  dimension.y = atoi(linea.c_str());
  
  char **estructura = new char*[dimension.x];
  char caracter;
  for (int i = 0; i < dimension.x; i++){
    estructura[i] = new char[dimension.y];
    for (int z = 0; z < dimension.y; z++){
      caracter = archivo_estructura.get();
      if (caracter == '\n')
        caracter = archivo_estructura.get();
      estructura[i][z] = caracter;
    }
  }
  
  archivo_estructura.close();
  ancho_celda = (ancho - POS_X * 2) / dimension.x;
  alto_celda = (alto - POS_Y * 2) / dimension.y;
  
  SDL_Rect origen;
  origen.x = POS_X;
  origen.y = POS_Y;
  origen.h = alto_celda;
  origen.w = ancho_celda;
  
  tablero->definir_forma(estructura, dimension, origen);
  tablero->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup);
  
  for (int i = 0; i < dimension.x; i++){
    delete[] estructura[i];
  }
  delete[] estructura;
  
  
  
  fondo = new Textura;
  fondo->cargar_textura(fondo_sup, ventana);
  delete fondo_sup;
  delete fondo_celda;

  // ANIMACIONES
  productos->cargar_animaciones(path,ventana);
  
  for (int i = 0; i < dimension.x; i++){
    for (int z = 0; z < dimension.y; z++){
      coordenada_t coord;
      coord.x = i;
      coord.y = z;
      int color = rand() % 5;
      int tipo = rand() % 4;
      Textura *textura = productos->ver_textura(tipo,color);
      Animacion *animacion = productos->ver_animacion(tipo, color);
      tablero->insertar(textura, animacion, coord); 
    }
  }
  
  // TEXTURA DE SELECCION
  seleccion = new Textura;
  seleccion->cargar_textura(path + "celda_seleccion.png", ventana);

  // EXPLOSION
  explosion->cargar_animacion(path, ventana);
  
  // CELDAS VACIAS
  celdas_vacias->inicializar(tablero->numero_columnas());
  
  std::string direccion = path + "sonidos/sound.wav";
  sonido = Mix_LoadWAV(direccion.c_str());   // FALTA DEFINIR CLASE SONIDO
}

//
void Nivel::asignar_sockets(Socket* enviar, Socket* recibir){
  socket_recibir = recibir;
  socket_enviar = enviar;
}

//
void Nivel::correr(const std::string &path, Ventana* ventana, int ancho, int alto){
  // Inicializacion
  Nivel::inicializar_datos(path, ventana, ancho, alto);
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = 16;
  
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = Nivel::analizar_evento(evento);
    }
    
    // Dibujado
    ventana->limpiar();
    Nivel::dibujar(ventana);
    
    // Actualizacion
    Nivel::actualizar_animaciones();
    
    if (SDL_GetTicks() - tiempo_actual < 1000){
      delay = Nivel::calcular_delay(frames);
    }
    
    // Presentar en ventana
    ventana->presentar(delay);
  }
}

//
bool Nivel::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT){ 
    return false;
    
  }else if (!tablero->esta_ocupada() && !explosion->explosion_en_curso()){
    if (evento.type == SDL_MOUSEBUTTONDOWN){
      coordenada_t celda;
      celda.x = (evento.button.x - POS_X) / ancho_celda;
      celda.y = (evento.button.y - POS_Y) / alto_celda;
      if (evento.button.x - POS_X >= 0 && 
      evento.button.y - POS_Y >= 0 && 
      celda.x < tablero->numero_columnas() && 
      celda.y < tablero->numero_filas())
      
      if (tablero->celda_existente(celda)){
          Mix_PlayChannel(-1, sonido, 0); // FALTA DEFINIR CLASE SONIDO
        if(evento.button.button == SDL_BUTTON_LEFT){
          coordenada_t celda_adyacente;
          if (tablero->adyacente_seleccionado(celda, celda_adyacente)){
            Nivel::intercambiar(celda, celda_adyacente);
          }else{
            tablero->seleccionar(seleccion, celda);
          }
        }else if(evento.button.button == SDL_BUTTON_RIGHT){
          tablero->quitar_seleccion();
          explotar(celda);
        }
      }
    }
  }
  return true;
}

//
void Nivel::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  tablero->dibujar(ventana);
}

//
void Nivel::actualizar_animaciones(){
  // actualizar explosion
  explosion->animar();
  
  // actualizar animaciones
  productos->animar();
  
  // actualizar celdas vacias
  if (!tablero->esta_ocupada() && !explosion->explosion_en_curso() && celdas_vacias->existentes()){
    coordenada_t celda;
    for (int i = 0; i < tablero->numero_columnas(); i++){
      if (!celdas_vacias->esta_vacia(i)){
        celda = celdas_vacias->borrar_proxima(i);
        int color = rand() % 5;
        int tipo = rand() % 4;
        Nivel::apilar(tipo,color,celda);
      }
    }
  }
}

//
int Nivel::calcular_delay(FPS &frames){
  frames.actualizar();
  return (1000/60.0f) * (frames.ver_fps()/60.0f);
}

//
void Nivel::intercambiar(coordenada_t &origen, coordenada_t &destino){
  tablero->intercambiar(origen, destino);
}

//
void Nivel::apilar(int tipo, int color, coordenada_t &celda){
  tablero->apilar(productos->ver_textura(tipo, color), productos->ver_animacion(tipo, color), celda);
}

//
void Nivel::explotar(coordenada_t &celda){
  explosion->explotar(celda, tablero);
  celdas_vacias->agregar(celda);
}
