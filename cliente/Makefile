#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread -L./lib ../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = cliente

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

login.o: Login/login.h Login/login.cpp
	$(CC) $(CFLAGS) -c Login/login.cpp
	
sala.o: Sala/sala.h Sala/sala.cpp
	$(CC) $(CFLAGS) -c Sala/sala.cpp
	
seleccion.o: Sala/seleccion.h Sala/seleccion.cpp
	$(CC) $(CFLAGS) -c Sala/seleccion.cpp
	
socket.o: ../libs/TDA/socket/socket.h ../libs/TDA/socket/socket.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/socket/socket.cpp

animacion.o: ../libs/SDL2/animacion/animacion.h ../libs/SDL2/animacion/animacion.cpp
	$(CC) $(CFLAGS) -c ../libs/SDL2/animacion/animacion.cpp

matriz.o: Nivel/matriz.h Nivel/matriz.cpp
	$(CC) $(CFLAGS) -c Nivel/matriz.cpp

nivel.o: Nivel/nivel.h Nivel/nivel.cpp
	$(CC) $(CFLAGS) -c Nivel/nivel.cpp
	
explosion.o: Nivel/explosion.h Nivel/explosion.cpp
	$(CC) $(CFLAGS) -c Nivel/explosion.cpp
	
celdas_vacias.o: Nivel/celdas_vacias.h Nivel/celdas_vacias.cpp
	$(CC) $(CFLAGS) -c Nivel/celdas_vacias.cpp
	
puntaje.o: Nivel/puntaje.h Nivel/puntaje.cpp
	$(CC) $(CFLAGS) -c Nivel/puntaje.cpp

productos.o: Nivel/productos.h Nivel/productos.cpp
	$(CC) $(CFLAGS) -c Nivel/productos.cpp
	
receptor_resultados.o: Nivel/receptor_resultados.h Nivel/receptor_resultados.cpp
	$(CC) $(CFLAGS) -c Nivel/receptor_resultados.cpp
	
thread.o: ../libs/TDA/thread/thread.h ../libs/TDA/thread/thread.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/thread/thread.cpp

hda_online.o: hda_online.h hda_online.cpp
	$(CC) $(CFLAGS) -c hda_online.cpp

$(CEXE): hda_online.o animacion.o matriz.o nivel.o explosion.o celdas_vacias.o puntaje.o productos.o receptor_resultados.o thread.o seleccion.o socket.o ventana.o superficie.o  texto.o boton.o text_box.o fps.o mensaje.o login.o sala.o $(CEXE).cpp
	$(CC) $(CFLAGS) hda_online.o animacion.o matriz.o nivel.o explosion.o celdas_vacias.o puntaje.o productos.o receptor_resultados.o thread.o seleccion.o socket.o ventana.o superficie.o  texto.o boton.o text_box.o fps.o mensaje.o login.o sala.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
