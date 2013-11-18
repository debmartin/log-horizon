#include <iostream>
#include "productos.h"

#define FPS_ANIMACION 8.0f

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
        exp_sup->cargar(path + "imagenes/rojo.png");
        break;
      case 1:
        exp_sup->cargar(path + "imagenes/amarillo.png");
        break;
      case 2:
        exp_sup->cargar(path + "imagenes/verde.png");
        break;
      case 3:
        exp_sup->cargar(path + "imagenes/azul.png");
        break;
      case 4:
        exp_sup->cargar(path + "imagenes/violeta.png");
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
  
  exp_sup->cargar(path + "imagenes/estrella.png");
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
