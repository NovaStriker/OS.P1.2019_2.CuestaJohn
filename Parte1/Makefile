# Este es el archivo Makefile que compilara todo el programa
CFLAGS=-I./include

CC=gcc

DEPS=./include/extras.h
  
RCS=./lib/extras.c

MAIN=./src/shell.c 

CD=./src/cd.c

# JWRITE = ./lib/libjwrite.a

# JREAD = ./lib/libjread.a

EJEC=./bin/shell

EJEC_CD=./bin/cd

#Llamada por defecto
all: shell cd prueba

#Compila shell en ./bin/shell
shell: $(MAIN) $(DEPS) 
	$(CC) -g -static -o $(EJEC) $(MAIN) $(RCS) $(CFLAGS) 

#Compila shell en ./bin/cd
cd: $(CD) $(DEPS) 
	$(CC) -g -static -o $(EJEC_CD) $(CD) $(RCS) $(CFLAGS) 

prueba: ./src/prueba.c $(DEPS)
	$(CC) -g -static -o ./bin/prueba ./src/prueba.c $(RCS) $(CFLAGS)

# Limpiar archivos
clean:
	rm $(EJEC) 
	
# $(JWRITE) $(JREAD)