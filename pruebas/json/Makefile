#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -L./lib ../../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = prueba

all: json $(CEXE) prueba_leer 

$(CEXE): json prueba_leer $(CEXE).cpp
	$(CC) $(CFLAGS)  $(CEXE).cpp -o $(CEXE)

prueba_leer: prueba_leer
	$(CC) $(CFLAGS) prueba_leer.cpp -o prueba_leer

json:
	cd ../../libs/json/; make

clean:
	-rm $(CEXE)
	-rm prueba_leer
	

cleanAll: clean
	-rm *.dat
	cd ../../libs/json/; make clean
	
run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
