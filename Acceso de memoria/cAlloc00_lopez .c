/********************************************************
*Pontificia universidad Javeriana
*Autor: Sergio Lopez
*Materia: Sistemas Operativos
*Fecha: 13 de agosto del 2024
*Tema: Uso de memoria dinamica con la función calloc
  La funcion calloc() se utiliza para reservar un bloque de memoria dinamica para un arreglo de cierto tamaño de bites
  y retorna un puntero a la memoria reservada.
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
// Se declaaran las variables y los punteros requeridos
  
  int *p1 =calloc(4, sizeof(int)); // Asignacion de vector de tamaño 4 int 
  int *p2 =calloc(11, sizeof(int[4])); // Bloque de memoria de 1 int
  int *p3 =calloc(5, sizeof(*p3)); // Bloque de memoria de 5 int
  // Bucle para llenar lso espacios de memoria
  for(int i=0; i<10; i++){
    p2[i]=i*2;
    printf("El valor de p2[%d] es %d\n", i, p2[i]);
  }
  if(p2){
    *(p1+8)= 9884;
  }
  
  // Liberacion de memoria
  free(p1);
  free(p2);
  free(p3);
  
  
 return 0; 
}