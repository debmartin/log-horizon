#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -lSDL2 -lSDL2_mixer -lSDL2_image
#Nombre del programa principal
CEXE = prueba


all: $(CEXE)


ventana.o: SDL/ventana.h SDL/ventana.cpp
	$(CC) $(CFLAGS) -c SDL/ventana.cpp


animacion.o: SDL/animacion.h SDL/animacion.cpp
	$(CC) $(CFLAGS) -c SDL/animacion.cpp


superficie.o: SDL/superficie.h SDL/superficie.cpp
	$(CC) $(CFLAGS) -c SDL/superficie.cpp


fps.o: SDL/fps.h SDL/fps.cpp
	$(CC) $(CFLAGS) -c SDL/fps.cpp


matriz.o: matriz.h matriz.cpp
	$(CC) $(CFLAGS) -c matriz.cpp


nivel.o: nivel.h nivel.cpp
	$(CC) $(CFLAGS) -c nivel.cpp


$(CEXE): ventana.o superficie.o animacion.o fps.o matriz.o nivel.o $(CEXE).cpp
	$(CC) $(CFLAGS) ventana.o superficie.o animacion.o fps.o matriz.o nivel.o $(CEXE).cpp -o $(CEXE)


clean:
	-rm *.o


cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
