#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread -L./lib ../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = main

all: $(CEXE) 

ventana.o: ../libs/SDL2/ventana/ventana.h ../libs/SDL2/ventana/ventana.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/ventana/ventana.cpp

superficie.o: ../libs/SDL2/superficie/superficie.h ../libs/SDL2/superficie/superficie.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/superficie/superficie.cpp
	
boton.o: ../libs/SDL2/boton/boton.h ../libs/SDL2/boton/boton.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/boton/boton.cpp
	
fps.o: ../libs/SDL2/fps/fps.h ../libs/SDL2/fps/fps.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/fps/fps.cpp

texto.o: ../libs/SDL2/texto/texto.h ../libs/SDL2/texto/texto.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/texto/texto.cpp

text_box.o: ../libs/SDL2/text_box/text_box.h ../libs/SDL2/text_box/text_box.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/text_box/text_box.cpp

mensaje.o: ../libs/SDL2/mensaje/mensaje.h ../libs/SDL2/mensaje/mensaje.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/mensaje/mensaje.cpp
	
socket.o: ../libs/TDA/socket/socket.h ../libs/TDA/socket/socket.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/socket/socket.cpp
	
thread.o: ../libs/TDA/thread/thread.h ../libs/TDA/thread/thread.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/thread/thread.cpp
	
editor.o: editor.h editor.cpp
	$(CC) $(CFLAGS) -c editor.cpp

creador_nivel.o: creador_nivel.h creador_nivel.cpp
	$(CC) $(CFLAGS) -c creador_nivel.cpp
	
editor_nivel.o: editor_nivel.h editor_nivel.cpp
	$(CC) $(CFLAGS) -c editor_nivel.cpp

$(CEXE): editor_nivel.o creador_nivel.o editor.o thread.o socket.o ventana.o superficie.o texto.o boton.o text_box.o fps.o mensaje.o $(CEXE).cpp
	$(CC) $(CFLAGS) editor_nivel.o creador_nivel.o editor.o thread.o socket.o ventana.o superficie.o texto.o boton.o text_box.o fps.o mensaje.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
