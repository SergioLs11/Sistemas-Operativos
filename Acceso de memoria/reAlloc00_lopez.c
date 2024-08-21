/********************************************************
*Pontificia universidad Javeriana
*Autor: Sergio Lopez
*Materia: Sistemas Operativos
*Fecha: 23 de agosto del 2024
*Tema: Uso de memoria dinamica con la función reAlloc()
  La funcion reAlloc() se usa para cambiar el tamaño de memoria dinamica asignada.
*********************************************************/

#include <stdio.h>
#include <stdlib.h>


int main() {
int *ptr, i , n1, n2;
printf("Ingrese el tamaño de memoria: ");
scanf("%d", &n1);
// Se crea una reserva de memoria para ptr elementos de tipo int y tamaño n1
ptr = (int*) malloc(n1 * sizeof(int));
printf("Direcciones originalmente asignadas: ");
//Se muestran una por una las direcciones asignadas a ptr
for(i = 0; i < n1; ++i)
printf("\n\np = %p\n", ptr+i);
printf("\n Ingrese un nuevo tamaño: ");
scanf("%d", &n2);
//Se usa la función realloc() para cambiar el tamaño de memoria dinamica asignada al nuevo valor, el cual va a ser n2
ptr = realloc(ptr, n2 * sizeof(int));
printf("Nuevas direcciones de memoria: ");
  //Se muestran una por una las nuevas direcciones asignadas a ptr
for(i = 0; i < n2; ++i)
printf("\n\np = %p\n", ptr+i);

free(ptr);
return 0;
}

//Codigo basado en ejemplos realizados en clase y ejemplos de IBM

