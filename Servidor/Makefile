#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -pthread
#Nombre del programa principal
CEXE = prueba


all: $(CEXE)


Celda.o: Celda.h Celda.cpp
	$(CC) $(CFLAGS) -c Celda.cpp


Jugador.o: Jugador.h Jugador.cpp
	$(CC) $(CFLAGS) -c Jugador.cpp


Matriz.o: Matriz.h Matriz.cpp
	$(CC) $(CFLAGS) -c Matriz.cpp


dimension.o: dimension.h dimension.cpp
	$(CC) $(CFLAGS) -c dimension.cpp


FueraDeRango.o: FueraDeRango.h mFueraDeRango.cpp
	$(CC) $(CFLAGS) -c FueraDeRango.cpp


Jugada.o: Jugada.h Jugada.cpp
	$(CC) $(CFLAGS) -c Jugada.cpp
	
Nivel.o: Nivel.h Nivel.cpp
	$(CC) $(CFLAGS) -c Nivel.cpp
	
Tablero.o: Tablero.h Tablero.cpp
	$(CC) $(CFLAGS) -c Tablero.cpp

$(CEXE): Celda.o Jugador.o Matriz.o dimension.o FueraDeRango.o Jugada.o Nivel.o Tablero.o $(CEXE).cpp
	$(CC) $(CFLAGS) Celda.o Jugador.o Matriz.o dimension.o FueraDeRango.o Jugada.o Nivel.o Tablero.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
