/***************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: named pipes bidireccionales
Se creará un sistema clinte servidor, en el cual se va a poder realizar una comunicación bidireccional entre el cliente y el servidor. sentido SERVIDOR-CLIENTE
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//Se define el archivo tipo named PIPE
#define FIFO_FILE "/tmp/fifo_twoway"
void de_cabeza(char *);
int main() {
   int fd;
   char contenedor[80]; 
   char fin[10]; 
   int finalizar; 
   int lectura; 
 
   //Se crea el archivo FIFO si no se ha creado aún
   mkfifo(FIFO_FILE, S_IFIFO|0660);
   strcpy(fin, "fin");
   fd = open(FIFO_FILE, O_RDWR);

   //Se inicia el ciclo para la recepción del mensaje y reenvío de la palabra invertida
   while(1) {
      lectura = read(fd, contenedor, sizeof(contenedor));
      contenedor[lectura] = '\0';
      printf("FIFOSERVIDOR : Mensaje recibido : \"%s\" y su tamaño es %d\n", contenedor, (int)strlen(contenedor));
      finalizar = strcmp(contenedor, fin);
  
      if (finalizar == 0) {
         close(fd);
         break;
       }
      de_cabeza(contenedor);
      printf("FIFOSERVIDOR: Enviando el mensaje de cabeza: \"%s\" con longitud de %d\n", contenedor, (int) strlen(contenedor));
      write(fd, contenedor, strlen(contenedor));
      /*
      sleep - This is to make sure other process reads this, otherwise this
      process would retrieve the message
      */
      sleep(2);
     }
   return 0;
 }


//Función encargada de invertir la palabra recibida
void de_cabeza(char *str) {
   int ultimo, limite, primero;
   char temp;
   ultimo = strlen(str) - 1;
   limite = ultimo/2;
   primero = 0;
 
   while (primero < ultimo) {
     temp = str[primero];
     str[primero] = str[ultimo];
     str[ultimo] = temp;
     primero++;
     ultimo--;
  }
   return;
 }
