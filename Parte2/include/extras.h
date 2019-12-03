// Segunda parte del Proyecto de Sistemas Operativos
// extras.c

// Nombre: John Aldo Cuesta Agila
// Paralelo: 2
// Fecha de entrega: 3 de Diciembre del 2019

#include <stdlib.h>
#include <stdio.h>

// Estructura de datos que maneja lo siguiente:
// file_name: Puntero que contiene el nombre del archivo
// start: Donde debe apuntar el puntero de lectura en el archivo
// blockSize: Valor que indica el tamaño del chunk
// lines: Cantidad de líneas encontradas en el chunk
// words: Cantidad de palabras encontradas en el chunk
// bytes: Cantidad de bytes encontrados en el chunk
// nul_byte_seen: Indica si el caracter nulo existe o no en el chunk
// end: Indica si es el último chunk (que contiene la parte final del archivo)
// start_char: Caracter con el cual comienza el chunk
// end_char: Caracter con el cual finaliza el chunk
typedef struct thread_data {
    char *file_name;
    long int offset;
    long int start;
    long int blockSize;
    long int lines;
    long int words;
    long int bytes;
    char nul_byte_seen;
    short int end;
    char start_char;
    char end_char;
} thread_data ;

// Función que muestra el reporte del comando TWC con el conteo realizado
void reporteTWC(int max_width,
                int linesOnly, 
                int wordsOnly,
                int lines,
                int words,
                int bytes,
                char *filename);