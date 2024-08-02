/*--------------------------------------------------------------------
    Autor: Sergio Lopez
    Fecha: 30/07/2024
    Materia: Sitemas Operativos 
    Tema: Argumentos de entrada en C
--------------------------------------------------------------------*/

#include <stdio.h>

// El programa va a pedir 3 argumentos de entrada(Nombre, edad y semestre), ademas de pedir por consola el resto de informacion, asi concretando 8 datos del usuario.


int main(int argc, char *argv[]){
  
  
  
  printf("\n\nNombre: %s\n", argv[1]);
  printf("Ha ingresado la edad: %s \n", argv[2]);
  printf("Ha ingresado el semestre: %s \n", argv[3]);
 
  int a,b,c,e;
  float d;
  printf("Ingrese su año de naciemiento: ");
  scanf("%i", &a);
  printf("Ingrese su mes de naciemiento: ");
  scanf("%i", &b);
  printf("Ingrese su dia de naciemiento: ");
  scanf("%i", &c);
  printf("Ingrese su nota esperada en Operativos: ");
  scanf("%f", &d);
  printf("Ingrese su cedula: ");
  scanf("%i", &e);
  
  printf("El estudiante %s con cedula de ciudadania %i, de %s semestre y con %s años, nacido el %i del %i del %i, espera tener una calificación final en Sistemas Operativos de %f \n",argv[1], e, argv[3], argv[2],c,b,a, d );
  
  
  return 0;
}