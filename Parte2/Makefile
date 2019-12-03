# Este es el archivo Makefile que compilara todo el programa
CFLAGS=-lpthread -I./include

CC=gcc

DEPS=./include/extras.h
  
RCS=./lib/extras.c

MAIN=./src/twc.c 

# JWRITE = ./lib/libjwrite.a

# JREAD = ./lib/libjread.a

EJEC=./bin/twc

EJEC_CD=./bin/cd

#Llamada por defecto
all: twc

#Compila twc en ./bin/twc
twc: $(MAIN) $(DEPS) 
	$(CC) -g -static -o $(EJEC) $(MAIN) $(RCS) $(CFLAGS) 

# Limpiar archivos
clean:
	rm $(EJEC) 
	
# $(JWRITE) $(JREAD)