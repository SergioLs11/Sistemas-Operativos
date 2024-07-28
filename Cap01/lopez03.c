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
  printf(" \t Bienvenido a mi primer clase de SO \n \n ");  
}

void suma(int a, int b){
  printf("El primer entero es %d \n", a);
  printf("El segundo entero es %d \n", b);
  int suma = a+b;
  printf("el resultado de la suma es: %d \n", suma);
}

void area(float a){
  float radio=a;
  float pi = 3.14;
  printf("el radio es : %f \n", radio);
  float area= radio*radio*pi;
  printf("el area es : %f \n", area);
}

int main(int argc, char *argv[]){
  bienvenida();
  int a, b;
  printf("Ingrese el primer numero a sumar: ");
  scanf("%i", &a);
  printf("Ingrese el segundo numero a sumar: ");
  scanf("%i", &b);
  
  suma(a,b);
  float radio;
   printf("Ingrese el radio del circulo: ");
  scanf("%f", &radio);
  area(radio);
  
  return 0;
}
