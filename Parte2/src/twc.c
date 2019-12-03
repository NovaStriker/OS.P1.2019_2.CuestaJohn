// Segunda parte del Proyecto de Sistemas Operativos
// twc.c

// Nombre: John Aldo Cuesta Agila
// Paralelo: 2
// Fecha de entrega: 3 de Diciembre del 2019

// Descripción de script
// Para la solución se decide a particionar el archivo en chunks, estos serán porciones del 
// archivo que dependen de la cantidad de hilos que se utilicen. Por ejemplo, si se tienen 4 
// hilos se tendrán 4 chunks. La cantidad de hilos se definen en “pthread_create”.

// Para realizar el conteo de líneas se busca el carácter “\n” o salto de línea, dependiendo de 
// la cantidad de estos caracteres se obtiene la cantidad de líneas.

// Para realizar el conteo de palabras se utilizan punteros para saber si el carácter actual es 
// diferente de espacio y si el carácter siguiente es un espacio. Si estas condiciones se dan,
// entonces se considera que es una palabra. Para verificar si hay una palabra en los límites
// de los chunks se utiliza la misma estrategia. Este algoritmo sigue la base del “wc” genérico 
// con algunas modificaciones.

// Dado que no hay operaciones de escritura, y se utilizan diferentes punteros para leer el
// archivo, ya que para cada hilo se obtiene una dirección de memoria diferente, por ende
// cada puntero es independiente uno del otro y no se manejan zonas críticas, por lo que no
// hubo la necesidad de utilizar la exclusión mutua.

// Esto se debe a que se realiza una lectura y no realiza conflictos o condiciones de carrera.
// Cuando se manejan multihilos se divide la longitud del archivo para la cantidad de hilos
// totales. El último chunk es el que va a tener más de la participación o menos de la
// participación.

// Esto depende de la división ya que la división en C es de paquetes enteros. Cuando se 
// tenga el tamaño del bloque, cada hilo se inicializa en estado “joineable” para poder esperar
// los hilos que van contando y cuando terminen se puedan juntar con los demás hilos.

// Cuando se tengan los chunks contados al final, se cuentan las partes que se pudieron
// saltar. Con la validación se maneja los caracteres de inicio y fin para controlar si lo que se 
// tiene es una palabra. De manera global sólo se acumulan las particiones para dar el 
// resultado.

// Este algoritmo es eficiente ya que no carga en memoria más de lo necesario. Esto se refleja
// especialmente en archivos muy grandes. Al contrario de utilizar, por ejemplo, mmap para 
// separar espacio en la RAM entonces  se está utilizando más memoria para el archivo de 
// gran contenido.

// Importaciones necesarias
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

// Importación propia
#include <extras.h>

#define MAX_WIDTH 7

// Función que el hilo maneja para el conteo respectivo
// de líneas, palabras y bytes
void *thread_wc(void *data) {
    thread_data* td = (thread_data *)data;
    char *buffer = (char *)malloc(td->blockSize);
    char c, c_after;

    FILE *fp = fopen(td->file_name, "r");
    
    fseek(fp, td->start, SEEK_SET);
    fread(buffer, td->blockSize, 1, fp);
   
    // Algoritmo tomado de una muestra de wc.c con ciertas modificaciones
    // para el manejo de los chunks.
    // Se toma el caracter actual y el siguiente, en donde se verifica que
    // si el caracter actual no es un espacio y el siguiente sí, se considera
    // que se encontró una palabra.
    // si el caracter actual es un salto de línea, se incrementa el contador de
    // líneas del chunk

    int ind;
    for (ind = 0; ind < td->blockSize - 1; ind++) {
        c = buffer[ind];
        if (ind == 0) {
            td->start_char = c;
        }
        c_after = buffer[ind + 1];
        td->bytes++;
        
        if (c == '\n') {
            td->lines++;
        }

        if (!isspace(c) && isspace(c_after)) {
            td->words++;
        }

        if(c == '\0'){
            td->words++;
            td->nul_byte_seen = 1;
        }
    }

    // Esta parte es para el último caracter que se lo dejó suelto
    // en el lazo for anterior
    td->bytes++;
    
    c = buffer[ind];
    td->end_char = c;
    c_after = buffer[ind + 1];
    if (c == '\n') {
        td->lines++;
    }

    // Esto aplica para el chunk final
    if (!isspace(c) && td->end) {
        td->words++;
    }
    // Esto aplica para el chunk final
    if(td->words == 0 && td->bytes > 0 && !isspace(c) && td->end) {
        td->words++;
    }
    
    fclose(fp);
    
    buffer = NULL;
    free(buffer);

    pthread_exit(NULL);
    return NULL;
}

int main (int argc, char *argv[]) {

    // Validando en caso que no existan la cantidad suficiente de argumentos
    if (argc < 2) {
        printf("twc [-lw] [file]\n");
        return 0;
    }

    // Variables que manejarán el caso de:
    // Solo palabras, solo lineas, existe una opción inválida
    // y ambas (palabras y líneas)
    int wordOnly, linesOnly, notValidOption;

    char opt;

    // Como se requirió que se manejen solo dos opciones permitidas
    // por el comando, se procedió a utilizar la función getopt la cual
    // ya realiza la validación respectiva de las opciones que se pasan
    // Cuando la opción parseada es ?, significa que se encontró una opción
    // no válida, y se setea la variable respectiva para seguir con la ejecución
    // del código
    while((opt = getopt(argc, argv,"lw")) != -1) {
        switch (opt) {
            case 'l':
                linesOnly = 1;
                break;
            case 'w':
                wordOnly = 1;
                break;
            case '?':
                notValidOption = 1;
                break;
            
        } 
    }

    // Pregunto si todas las opciones son válidas
    if (!notValidOption) {
        int id, blockSize, longitud;

        // Variables globales que mantienen el conteo total de todos los archivos
        int gLines = 0;
        int gWords = 0;
        int gBytes = 0;

        int MAX_THREADS_PER_FILE = 4;

        for (; optind < argc; optind++) {
            
            FILE *input = fopen(argv[optind], "r");
            if (input) {
                // Se necesita conocer la cantidad de bytes del archivo para poder 
                // dividirlo en chunks
                fseek(input, 0, SEEK_END);
                longitud = ftell(input);
                rewind(input);
                
                // Se obtiene el tamaño de cada chunk en base a la cantidad de hilos
                // que se define para cada archivo
                blockSize = longitud / MAX_THREADS_PER_FILE;

                // Se valida en caso que el chunk sea demasiado pequeño y no cumpla con
                // verdadero proposito
                while (blockSize < 1) {
                    MAX_THREADS_PER_FILE /= 4;
                    blockSize = longitud / MAX_THREADS_PER_FILE;
                }

                // Estructuras que manejan los chunks e hilos respectivamente
                thread_data *data[MAX_THREADS_PER_FILE];
                pthread_t threads[MAX_THREADS_PER_FILE];

                for(id = 0; id < MAX_THREADS_PER_FILE; id++){
                    data[id] = (thread_data *)malloc(sizeof(thread_data));
                    data[id]->file_name = argv[optind];
                    data[id]->offset = blockSize;
                    data[id]->blockSize = blockSize;
                    data[id]->start = id * blockSize;

                    data[id]->lines = 0;
                    data[id]->words = 0;
                    data[id]->bytes = 0;

                    if (id != 0) {
                        data[id]->start++;
                        data[id]->blockSize;
                    }

                    // ÚLTIMO HILO
                    // Aquí se tiene la parte que el último chunk puede tener un tamaño mayor
                    // o menor dependiendo de la cantidad restante con el total de bytes que
                    // tiene el archivo
                    if (id == MAX_THREADS_PER_FILE - 1) {
                        data[id]->end = 1;
                        data[id]->blockSize = longitud + 1 - data[id]->start;
                        data[id]->offset = data[id]->blockSize;
                    }
                }

                // Se inicializan los hilos
                for(id = 0; id < MAX_THREADS_PER_FILE; id++) {
                    pthread_create(&threads[id], NULL, thread_wc, (void *)data[id]);
                }

                // Se espera a que todos los hilos hagan su trabajo para poder
                // acumular el valor total por archivo
                for(id = 0; id < MAX_THREADS_PER_FILE; id++) {
                    pthread_join(threads[id], NULL);
                }

                fclose(input);

                // Variables que mantienen el conteo de cada archivo y que van a
                // ser acumulados por los valores que cada hilo procesó
                int fLines = 0;
                int fWords = 0;
                int fBytes = 0;

                for (id = 0; id < MAX_THREADS_PER_FILE; id++) {
                    fLines = fLines + data[id]->lines;
                    fWords = fWords + data[id]->words;

                    // Aquí se verifica con el mismo algoritmo que se hizo en el hilo,
                    // para saber si el caracter fin del chunk anterior y el caracter
                    // inicial del chunk actual sean identificados como una palabra.
                    // Esto no es necesario en caso que la lectura del archivo se realice
                    // en un solo hilo
                    if (id > 0) {
                        if (!isspace(data[id - 1]->end_char) && isspace(data[id]->start_char)) {
                            fWords++;
                        }
                    }

                    fBytes = fBytes + data[id]->bytes;
                }
                
                gLines += fLines;
                gWords += fWords;
                gBytes += fBytes;

                // Aquí se muestra la información del conteo realizado por cada archivo
                reporteTWC(MAX_WIDTH, linesOnly, wordOnly, fLines, fWords, fBytes, argv[optind]);

                *data = NULL;
                free(*data);
            } else {
                // Esto se va a mostrar cuando el archivo no exista, es decir, no suma
                // a las variables globales de conteo.
                char buffer[8096];
                sprintf(buffer, "twc: %s", argv[optind]); 
                perror(buffer);
            }
        }

        // Aquí se muestra la información global del conteo realizado
        reporteTWC(MAX_WIDTH, linesOnly, wordOnly, gLines, gWords, gBytes, "total");
        
    }

    return 0;
}