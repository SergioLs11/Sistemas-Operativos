/***************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: cliente usando pipes
Se creará un proceso que haga las veces de ciente, para de esta manera realizar una comunicación entre este y un servidor usando pies y named pipes.
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "MYFIFO"

 int main() {
     int ArchivoDesc;
     int fin_proceso;
     int tamano;
     char buffer[80];
     char final_str[5];
   
     printf("<<<<<<<<<<Inicio cliente>>>>>>>>>>>\n");
     printf("FIFO_CLIENTE: Enviando mensaje, infinitamente, para finalizar ingrese \"fin\"\n");
     ArchivoDesc = open(FIFO_FILE, O_CREAT|O_WRONLY, 0640);
     strcpy(final_str, "fin");
  
     while (1) {
        printf("Ingrese el mensaje: ");
        fgets(buffer, sizeof(buffer), stdin);
        tamano = strlen(buffer);
        buffer[tamano - 1] = '\0';
        fin_proceso = strcmp(buffer, final_str);
  
        //printf("end_process is %d\n", end_process);
        if (fin_proceso != 0) {
           write(ArchivoDesc, buffer, strlen(buffer));
          printf("Mensaje enviado: \"%s\" y el tamaño es %d\n", buffer, (int)    strlen(buffer));
        } else {
           write(ArchivoDesc, buffer, strlen(buffer));
           printf("Mensaje enviado: \"%s\" y el tamaño es %d\n", buffer, (int)    strlen(buffer));
           close(ArchivoDesc);
           break;
        }
     }
     return 0;}
