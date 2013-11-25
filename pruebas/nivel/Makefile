#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread -L./lib ../../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = prueba

all: prueba_enviar $(CEXE) 

ventana.o: ../../libs/SDL2/ventana/ventana.h ../../libs/SDL2/ventana/ventana.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/ventana/ventana.cpp

animacion.o: ../../libs/SDL2/animacion/animacion.h ../../libs/SDL2/animacion/animacion.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/animacion/animacion.cpp

superficie.o: ../../libs/SDL2/superficie/superficie.h ../../libs/SDL2/superficie/superficie.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/superficie/superficie.cpp
	
text_box.o: ../../libs/SDL2/text_box/text_box.h ../../libs/SDL2/text_box/text_box.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/text_box/text_box.cpp

fps.o: ../../libs/SDL2/fps/fps.h ../../libs/SDL2/fps/fps.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/fps/fps.cpp
	
texto.o: ../../libs/SDL2/texto/texto.h ../../libs/SDL2/texto/texto.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/texto/texto.cpp

matriz.o: ../../cliente/Nivel/matriz.h ../../cliente/Nivel/matriz.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/matriz.cpp

nivel.o: ../../cliente/Nivel/nivel.h ../../cliente/Nivel/nivel.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/nivel.cpp
	
explosion.o: ../../cliente/Nivel/explosion.h ../../cliente/Nivel/explosion.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/explosion.cpp
	
celdas_vacias.o: ../../cliente/Nivel/celdas_vacias.h ../../cliente/Nivel/celdas_vacias.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/celdas_vacias.cpp

productos.o: ../../cliente/Nivel/productos.h ../../cliente/Nivel/productos.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/productos.cpp
	
receptor_resultados.o: ../../cliente/Nivel/receptor_resultados.h ../../cliente/Nivel/receptor_resultados.cpp
	$(CC) $(CFLAGS) -c ../../cliente/Nivel/receptor_resultados.cpp

socket.o: ../../libs/TDA/socket/socket.h ../../libs/TDA/socket/socket.cpp
	$(CC) $(CFLAGS) -c ../../libs/TDA/socket/socket.cpp
	
thread.o: ../../libs/TDA/thread/thread.h ../../libs/TDA/thread/thread.cpp
	$(CC) $(CFLAGS) -c ../../libs/TDA/thread/thread.cpp


$(CEXE): ventana.o superficie.o animacion.o fps.o texto.o text_box.o matriz.o nivel.o explosion.o celdas_vacias.o productos.o receptor_resultados.o socket.o thread.o $(CEXE).cpp
	$(CC) $(CFLAGS) ventana.o superficie.o animacion.o fps.o texto.o text_box.o matriz.o nivel.o explosion.o celdas_vacias.o productos.o receptor_resultados.o socket.o thread.o $(CEXE).cpp -o $(CEXE)

prueba_enviar: receptor_resultados.o socket.o thread.o prueba_enviar_datos.cpp
	$(CC) $(CFLAGS) receptor_resultados.o socket.o thread.o prueba_enviar_datos.cpp -o prueba_enviar

json:
	cd ../../libs/json/; make

clean:
	-rm *.o


cleanAll: clean
	-rm $(CEXE) prueba_enviar

run: all
	./prueba_enviar	

test: $(CEXE)
	valgrind ./$(CEXE)
