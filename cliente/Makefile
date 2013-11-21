#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread
#Nombre del programa principal
CEXE = prueba


all: $(CEXE)


ventana.o: ../libs/SDL2/ventana/ventana.h ../libs/SDL2/ventana/ventana.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/ventana/ventana.cpp


animacion.o: ../libs/SDL2/animacion/animacion.h ../libs/SDL2/animacion/animacion.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/animacion/animacion.cpp


superficie.o: ../libs/SDL2/superficie/superficie.h ../libs/SDL2/superficie/superficie.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/superficie/superficie.cpp


fps.o: ../libs/SDL2/fps/fps.h ../libs/SDL2/fps/fps.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/fps/fps.cpp
	
texto.o: ../libs/SDL2/texto/texto.h ../libs/SDL2/texto/texto.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/texto/texto.cpp


matriz.o: matriz.h matriz.cpp
	$(CC) $(CFLAGS) -c matriz.cpp


nivel.o: nivel.h nivel.cpp
	$(CC) $(CFLAGS) -c nivel.cpp
	
explosion.o: explosion.h explosion.cpp
	$(CC) $(CFLAGS) -c explosion.cpp
	
celdas_vacias.o: celdas_vacias.h celdas_vacias.cpp
	$(CC) $(CFLAGS) -c celdas_vacias.cpp

productos.o: productos.h productos.cpp
	$(CC) $(CFLAGS) -c productos.cpp
	
receptor_resultados.o: receptor_resultados.h receptor_resultados.cpp
	$(CC) $(CFLAGS) -c receptor_resultados.cpp

socket.o: socket.h socket.cpp
	$(CC) $(CFLAGS) -c socket.cpp
	
thread.o: thread.h thread.cpp
	$(CC) $(CFLAGS) -c thread.cpp

$(CEXE): ventana.o superficie.o animacion.o fps.o texto.o matriz.o nivel.o explosion.o celdas_vacias.o productos.o receptor_resultados.o socket.o thread.o $(CEXE).cpp
	$(CC) $(CFLAGS) ventana.o superficie.o animacion.o fps.o texto.o matriz.o nivel.o explosion.o celdas_vacias.o productos.o receptor_resultados.o socket.o thread.o $(CEXE).cpp -o $(CEXE)

prueba_enviar: receptor_resultados.o socket.o thread.o prueba_enviar_datos.cpp
	$(CC) $(CFLAGS) receptor_resultados.o socket.o thread.o prueba_enviar_datos.cpp -o prueba_enviar



clean:
	-rm *.o


cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
