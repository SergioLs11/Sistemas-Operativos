/********************************************************
*Pontificia universidad Javeriana
*Autor: Sergio Lopez
*Materia: Sistemas Operativos
*Fecha: 13 de agosto del 2024
*Tema: Uso de memoria dinamica con la función malloc
  La funcion malloc() se utiliza para reservar un bloque de memoria dinamica de cierto tamaño de bites
  y retorna un puntero a la memoria reservada.
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
// Se declaaran las variables y los punteros requeridos
  
  int *bloque00 =malloc(4*sizeof(int)); // Asignacion de vector de tamaño 4 int 
  int *puntero = malloc(15*sizeof(*puntero)); // Bloque de memoria de 14 int

  // Bucle para llenar lso espacios de memoria
  for(int i=0; i<4; i++){
    bloque00[i]=i*2;
    printf("El valor de Bloque00[%d] es %d\n", i, bloque00[i]);
  }
  if(puntero!=NULL){
    *(puntero+8)= 9884;
  }
  printf("\nEl valor de puntero[8] es %d\n", *(puntero+8));
  
  // Liberacion de memoria
  free(bloque00);
  free(puntero);
  
  
 return 0; 
}