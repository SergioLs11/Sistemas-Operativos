
/**************************************************************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 29/10/2024
Materia: Sistemas Operativos
Tema: Uso de hilos en procesos
En el siguiente código se emplearán 5 hilos en una concurrencia pesimista, en el cual, el inicio y la finalización de los hilos se realizará en orden, con una espera de 300 enteros. Como se está usando un control pesimista,
se está asegurando la exclusion mutua en el acceso a los recursos, asi declarando una rutuna de MUTEX(exclusion mutua) con nombre BLOQUEO
Modo de ejecución:
Para la ejecución de este programa, 
Se realizará al descarga del código
Se entrará a la ubicación del archivo desde linea de comandos
se escribirá la siguiente linea "gcc Parcial3_Lopez.c -o LopezP3"
seguido de la linea "./LopezP3"
De esta manera aparecerá el siguiente mensaje en la pantalla:
Hilo 0 Iniciado.

Hilo 0 finalizado.

Hilo 1 Iniciado.

Hilo 1 finalizado.

Hilo 2 Iniciado.

Hilo 2 finalizado.

Hilo 3 Iniciado.

Hilo 3 finalizado.

Hilo 4 Iniciado.

Hilo 4 finalizado.
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Creacion de la rutina de exclusión mutua 
pthread_mutex_t BLOQUEO;

void *funcion(void *job){
  pthread_mutex_lock(&BLOQUEO);
size_t IDh = *(size_t *)job; //inicio de los hilos
int cont = 5*IDh;
printf("\nHilo %zu Iniciado. \n", IDh);
  
for(int i=0;i<300;i++){ // Espera de lso 300 enteros
cont *=(cont+1);
}
  
printf("\nHilo %zu finalizado. \n", IDh);
pthread_mutex_unlock(&BLOQUEO);// fin de los hilos
return NULL;}

// El programa principal va a ser el encargado de definir el número de trabajos y de hilos a usar en este programa, exponer en caso de que falle alguna creación o implementación de los hilos, y finalmente 
int main (){

  size_t jobs[5];
  pthread_t hilos[5];
  int i =0;
  int error;
  if (pthread_mutex_init(&BLOQUEO, NULL) !=0){
    printf("\n Fallo de inicializacion de MUTEX\n");
    return 1;
}
while (i<5){
jobs [i] =i;
error = pthread_create(&hilos[i], NULL, &funcion, jobs+i);
if(error !=0)
  printf("\nError en la creacion de los hilos: [%s]", strerror(error));
i++;
}
pthread_join(hilos[0], NULL);
pthread_join(hilos[1], NULL);
pthread_join(hilos[2], NULL);
pthread_join(hilos[3], NULL);
pthread_join(hilos[4], NULL);
pthread_mutex_destroy(&BLOQUEO); // Eliminación de la rutian de excluxión mutua

return 0;}
