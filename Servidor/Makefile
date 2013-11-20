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


dimension.o: dimension.h dimension.cpp
	$(CC) $(CFLAGS) -c dimension.cpp


FueraDeRango.o: FueraDeRango.h FueraDeRango.cpp
	$(CC) $(CFLAGS) -c FueraDeRango.cpp


Jugada.o: Jugada.h Jugada.cpp
	$(CC) $(CFLAGS) -c Jugada.cpp
	
Nivel.o: Nivel.h Nivel.cpp
	$(CC) $(CFLAGS) -c Nivel.cpp
	
Tablero.o: Tablero.h Tablero.cpp
	$(CC) $(CFLAGS) -c Tablero.cpp

emisor_resultados.o: emisor_resultados.h emisor_resultados.cpp
	$(CC) $(CFLAGS) -c emisor_resultados.cpp
	
thread.o: thread.h thread.cpp
	$(CC) $(CFLAGS) -c thread.cpp
	
socket.o: socket.h socket.cpp
	$(CC) $(CFLAGS) -c socket.cpp

ThreadJugador.o: ThreadJugador.h ThreadJugador.cpp
	$(CC) $(CFLAGS) -c ThreadJugador.cpp

$(CEXE): Celda.o Jugador.o dimension.o FueraDeRango.o Jugada.o Nivel.o Tablero.o emisor_resultados.o thread.o socket.o ThreadJugador.o $(CEXE).cpp
	$(CC) $(CFLAGS) Celda.o Jugador.o dimension.o FueraDeRango.o Jugada.o Nivel.o Tablero.o emisor_resultados.o thread.o socket.o ThreadJugador.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
