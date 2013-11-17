#include <iostream>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/fps.h"
#include "nivel.h"
#include "matriz.h"

// largo inicial del vector animaciones
#define LARGO 15

#define POS_X 180 // posicion en x de la matriz
#define POS_Y 100 // posicion de y en la matriz
#define LARGO_CELDA_X 50
#define LARGO_CELDA_Y 40
#define CANT_ANIMACIONES 1
#define CANT_CELDAS_X 10
#define CANT_CELDAS_Y 8
#define CANTIDAD_FPS 15.0f

//
Nivel::Nivel(){
  animaciones = new animacion_t*[LARGO];
  cant_animaciones = 0;
  fondo = NULL;
  seleccion = NULL;
  explosion = NULL;
  tablero = NULL;
  explosion = new Explosion;
  celdas_vacias = new CeldasVacias;
}

//
Nivel::~Nivel(){
  if (fondo) delete fondo;
  if (seleccion) delete seleccion;
  if (tablero) delete tablero;
  for (int i = 0; i < cant_animaciones; i++){
    delete animaciones[i]->animacion;
    delete animaciones[i]->textura;
    delete animaciones[i];
  }
  delete[] animaciones;
  delete explosion;
  delete celdas_vacias;
}

//
void Nivel::inicializar_datos(const std::string &path, Ventana *ventana){
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar(path + "fondo1.png");
  fondo_sup->escalar(840,525);
  
  Superficie *fondo_celda = new Superficie;
  fondo_celda->cargar("imagenes/celda_fondo.png");
  
  
  tablero = new Matriz;
  
  // LO SIGUIENTE LO DEBE RECIBIR DEL SERVIDOR
  coordenada_t dimension;
  dimension.x = CANT_CELDAS_X;
  dimension.y = CANT_CELDAS_Y;
  char **estructura = new char*[dimension.x];
  for (int i = 0; i < dimension.x; i++){
    estructura[i] = new char[dimension.y];
    for (int z = 0; z < dimension.y; z++){
      estructura[i][z] = 1;
    }
  }
  
  // HASTA ACA
  
  SDL_Rect origen;
  origen.x = POS_X;
  origen.y = POS_Y;
  origen.h = LARGO_CELDA_Y;
  origen.w = LARGO_CELDA_X;
  
  tablero->definir_forma(estructura, dimension, origen);
  tablero->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup);
  
  fondo = new Textura;
  fondo->cargar_textura(fondo_sup, ventana);
  delete fondo_sup;
  delete fondo_celda;

  // ANIMACIONES
  for (int i = 0; i < CANT_ANIMACIONES; i++){
    SpritePos_t sprite;
    SDL_Rect tam;
    tam.x = 0;
    tam.y = 0;
    tam.w = 352;
    tam.h = 40;
    origen.x = 0;
    origen.y = 0;
    origen.w = 32;
    origen.h = 40;
    sprite.dimension_total = tam;
    sprite.imagen_inicial = origen;
    sprite.desplazamiento_x = 32;
    sprite.desplazamiento_y = 0;
    
    Superficie *animacion_sup = new Superficie;
    animacion_sup->cargar(path + "estrella.png");
    SDL_Color color;
    animacion_sup->color_pixel(0,0, color);
    animacion_sup->color_clave(color);
    
    Textura* animacion_tex = new Textura;
    animacion_tex->cargar_textura(animacion_sup, ventana);
    delete animacion_sup;
    
    Animacion *animacion = new Animacion;
    animacion->cargar_sprite(sprite);
    animacion_t *animacion_temp = new animacion_t;
    animacion_temp->textura = animacion_tex;
    animacion_temp->animacion = animacion;
    animaciones[i] = animacion_temp;
    animaciones[i]->animacion->establecer_fps(CANTIDAD_FPS);
    cant_animaciones++;
  }
  
  coordenada_t celda;
  for (int i = 0; i < dimension.x; i++){
    celda.x = i;
    for (int z = 0; z < dimension.y; z++){
      celda.y = z;
      tablero->insertar(animaciones[0]->textura, animaciones[0]->animacion, celda);
    }
  }
  
  
  
  // TEXTURA DE SELECCION
  seleccion = new Textura;
  seleccion->cargar_textura("imagenes/celda_seleccion.png", ventana);

  // EXPLOSION
  explosion->cargar_animacion(path, ventana);
  
  // CELDAS VACIAS
  celdas_vacias->inicializar(tablero->numero_columnas());
  
  sonido = Mix_LoadWAV("sonidos/sound.wav");   // FALTA DEFINIR CLASE SONIDO
}

//
void Nivel::correr(const std::string &path, Ventana* ventana){
  Nivel::inicializar_datos(path, ventana);
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = 16;
  while (corriendo){
    while (SDL_PollEvent(&evento)){
      corriendo = Nivel::analizar_evento(evento);
    }
    ventana->limpiar();
    Nivel::dibujar(ventana);
    Nivel::actualizar_animaciones();
    
    if (SDL_GetTicks() - tiempo_actual < 2000){
      delay = Nivel::calcular_delay(frames);
    }
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
      celda.x = (evento.button.x - POS_X) / LARGO_CELDA_X;
      celda.y = (evento.button.y - POS_Y) / LARGO_CELDA_Y;
      if (evento.button.x - POS_X >= 0 && 
      evento.button.y - POS_Y >= 0 && 
      celda.x < CANT_CELDAS_X && 
      celda.y < CANT_CELDAS_Y){
            
        Mix_PlayChannel(-1, sonido, 0); // FALTA DEFINIR CLASE SONIDO
        
        if(evento.button.button == SDL_BUTTON_LEFT){
          coordenada_t celda_adyacente;
          if (tablero->adyacente_seleccionado(celda, celda_adyacente)){
            Nivel::intercambiar(celda, celda_adyacente);
          }else{
            tablero->seleccionar(seleccion, celda);
          }
        }else if(evento.button.button == SDL_BUTTON_RIGHT){
          Nivel::secuencia_prueba();
          //tablero->quitar_seleccion();
          //explotar(celda);
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
  animaciones[0]->animacion->animar();
  if (animaciones[0]->animacion->fuera_del_sprite()){
    animaciones[0]->animacion->reiniciar();
  }
  
  // actualizar celdas vacias
  if (!tablero->esta_ocupada() && !explosion->explosion_en_curso()){
    coordenada_t celda;
    for (int i = 0; i < tablero->numero_columnas(); i++){
      if (!celdas_vacias->esta_vacia(i)){
        celda = celdas_vacias->borrar_proxima(i);
        Nivel::apilar(0,celda);
      }
    }
  }
}

//
int Nivel::calcular_delay(FPS &frames){
  frames.actualizar();
  return (1000/60.0f) * (frames.ver_fps() / 60.0f);
}

//
void Nivel::intercambiar(coordenada_t &origen, coordenada_t &destino){
  tablero->intercambiar(origen, destino);
}

//
void Nivel::apilar(int producto, coordenada_t &celda){
  tablero->apilar(animaciones[producto]->textura, animaciones[producto]->animacion, celda);
}

//
void Nivel::explotar(coordenada_t &celda){
  explosion->explotar(celda, tablero);
  celdas_vacias->agregar(celda);
}

//
void Nivel::explotar_segmento(coordenada_t &origen, coordenada_t &destino){
  int desp_x = destino.x - origen.x;
  if (desp_x != 0) desp_x = desp_x / abs(desp_x);
  
  int desp_y = destino.y - origen.y;
  if (desp_y != 0) desp_y = desp_y / abs(desp_y);
  
  coordenada_t actual = origen;
  Nivel::explotar(actual);
  while (actual.x != destino.x || actual.y != destino.y){
    actual.x += desp_x;
    actual.y += desp_y;
    Nivel::explotar(actual);
  }
}


void Nivel::secuencia_prueba(){
  coordenada_t inicio;
  inicio.x = 0;
  inicio.y = 0;
  coordenada_t fin;
  fin.x = 5;
  fin.y = 5;
  Nivel::explotar_segmento(inicio,fin);
  inicio.y = 1;
  fin.x = 0;
  fin.y = 5;
  Nivel::explotar_segmento(inicio,fin);
}

/* ********************************************************************
 *                        CELDAS VACIAS
 * ********************************************************************/


//
CeldasVacias::CeldasVacias(){
  celdas_vacias = NULL;
}

//
CeldasVacias::~CeldasVacias(){
  if (celdas_vacias){
    for (int i = 0; i < columnas; i++){
      delete celdas_vacias[i];
    }
    delete[] celdas_vacias;
  }
}

//
void CeldasVacias::inicializar(int cantidad_columnas){
  columnas = cantidad_columnas;
  celdas_vacias = new Lista<coordenada_t>*[columnas];
  for (int i = 0; i < columnas; i++)
    celdas_vacias[i] = new Lista<coordenada_t>;
}

//
void CeldasVacias::agregar(coordenada_t &celda){
  Lista<coordenada_t> *lista = celdas_vacias[celda.x];
  if (lista->esta_vacia()){
    lista->insertar_ultimo(celda);
  }else{
    Lista_iter<coordenada_t> iterador;
    iterador.iterar_en_lista(lista);
    
    coordenada_t actual = iterador.ver_actual();
    while (actual.y < celda.y){
      iterador.avanzar();
      if (iterador.al_final()) break;
      actual = iterador.ver_actual();
    }
    iterador.insertar(celda);
  }
}

//
bool CeldasVacias::esta_vacia(int columna){
  if (columna < 0 || columna >= columnas) return false;
  Lista<coordenada_t> *lista = celdas_vacias[columna];
  return lista->esta_vacia();
}

//
coordenada_t CeldasVacias::borrar_proxima(int columna)throw(ListaVacia, ColumnaInvalida){
  if (columna < 0 || columna >= columnas) throw ColumnaInvalida();
  Lista<coordenada_t> *lista = celdas_vacias[columna];
  return lista->borrar_primero();
}

/* ********************************************************************
 *                        EXPLOSION
 * ********************************************************************/

//
Explosion::Explosion(){
  animacion = NULL;
  textura = NULL;
  en_curso = false;
}

//
Explosion::~Explosion(){
  if (textura) delete textura;
  if (animacion) delete animacion;
}

//
void Explosion::cargar_animacion(const std::string &path, Ventana *ventana){
  SpritePos_t exp;
  SDL_Rect SrcE;
  SDL_Rect DestE;
  SrcE.x = 0;
  SrcE.y = 0;
  SrcE.w = 73;
  SrcE.h = 90;
  DestE.x = 0;
  DestE.y = 0;
  DestE.w = 1022;
  DestE.h = 90;
  exp.dimension_total = DestE;
  exp.imagen_inicial = SrcE;
  exp.desplazamiento_x = 73;
  exp.desplazamiento_y = 0;
  
  Superficie *exp_sup = new Superficie;
  exp_sup->cargar(path + "explosion.png");
  SDL_Color color_exp;
  exp_sup->color_pixel(0,0, color_exp);
  exp_sup->color_clave(color_exp);
  
  textura = new Textura;
  textura->cargar_textura(exp_sup, ventana);
  delete exp_sup;
  
  animacion = new Animacion;
  animacion->cargar_sprite(exp);
  animacion->establecer_fps(CANTIDAD_FPS);
}

//
void Explosion::explotar(coordenada_t &celda, Matriz* tablero){
  animacion->reiniciar();
  en_curso = true;
  tablero->insertar(textura, animacion, celda);
}

//
bool Explosion::explosion_en_curso(){
  return en_curso;
}

//
bool Explosion::finalizada(){
  return animacion->fuera_del_sprite();
}

//
void Explosion::animar(){
  if (animacion->fuera_del_sprite()){
    en_curso = false;
  }
  if (!en_curso) return;
  animacion->animar();
}
