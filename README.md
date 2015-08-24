## Descripción de los directorios y archivos
Dentro del directorio mrp se encuentra el Makefile y el directorio src con los archivos fuente de la aplicación.

## Instrucciones de compilación
La aplicación comprende originalmente la utilización de los algoritmos _Greedy_ y _Hill Climbing_. Al no poder generar un algoritmo _Greedy_ ciento porciento efectivo (ver informe), la aplicación puede generarse en dos versiones, cada una implementando solo uno de los algoritmos mencionados anteriormente.

La versión _Hill Climbing_ es la que se construye por defecto. Con una terminal, dentro del directorio mrp hacer:

> $ make

Para generar la versión _Greedy_, hacer:

> $ make GREEDY=true

Se generan los directorios bin y build, que contienen el binario de la aplicación y los archivos .o respectivamente. Finalmente, se generará un enlace simbólico, llamado mrp, dentro del directorio del mismo nombre, apuntando al binario dentro de bin. Para realizar una limpieza y eliminar los directorios y archivos anteriormente mencionados, ejecutar:

> $ make clean

## Instrucciones de uso
Con una terminal, dentro del directorio donde se genera el enlace simbólico mrp, ejecutar:

### Versión Hill Climbing

> $ ./mrp [parámetro]... [valor]...

#### Parámetros

* -t time_limit: Obligatorio. Especifica la cantidad en segundos time_limit como el tiempo máximo de ejecución de la aplicación.

* -p instance_filename: Obligatorio. Especifica el archivo instance_filename de la instancia a leer por la aplicación.

* -i original_solution_filename: Obligatorio. Especifica el archivo original_solution_filename de la solución inicial a leer por la aplicación.

* -o new_solution_filename: Obligatorio. Especifica el archivo new_solution_filename para escribir la nueva solución generada por la aplicación.

* -s seed: Opcional. Especifica el número seed, para efectuar como semilla en la generación de números aleatorios dentro de la aplicación.

* -m mode:  Opcional. Especifica la forma de selección mode de procesos del  _Hill Climbing_. Si se especifica, con mode igual a "sorted", el _Hill Climbing_ seleccionará los procesos según orden descendente por tamaño. De no especificar, el _Hill Climbing_ seleccionará los procesos de acuerdo a la secuencia de lectura del archivo de la instancia del problema.

#### Ejemplos

> $ ./mrp -t 300 -p model_a1_1.txt -i assignment_a1_1.txt -o output_a1_1.txt -s 1

O con los procesos ordenados por tamaño:

> $ ./mrp -t 300 -p model_a1_1.txt -i assignment_a1_1.txt -o output_a1_1.txt -s 1 -m sorted

#### Salida

A través de la salida estándar, se obtiene el siguiente resultado:

> $ costo_solución_final;tiempo_en_segundos;núm_iteraciones

Ejemplo:
> $ 2128654638;3;8000

### Versión Greedy

Ejecutar:

> $ ./mrp [parámetro]... [valor]...

#### Parámetros

* -t time_limit: Obligatorio. Especifica la cantidad en segundos time_limit como el tiempo máximo de ejecución de la aplicación. Obligatorio.

* -p instance_filename: Obligatorio. Especifica el archivo instance_filename de la instancia a leer y utilizar por la aplicación. Obligatorio.

* -o new_solution_filename: Obligatorio. Especifica el archivo new_solution_filename para escribir la nueva solución inicial, generada por la aplicación.

#### Ejemplo

> $ ./mrp -t 300 -p model_a1_1.txt -o output_a1_1.txt

#### Salida

A través de la salida estándar, se obtiene el siguiente resultado:

> $ costo_solución;tiempo_en_segundos;núm_procesos;núm_procesos_asignados

El número de procesos (total de la instancia) y el de procesos asignados, son mostrados para determinar la efectividad del _Greedy_ en la generación de asignaciones iniciales.

Ejemplo:

> $ 224149350;0;1000;1000

Donde se genera una solución con un costo inicial de 224149350, en ~ 0 segundos de tiempo, para una instancia compuesta de 1000 procesos, todos ellos  asignados. En caso de que no todos lo sean, se obtiene una salida como la siguiente:

> $ -;0;100;98

Donde el costo inicial no se puede determinar (-), en ~ 0 segundos de tiempo, para una instancia compuesta de 1000 procesos, con 98 de ellos asignados.
