#include <iostream>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/fps.h"
#include "nivel.h"
#include "matriz.h"

// largo inicial del vector animaciones
#define LARGO 15

#define POS_X 100 // posicion en x de la matriz
#define POS_Y 100 // posicion de y en la matriz
#define CANT_CELDAS_X 15
#define CANT_CELDAS_Y 10
#define FPS_ANIMACION 8.0f
#define FPS_EXPLOSION 15.0f

//
Nivel::Nivel(){
  fondo = NULL;
  seleccion = NULL;
  explosion = NULL;
  tablero = NULL;
  explosion = new Explosion;
  celdas_vacias = new CeldasVacias;
  productos = new Productos;
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
void Nivel::inicializar_datos(const std::string &path, Ventana *ventana, int ancho, int alto){
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar(path + "fondo1.png");
  fondo_sup->escalar(ancho,alto);
  
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
  
  ancho_celda = (ancho - POS_X * 2) / CANT_CELDAS_X;
  alto_celda = (alto - POS_Y * 2) / CANT_CELDAS_Y;
  
  SDL_Rect origen;
  origen.x = POS_X;
  origen.y = POS_Y;
  origen.h = alto_celda;
  origen.w = ancho_celda;
  
  tablero->definir_forma(estructura, dimension, origen);
  tablero->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup);
  
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
      tablero->insertar(productos->ver_textura(tipo,color), productos->ver_animacion(tipo,color),coord); 
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
void Nivel::correr(const std::string &path, Ventana* ventana, int ancho, int alto){
  Nivel::inicializar_datos(path, ventana, ancho, alto);
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
    
    if (SDL_GetTicks() - tiempo_actual < 1000){
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
      celda.x = (evento.button.x - POS_X) / ancho_celda;
      celda.y = (evento.button.y - POS_Y) / alto_celda;
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
          //Nivel::secuencia_prueba();
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
  contador = 0;
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
  contador++;
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
  contador--;
  return lista->borrar_primero();
}

//
bool CeldasVacias::existentes(){
  return contador != 0;
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
  animacion->establecer_fps(FPS_EXPLOSION);
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

/* ********************************************************************
 *                           PRODUCTOS
 * ********************************************************************/

//
Productos::Productos(){
  cant_tipos = 3;
  cant_colores = 5;
  animaciones = new animacion_t**[cant_tipos];
  for (int i = 0; i < cant_tipos; i++){
    animaciones[i] = new animacion_t*[cant_colores];
    for (int w = 0; w < cant_colores; w++){
      animaciones[i][w] = new animacion_t;
      animaciones[i][w]->textura = new Textura;
      animaciones[i][w]->animacion = new Animacion;
    }
  }
  estrella = new animacion_t;
  estrella->textura = new Textura;
  estrella->animacion = new Animacion;
}

//
Productos::~Productos(){
  delete estrella->textura;
  delete estrella->animacion;
  delete estrella;
  for (int i = 0; i < cant_tipos; i++){
    for (int w = 0; w < cant_colores; w++){
      delete animaciones[i][w]->animacion;
      delete animaciones[i][w]->textura;
      delete animaciones[i][w];
    }
    delete[] animaciones[i];
  }  
  delete[] animaciones;
}

//
void Productos::cargar_animaciones(const std::string &path, Ventana *ventana){
  SpritePos_t exp;
  SDL_Rect SrcE;
  SDL_Rect DestE;
  Superficie *exp_sup = new Superficie;
  for (int i = 0; i < cant_colores; i++){
    switch(i){
      case 0:
        exp_sup->cargar(path + "rojo.png");
        break;
      case 1:
        exp_sup->cargar(path + "amarillo.png");
        break;
      case 2:
        exp_sup->cargar(path + "verde.png");
        break;
      case 3:
        exp_sup->cargar(path + "azul.png");
        break;
      case 4:
        exp_sup->cargar(path + "violeta.png");
        break;
    }
    SDL_Color color_exp;
    exp_sup->color_pixel(0,0, color_exp);
    exp_sup->color_clave(color_exp);
    
    SrcE.x = 0;
    SrcE.w = 240;
    SrcE.h = 60;
    DestE.x = 0;
    DestE.w = 60;
    DestE.h = 60;    
    exp.desplazamiento_x = 60;
    exp.desplazamiento_y = 0;

    DestE.y = 0;
    SrcE.y = 0;
    exp.dimension_total = SrcE;
    exp.imagen_inicial = DestE;

    animaciones[0][i]->textura->cargar_textura(exp_sup, ventana);
    animaciones[0][i]->animacion->cargar_sprite(exp);
    animaciones[0][i]->animacion->establecer_fps(FPS_ANIMACION);
    
    SrcE.y = 60;
    DestE.y = 60;
    exp.dimension_total = SrcE;
    exp.imagen_inicial = DestE;

    animaciones[1][i]->textura->cargar_textura(exp_sup, ventana);
    animaciones[1][i]->animacion->cargar_sprite(exp);
    animaciones[1][i]->animacion->establecer_fps(FPS_ANIMACION);


    SrcE.y = 120;
    DestE.y = 120;
    exp.dimension_total = SrcE;
    exp.imagen_inicial = DestE;

    animaciones[2][i]->textura->cargar_textura(exp_sup, ventana);
    animaciones[2][i]->animacion->cargar_sprite(exp);
    animaciones[2][i]->animacion->establecer_fps(FPS_ANIMACION);
  }
  
  exp_sup->cargar(path + "estrella.png");
  SDL_Color color_exp;
  exp_sup->color_pixel(0,0, color_exp);
  exp_sup->color_clave(color_exp);
  
  SrcE.x = 0;
  SrcE.w = 355;
  SrcE.h = 40;
  DestE.x = 0;
  DestE.w = 32;
  DestE.h = 40;    
  exp.desplazamiento_x = 32;
  exp.desplazamiento_y = 0;

  DestE.y = 0;
  SrcE.y = 0;
  exp.dimension_total = SrcE;
  exp.imagen_inicial = DestE;

  estrella->textura->cargar_textura(exp_sup, ventana);
  estrella->animacion->cargar_sprite(exp);
  estrella->animacion->establecer_fps(FPS_ANIMACION);
  
  delete exp_sup;
}
  
//
void Productos::animar(){
  estrella->animacion->animar();
  if (estrella->animacion->fuera_del_sprite())
    estrella->animacion->reiniciar();
  
  for (int x = 0; x < cant_tipos; x++){
    for (int y = 0; y < cant_colores; y++){
      animaciones[x][y]->animacion->animar();
      if (animaciones[x][y]->animacion->fuera_del_sprite())
        animaciones[x][y]->animacion->reiniciar();
    }
  }  
}

//
Animacion *Productos::ver_animacion(int tipo, int color){
  if (tipo > cant_tipos || tipo < 0 || color < 0 || color >= cant_colores)
    return NULL;
  if (tipo == cant_tipos) return estrella->animacion;
  return animaciones[tipo][color]->animacion;
}

//
Textura *Productos::ver_textura(int tipo, int color){
  if (tipo > cant_tipos || tipo < 0 || color < 0 || color >= cant_colores)
    return NULL;
  if (tipo == cant_tipos) return estrella->textura;
  return animaciones[tipo][color]->textura;
}
