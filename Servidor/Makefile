#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -pthread -L./lib ../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = prueba


all: $(CEXE)


Celda.o: Celda.h Celda.cpp
	$(CC) $(CFLAGS) -c Celda.cpp


Jugador.o: Jugador.h Jugador.cpp
	$(CC) $(CFLAGS) -c Jugador.cpp


dimension.o: dimension.h dimension.cpp
	$(CC) $(CFLAGS) -c dimension.cpp

Jugada.o: Jugada.h Jugada.cpp
	$(CC) $(CFLAGS) -c Jugada.cpp
	
Nivel.o: Nivel.h Nivel.cpp
	$(CC) $(CFLAGS) -c Nivel.cpp
	
Tablero.o: Tablero.h Tablero.cpp
	$(CC) $(CFLAGS) -c Tablero.cpp

emisor_resultados.o: emisor_resultados.h emisor_resultados.cpp
	$(CC) $(CFLAGS) -c emisor_resultados.cpp
	
EmisorTablero.o: EmisorTablero.h EmisorTablero.cpp
	$(CC) $(CFLAGS) -c EmisorTablero.cpp
	
ReceptorJugada.o: ReceptorJugada.h ReceptorJugada.cpp
	$(CC) $(CFLAGS) -c ReceptorJugada.cpp
	
ConjuntoJugadores.o: ConjuntoJugadores.h ConjuntoJugadores.cpp
	$(CC) $(CFLAGS) -c ConjuntoJugadores.cpp

Servidor.o: Servidor.h Servidor.cpp
	$(CC) $(CFLAGS) -c Servidor.cpp
	
Login.o: Login.h Login.cpp
	$(CC) $(CFLAGS) -c Login.cpp

SalaPartidas.o: SalaPartidas.h SalaPartidas.cpp
	$(CC) $(CFLAGS) -c SalaPartidas.cpp
	
ArchivoDirecto.o: ArchivoDirecto.h ArchivoDirecto.cpp
	$(CC) $(CFLAGS) -c ArchivoDirecto.cpp

funciones_auxiliares.o: funciones_auxiliares.h funciones_auxiliares.cpp
	$(CC) $(CFLAGS) -c funciones_auxiliares.cpp
	
thread.o: ../libs/TDA/thread/thread.h ../libs/TDA/thread/thread.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/thread/thread.cpp
	
socket.o: ../libs/TDA/socket/socket.h ../libs/TDA/socket/socket.cpp
	$(CC) $(CFLAGS) -c ../libs/TDA/socket/socket.cpp

$(CEXE): Celda.o ArchivoDirecto.o Jugador.o dimension.o Servidor.o Jugada.o Nivel.o EmisorTablero.o ReceptorJugada.o funciones_auxiliares.o SalaPartidas.o ConjuntoJugadores.o Tablero.o emisor_resultados.o Login.o thread.o socket.o Servidor.o $(CEXE).cpp
	$(CC) $(CFLAGS) Celda.o Jugador.o ArchivoDirecto.o dimension.o Jugada.o Nivel.o funciones_auxiliares.o SalaPartidas.o EmisorTablero.o ReceptorJugada.o ConjuntoJugadores.o Tablero.o emisor_resultados.o thread.o Login.o socket.o Servidor.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
	
