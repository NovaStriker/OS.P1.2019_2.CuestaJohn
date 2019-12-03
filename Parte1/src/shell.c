// Primera parte del Proyecto de Sistemas Operativos
// shell.c

// Nombre: John Aldo Cuesta Agila
// Paralelo: 2
// Fecha de entrega: 3 de Diciembre del 2019

// Descripción de script
// Este código cumple como ejecución principal del programa en el que
// se desarrollan todas las operaciones, funciones y se validan los
// posibles casos por los que atraviese el programa.

// El código escrito a continuación crea una consola de comandos que
// debe manejar dos comandos específicos: exit y cd.
// Estos mantienen su funcionalidad similar al que deben tener si son
// usados en una consola de un sistema operativo cualquiera.

// El comando cd mantiene un formato que muestra la matrícula del
// estudiante como parte de la salida por pantalla de la consola.

// El programa debe validar los posibles escenarios para mantener el
// correcto funcionamiento del programa y sus procesos.

// Importaciones necesarias
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wordexp.h>

// Importación de librería adicional propia
#include <extras.h>

int main(int argc, char* argv[]) {

    // Se establece la longitud de la cadena ingresada como directorio actual
    // Si la cadena de longitud nula se emite un error al no ser traído correctamente
    char cwd[MAXIMO_COMANDO];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Las salidas por pantalla muestran un formato con la inclusión de la matrícula del estudiante
        printf("SO<201017936>sh%%: %s$ ", cwd);
        //Se declara la variable "valorDato"
        char valorDato[MAXIMO_COMANDO];
        //En la variable "valorDato" se va a almacenar lo que el usuario escriba en la consola de entrada
        fgets(valorDato, MAXIMO_COMANDO, stdin);

        // Si la longitud de la cadena "valorDato" es mayor a cero y, también, si termina en un salto de línea entonces
        if ((strlen(valorDato) > 0) && (valorDato[strlen(valorDato) - 1] == '\n')) {
            // Se reemplaza el salto de línea por un caracter vacío
            valorDato[strlen(valorDato) - 1] = '\0';
        }

        // Se realiza la validación de 
        char *limpio = strstrip(valorDato);

        // Si el comando ingresado es "exit" entonces se finaliza la ejecución del programa y terminan todos los hilos
        if (strcmp(limpio, "exit") == 0) {
            printf("Exit from shell\n");
            return 0;
        }

        // Aquí una cadena puede limpiarse de las tabulaciones mediante un reemplazamiento con un espacio en blanco
        char *limpioTabs = str_replace(limpio, "\t", " ");

        // Se separan los comandos en una lista para almacenarlos
        char **tokens = str_split(limpioTabs, ' ');

        // Se evalúa la cadena para saber si es el comando "cd"
        if (strcmp(*(tokens), "cd") == 0) {
            int i;
            // Se declara una estructura para almacenar expresiones que puedan utilizarse como
            // comandos
            wordexp_t exp_result;
            // Se cuenta cuántos tokens hay
            for (i = 0; *(tokens + i); i++);
            // Se validan los escenarios en donde el comando cd nos redirige al directorio home
            switch (i) {
                // Si solo se ingresa el comando "cd" y nada más entonces la expresión se reemplaza
                // por la expresión con virgulilla que nos redirige al directorio home
                case 1:
                    wordexp("~", &exp_result, 0);
                    *(tokens + 1) = strdup(exp_result.we_wordv[0]);
                    break;
                
                // Si se escribe el comando "cd" y adicionalmente se escribe la virgulilla entonces
                // se procede a redigir al directorio home
                case 2:
                    if (strcmp(*(tokens + 1), "~") == 0) {
                        wordexp("~", &exp_result, 0);
                        *(tokens + 1) = strdup(exp_result.we_wordv[0]);
                    }
                    break;
            }

            // Si hay más palabras ingresadas se emite el mensaje de error correspondiente
            if (i > 2) {
                printf("sh: cd: Too many arguments\n");

            // Si hay menos de dos palabras se procede a validar la entrada 
            } else {
                // Se realiza el cambio de directorio, si el directorio no existe entonces se
                // emite el error
                if(chdir(*(tokens + 1))== -1) {
                    char buffer[MAXIMO_COMANDO];
                    sprintf(buffer, "sh: cd: %s", *(tokens + 1)); 
                    perror(buffer);
                };
                // Si no se puede cambiar de directorio se emite el siguiente error
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    perror("getcwd() error");
            }
        // Si la cadena ingresada es diferente del comando "cd" 
        } else {
            // El proceso hijo ejecuta el comando desconocido sin éxito
            if (fork() == 0) {
                execv(*(tokens), tokens);
                exit(-1);
            // Si no, entonces se espera a que los procesos hijos terminen
            } else { 
                wait(NULL);
            }
        }
    }
}
