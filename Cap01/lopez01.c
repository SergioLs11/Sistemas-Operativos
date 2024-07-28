/***************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 23/07/2024
Materia: Sistemas Operativos
Tema: Programación en C
Requerimientos:
    -Función que sume dos enteros
    -Función que calcule el area de una circunferencia
    -Función que de la bienvenida
***************************************/
#include <stdio.h>

void bienvenida(){
  printf("Hola mundo...\n");
  printf(" \t Bienvenido a mi primer clase de SO \n ");  
}

void suma(){
  printf("El primer entero es 5 \n");
  printf("El segundo entero es 16 \n");
  int suma = 5+16;
  printf("el resultado es : %d \n", suma);
}

void area(){
  int radio = 5;
  float pi = 3.14;
  printf("el radio es : %d \n", radio);
  float area= radio*radio*pi;
  printf("el area es : %f \n", area);
}

int main(int argc, char *argv[]){
  bienvenida();
  suma();
  area();
  
  return 0;
}
