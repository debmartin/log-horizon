#Compilador
CC = python
#Opciones usadas para compilar
CFLAGS = platform=linux-gcc check
#Nombre del programa principal
CEXE = scons.py

all: $(CEXE)
	$(CC) $(CEXE) $(CFLAGS)

clean:
	-rm -r bin
	-rm -r buildscons
	-rm -r libs
	
