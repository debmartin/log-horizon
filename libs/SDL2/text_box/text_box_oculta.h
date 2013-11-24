#ifndef TEXT_BOX_OCULTA_H
#define TEXT_BOX_OCULTA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "text_box.h"
#include "../texto/texto.h"
#include "../ventana/ventana.h"
#include "../superficie/superficie.h"
#include "../text_box/text_box.h"

class TextBoxOculta : public TextBox{
  protected:
    //
    void append_mostrar(const char c);

    //
    void borrar_primero_mostrar();
};

#endif // TEXT_BOX_OCULTA_H

