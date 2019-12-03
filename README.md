# OS.P1.2019_2.CuestaJohn
Proyecto de Sistemas Operativos - Primer parcial - Segundo término 2019

PARTE 1:

DESCRIPCIÓN GENERAL DEL DISEÑO

Esta es una estructura genérica en donde se tiene la carpeta de los ejecutables (bin) generados por el 
MAKEFILE, la carpeta include están los archivos headers para el código adicional que se necesita. En la
carpeta lib se encuentran los archivos customizables enlazados a los archivos headers realizados en la
carpeta include. La carpeta src tiene el código principal que ejecuta el programa.



ESPECIFICACIÓN COMPLETA

El planteamiento de la solución para esta parte del proyecto es el de mantener activo un proceso padre, 
es decir, la consola como tal. Además, se deben ejecutar procesos hijos, o los comandos de la consola, 
que deben ejecutar sus propias tareas.

Mientras los procesos hijos se crean y se ejecutan, el proceso padre debe mantenerse vigente. Cuando 
finalmente el proceso hijo termina, el proceso padre espera para mostrar el resultado del proceso hijo.

Cuando el shell está ejecutando espera una entrada para dirigirse a ese directorio. En el caso de ser
un espacio se lo maneja de manera que no se ha ingresaso una cadena válida. Si el comando tiene una
virgulilla y nada más se redirige al home del usuario. Todo esto se maneja con validaciones de la entrada
de las cadenas.

Finalmente, cuando el proceso padre termina todo lo que se está ejecutando se cierra en ese momento.



ERRORES O PROBLEMAS
> No se aceptan pipes
> No se aceptan directorios que tengan un espacio en el nombre






PARTE 2:

DESCRIPCIÓN GENERAL DEL DISEÑO
Esta es una estructura genérica en donde se tiene la carpeta de los ejecutables (bin) generados por el 
MAKEFILE, la carpeta include están los archivos headers para el código adicional que se necesita. En la
carpeta lib se encuentran los archivos customizables enlazados a los archivos headers realizados en la
carpeta include. La carpeta src tiene el código principal que ejecuta el programa.



ESPECIFICACIÓN COMPLETA

Para la solución se decide a particionar el archivo en chunks, estos serán porciones del archivo que 
dependen de la cantidad de hilos que se utilicen. Por ejemplo, si se tienen 4 hilos se tendrán 4 chunks. 
La cantidad de hilos se definen en “pthread_create”.

Para realizar el conteo de líneas se busca el carácter “\n” o salto de línea, dependiendo de la cantidad 
de estos caracteres se obtiene la cantidad de líneas.

Para realizar el conteo de palabras se utilizan punteros para saber si el carácter actual es diferente de 
espacio y si el carácter siguiente es un espacio. Si estas condiciones se dan, entonces se considera que 
es una palabra. Para verificar si hay una palabra en los límites de los chunks se utiliza la misma 
estrategia. Este algoritmo sigue la base del “wc” genérico con algunas modificaciones.

Dado que no hay operaciones de escritura, y se utilizan diferentes punteros para leer el archivo, ya que 
para cada hilo se obtiene una dirección de memoria diferente, por ende cada puntero es independiente uno 
del otro y no se manejan zonas críticas, por lo que no hubo la necesidad de utilizar la exclusión mutua.

Esto se debe a que se realiza una lectura y no realiza conflictos o condiciones de carrera. Cuando se 
manejan multihilos se divide la longitud del archivo para la cantidad de hilos totales. El último chunk 
es el que va a tener más de la participación o menos de la participación.

Esto depende de la división ya que la división en C es de paquetes enteros. Cuando se tenga el tamaño del 
bloque, cada hilo se inicializa en estado “joineable” para poder esperar los hilos que van contando y 
cuando terminen se puedan juntar con los demás hilos.

Cuando se tengan los chunks contados al final, se cuentan las partes que se pudieron saltar. Con la 
validación se maneja los caracteres de inicio y fin para controlar si lo que se tiene es una palabra. De 
manera global sólo se acumulan las particiones para dar el resultado.

Este algoritmo es eficiente ya que no carga en memoria más de lo necesario. Esto se refleja especialmente 
en archivos muy grandes. Al contrario de utilizar, por ejemplo, mmap para separar espacio en la RAM 
entonces  se está utilizando más memoria para el archivo de gran contenido.



ERRORES O PROBLEMAS
> Si hay una cantidad muy elevada de hilos no va a funcionar correctamente la partición que se hace.


