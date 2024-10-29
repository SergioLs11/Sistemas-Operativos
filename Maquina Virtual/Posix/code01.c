/**************************************************************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 29/10/2024
Materia: Sistemas Operativos
Tema: introducción a procesos con hilos
La creación de hilos se da por la necesidad de dividir el proceso lo mayor posible, de esta manera, los hilos son las unidades de procesamiento mas pequeña. En el siguiente código se ralizará la separación mediante el comando "pthreads_t", creando el número de trabajos e hilos que ingrese el usuario.
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

/*Función cualquiera para ejecutar el thread*/
 static void *funcion(void *argumentos){
     size_t job = *(size_t *)argumentos;
     printf("\n\t Job %zu \n", job);
     return NULL;
 }

 int main(int argc, char *argv[]){
     int N = (int)atoi(argv[1]);
     size_t jobs[N];                 //Vector de trabajos|
     pthread_t hilos[N];             //Vector de hilos
     for(int i=0; i<N; i++){
         jobs[i] = i;
         //Creación de hilos y envío de función
         pthread_create(&hilos[i], NULL, funcion, jobs+i);
     }
     for(int i=0; i<N; i++)
         pthread_join(hilos[i], NULL);

     return 0;
 }
