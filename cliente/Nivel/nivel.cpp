#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../libs/SDL2/SDL.h"
#include "../../libs/json/include/json/json.h"
#include "nivel.h"

#define POS_X 100 // posicion en x de la matriz
#define POS_Y 100 // posicion de y en la matriz
#define ALPHA_MENSAJE 200

#define MOVER 0
#define EXPLOTAR 1
#define INSERTAR 2
#define PUNTAJE 3
#define FINALIZAR 4

#define VOLUMEN_ANIMACIONES 40

//
Nivel::Nivel(){
  fondo = NULL;
  seleccion = NULL;
  explosion = NULL;
  tablero = NULL;
  explosion = new Explosion;
  celdas_vacias = new CeldasVacias;
  productos = new Productos;
  receptor = new ReceptorResultados;
  celdas_a_explotar = new Lista<reemplazo_t>;
  puntaje = new Puntaje;
  mensaje = new Mensaje;
  socket_enviar = NULL;
  socket_recibir = NULL;
  finalizado = false;

  sonido_explosion = NULL;
  sonido_movimiento = NULL;
  sonido_seleccion = NULL;
}

//
Nivel::~Nivel(){
  if (fondo) delete fondo;
  if (seleccion) delete seleccion;
  if (tablero) delete tablero;
  
  delete explosion;
  delete celdas_vacias;
  delete productos;
  delete receptor;
  delete celdas_a_explotar;
  delete puntaje;
  delete mensaje;
  
  if (sonido_explosion) Mix_FreeChunk(sonido_explosion);
  if (sonido_movimiento) Mix_FreeChunk(sonido_movimiento);
  if (sonido_seleccion) Mix_FreeChunk(sonido_seleccion);
}

//
void Nivel::inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir){
  unsigned int ancho = ventana->ver_ancho();
  unsigned int alto = ventana->ver_alto();
  
  // SOCKETS
  receptor->agregar_socket(recibir, PUNTAJE);
  receptor->correr();
  socket_enviar = enviar;
  socket_recibir = recibir;
  
  // FONDOS
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar(path + "imagenes/fondo.png");
  fondo_sup->escalar(ancho,alto);
  
  // DEFINIMOS LA MATRIZ
  tablero = new Matriz;
  std::string direccion_archivo = path + "estructura.dat";
  
  Json::Value estructura;
  Json::Value aux;
  Json::Reader reader;  
  
  std::ifstream archivo_estructura(direccion_archivo.c_str(), std::ifstream::binary | std::ifstream::in);
  reader.parse(archivo_estructura, estructura, false);
  
  aux = estructura.get("filas", aux);
  int filas = aux.asInt();
  aux = estructura.get("columnas", aux);
  int columnas = aux.asInt();

  aux = estructura.get("matriz",aux );
  
  char **estructura_matriz = new char*[columnas];
  for (int x = 0; x < columnas; x++){
    estructura_matriz[x] = new char[filas];
    for (int y = 0; y < filas; y++){
      estructura_matriz[x][y] = aux[x][y].asInt();
    }
  }
  
  coordenada_t dimension;
  dimension.x = columnas;
  dimension.y = filas;
  
  archivo_estructura.close();
  
  ancho_celda = (ancho - POS_X * 2) / dimension.x;
  alto_celda = (alto - POS_Y * 2) / dimension.y;
  
  SDL_Rect origen;
  origen.x = POS_X;
  origen.y = POS_Y;
  origen.h = alto_celda;
  origen.w = ancho_celda;
  
  Superficie *fondo_celda = new Superficie;
  fondo_celda->cargar(path + "imagenes/celda_fondo.png");
  
  tablero->definir_forma(estructura_matriz, dimension, origen);
  for (int x = 0; x < columnas; x++){
    for (int y = 0; y < filas; y++){
      coordenada_t coordenada_actual;
      coordenada_actual.x = x;
      coordenada_actual.y = y;
      if (tablero->celda_existente(coordenada_actual)){
        char x_char = x + '0';
        char y_char = y + '0';
        std::string celda_especial = path + "imagenes/" + x_char + y_char + ".png";
        
        struct stat buffer;
        Superficie sup_celda_especial;
        if (stat(celda_especial.c_str(), &buffer) == 0){
          sup_celda_especial.cargar(celda_especial);
          tablero->dibujar_fondo_celdas(&sup_celda_especial, NULL, fondo_sup, coordenada_actual);
        }else{
          tablero->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup, coordenada_actual);
        }
      }
    }
  }
  
  for (int i = 0; i < dimension.x; i++){
    delete[] estructura_matriz[i];
  }
  delete[] estructura_matriz;  
  
  fondo = new Textura;
  fondo->cargar_textura(fondo_sup, ventana);
  delete fondo_sup;
  delete fondo_celda;

  // ANIMACIONES
  productos->cargar_animaciones(path,ventana);
  
  /*
  // RECIBIMOS LOS PRODUCTOS DE LA MATRIZ
  dato_t primero;
  dato_t segundo;
  char accion;
  bool continuar = true;
  while (continuar){
    if (!receptor->cola_vacia()){
      accion = receptor->ver_siguiente();
      if (accion == INSERTAR){
        receptor->borrar_siguiente(primero, segundo);
        Nivel::parsear_insercion(primero, segundo);
      }else if (accion == PUNTAJE){
        continuar = false;
      }
    }
  }
  */
  
  // RECIBIR ESTOS DATOS DEL SERVIDOR
  for (int i = 0; i < dimension.x; i++){
    for (int z = 0; z < dimension.y; z++){
      coordenada_t coord;
      coord.x = i;
      coord.y = z;
      if (tablero->celda_existente(coord)){
        
        int color = rand() % 5;
        int tipo = rand() % 4;

        Textura *textura = productos->ver_textura(tipo,color);
        Animacion *animacion = productos->ver_animacion(tipo, color);
        tablero->insertar(textura, animacion, coord);
      }
    }
  }
  
  // TEXTURA DE SELECCION
  seleccion = new Textura;
  seleccion->cargar_textura(path + "celda_seleccion.png", ventana);

  // EXPLOSION
  explosion->cargar_animacion(path, ventana);
  
  // CELDAS VACIAS
  celdas_vacias->inicializar(tablero->numero_columnas());
  
  // PUNTAJE
  SDL_Color color;
  color.r = 0;
  color.g = 0;
  color.b = 0;
  color.a = 255;
  
  puntaje->asignar_fuente("../../recursos/fuentes/orange.ttf", 100, color);
  SDL_Rect destino_puntaje;
  destino_puntaje.x = 10;
  destino_puntaje.y = 10;
  destino_puntaje.w = 35;
  destino_puntaje.h = 50;
  puntaje->asignar_destino(destino_puntaje, ventana);
  
  // MENSAJE
  Superficie sup;
  sup.cargar("../../recursos/imagenes/sub_ventana.png");
  SDL_Rect destino_mensaje;
  destino_mensaje.w = ancho;
  destino_mensaje.h = alto / 5;
  destino_mensaje.x = 0;
  destino_mensaje.y = alto / 2 - destino_mensaje.h / 2;
  mensaje->asignar_fondo(&sup, destino_mensaje, ventana);
  mensaje->establecer_alpha_fondo(ALPHA_MENSAJE);
  
  // SONIDO

  std::string direccion_explosion = "../../recursos/sonidos/explosion.wav";
  std::string direccion_movimiento = "../../recursos/sonidos/movimiento.wav";
  std::string direccion_seleccion = "../../recursos/sonidos/seleccion.wav";

  sonido_explosion = Mix_LoadWAV(direccion_explosion.c_str());
  sonido_movimiento = Mix_LoadWAV(direccion_movimiento.c_str());
  sonido_seleccion = Mix_LoadWAV(direccion_seleccion.c_str());

  if (sonido_explosion) sonido_explosion->volume = VOLUMEN_ANIMACIONES;
  if (sonido_movimiento) sonido_movimiento->volume = VOLUMEN_ANIMACIONES;
  if (sonido_seleccion) sonido_seleccion->volume = VOLUMEN_ANIMACIONES;
}

//
bool Nivel::correr(Ventana* ventana){
  if (!socket_recibir || !socket_enviar) return false;
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
    // realizamos resultados recibidos por el servidor
    Nivel::actualizar_receptor();
    // Dibujado
    ventana->limpiar();
    Nivel::dibujar(ventana);
    // Actualizacion
    Nivel::actualizar_animaciones();
    // actualizamos los fps
    Nivel::obtener_delay(frames, tiempo_actual, delay);
  
    // Presentar en ventana
    ventana->presentar(delay);
  }
  receptor->finalizar();
  receptor->join();
  return true;
}

//
bool Nivel::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT){ 
    return false;
    
  }else if (!tablero->esta_ocupada() && !explosion->explosion_en_curso() && 
            receptor->cola_vacia() && !receptor->recibiendo_datos() && !finalizado){
    if (evento.type == SDL_MOUSEBUTTONDOWN){
      coordenada_t celda;
      celda.x = (evento.button.x - POS_X) / ancho_celda;
      celda.y = (evento.button.y - POS_Y) / alto_celda;
      if (evento.button.x - POS_X >= 0 && 
          evento.button.y - POS_Y >= 0 && 
          celda.x < tablero->numero_columnas() && 
          celda.y < tablero->numero_filas()){
      
        if (tablero->celda_existente(celda)){
          if(evento.button.button == SDL_BUTTON_LEFT){
            coordenada_t celda_adyacente;
            if (tablero->adyacente_seleccionado(celda, celda_adyacente)){
              Nivel::intercambiar(celda, celda_adyacente);
              Nivel::enviar_movimiento(celda, celda_adyacente);
            }else{
              tablero->seleccionar(seleccion, celda);
              Mix_PlayChannel(-1, sonido_seleccion, 0);
            }
          }
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
  puntaje->dibujar();
  if (finalizado) mensaje->dibujar(ventana);
}

//
void Nivel::actualizar_animaciones(){
  // actualizar explosion
  explosion->animar();
  
  // actualizar animaciones
  productos->animar();
  
  // actualizar celdas vacias
  if (!tablero->esta_ocupada() && !explosion->explosion_en_curso() && celdas_vacias->existentes()){
    for (int i = 0; i < tablero->numero_columnas(); i++){
      if (!celdas_vacias->esta_vacia(i)){
        reemplazo_t reemplazo = celdas_vacias->borrar_proxima(i);
        Nivel::apilar(reemplazo.tipo,reemplazo.color,reemplazo.celda);
      }
    }
  }
}

//
void Nivel::obtener_delay(FPS &frames, int tiempo_actual, int &delay){
  if (SDL_GetTicks() - tiempo_actual < 1000){
    frames.actualizar();
    delay =  (1000/60.0f) * (frames.ver_fps()/60.0f);
  }
}

//
void Nivel::intercambiar(coordenada_t &origen, coordenada_t &destino){
  tablero->intercambiar(origen, destino);
  Mix_PlayChannel(-1, sonido_movimiento, 0);
}

//
void Nivel::apilar(int tipo, int color, coordenada_t &celda){
  tablero->apilar(productos->ver_textura(tipo, color), productos->ver_animacion(tipo, color), celda);
}

//
void Nivel::explotar(coordenada_t &celda, int tipo, int color){
  if (!tablero->celda_existente(celda)) return;
  explosion->explotar(celda, tablero);
  celdas_vacias->agregar(celda, tipo, color);
}

//
void Nivel::actualizar_receptor(){
  if (!receptor->cola_vacia()){
    Uint32 mensaje;
    char tipo = receptor->borrar_siguiente(mensaje);
    switch(tipo){
      case MOVER:
        this->parsear_movimiento(mensaje);
        break;
      case EXPLOTAR:
        this->parsear_explosion(mensaje);
        break;
      case INSERTAR:
        Nivel::parsear_insercion(mensaje);
        break;
      case PUNTAJE:
        Nivel::parsear_puntaje(mensaje);
        break;
      case FINALIZAR:
        finalizado = true;
        break;
    }
    if (tipo != EXPLOTAR && !celdas_a_explotar->esta_vacia()){
      reemplazo_t datos;
      do{
        datos = celdas_a_explotar->borrar_primero();
        Nivel::explotar(datos.celda, datos.tipo, datos.color);
      }while (!celdas_a_explotar->esta_vacia());
      Mix_PlayChannel(-1, sonido_explosion, 0);
    }
  }
}

//
void Nivel::enviar_movimiento(coordenada_t &celda, coordenada_t &celda_adyacente){
  char mensaje[5];
  mensaje[0] = MOVER;
  mensaje[1] = celda.x;
  mensaje[2] = celda.y;
  mensaje[3] = celda_adyacente.x;
  mensaje[4] = celda_adyacente.y;
  socket_enviar->enviar(mensaje, 5);
}

//
void Nivel::insertar(coordenada_t &celda, int tipo, int color){
  if (tablero->celda_existente(celda) && tipo <= 3 && tipo >= 0 && color >= 0 && color <= 4){
    Textura *textura = productos->ver_textura(tipo,color);
    Animacion *animacion = productos->ver_animacion(tipo, color);
    tablero->insertar(textura, animacion, celda);
  }
}

//
void Nivel::parsear_movimiento(uint32_t &mensaje){
  coordenada_t origen, destino;
  char traducir[4];
  memcpy(traducir, &mensaje, 4);
  origen.x = traducir[0];
  origen.y = traducir[1];
  destino.x = traducir[2];
  destino.y = traducir[3];
  if (tablero->celda_existente(origen) && tablero->celda_existente(destino) && (origen.x != destino.x || origen.y != destino.y)){
    this->intercambiar(origen, destino);
  }
}

//
void Nivel::parsear_explosion(uint32_t &mensaje){
  char traducir[4];
  memcpy(traducir, &mensaje, 4);
  coordenada_t celda;
  celda.x = traducir[0];
  celda.y = traducir[1];
  int tipo = traducir[2];
  int color = traducir[3];
  if (tablero->celda_existente(celda) && tipo <= 3 && tipo >= 0 && color >= 0 && color <= 4){
    reemplazo_t datos;
    datos.tipo = tipo;
    datos.color = color;
    datos.celda = celda;
    celdas_a_explotar->insertar_ultimo(datos);
  }
}

//
void Nivel::parsear_insercion(uint32_t &mensaje){
  char traducir[4];
  memcpy(traducir, &mensaje, 4);
  coordenada_t celda;
  celda.x = traducir[0];
  celda.y = traducir[1];
  int tipo = traducir[2];
  int color = traducir[3];
  if (tablero->celda_existente(celda)){
    Nivel::insertar(celda, tipo, color);
  }
}

//
void Nivel::parsear_puntaje(uint32_t &mensaje){
  unsigned int dif = ntohl(mensaje);
  puntaje->sumar_puntos(dif);
}
