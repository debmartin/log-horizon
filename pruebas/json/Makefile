#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -L./lib ../../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = prueba

all: $(CEXE) 

$(CEXE): $(CEXE).cpp
	$(CC) $(CFLAGS)  $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o


cleanAll: clean
	-rm $(CEXE)
	-rm *.dat

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
