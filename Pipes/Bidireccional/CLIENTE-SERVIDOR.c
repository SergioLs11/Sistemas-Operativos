/***************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: pipes bidireccionales
Se creará un sistema cleinte servidor, en el cual se va a poder realizar una comunicación bidireccional entre el cliente y el servidor. sentido CLIENTE-SERVIDOR
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Se crea el archivo para el named FIFO
#define FIFO_FILE "/tmp/fifo_twoway"
 int main() {
   int fd;
   int finproceso;  
   int tamañostring;  
   int lectura;  
   char contenedor[80]; 
   char fin[5]; 

  // inicio del servicio de parte del cliente
   printf("CLIENTE_FIFO: Sen enviarán mensajes indefinidamente, para finalizar ingrese \"fin\"\n");
   fd = open(FIFO_FILE, O_CREAT|O_RDWR, 0666); 
   strcpy(fin, "fin");

//Se inicia el ciclo de envío del mensaje g¿hacia el servidor
   while (1) {
     printf("Ingrese el mensaje: ");
     fgets(contenedor, sizeof(contenedor), stdin);
     tamañostring = strlen(contenedor);
     contenedor[tamañostring - 1] = '\0';
     finproceso = strcmp(contenedor, fin);
 
      //printf("end_process is %d\n", end_process);
  if (finproceso != 0) {
     write(fd, contenedor, strlen(contenedor));
     printf("FIFOCLIENTE: Mensaje enviado: \"%s\" y su tamaño es %d\n", contenedor, (int)strlen(contenedor));
     lectura = read(fd, contenedor, sizeof(contenedor));
     contenedor[lectura] = '\0';
     printf("FIFOCLIENTE: Mensaje enviado: \"%s\" y su tamaño es %d\n", contenedor, (int)strlen(contenedor));
    } else {
     write(fd, contenedor, strlen(contenedor));
     printf("FIFOCLIENTE: Mensaje enviado: \"%s\" y su tamaño es %d\n", contenedor, (int)strlen(contenedor));
     close(fd);
      break;
   }
   }
  return 0;
  } 
