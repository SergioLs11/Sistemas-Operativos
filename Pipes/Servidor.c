/***************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: Servidor usando pipes
Se creará un proceso que haga las veces de servidor, para de esta manera realizar una comunicación entre este y un cliente usando pies y named pipes.
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
  char BusMensaje[80];
  char final[10];
  int fin;
  int cantidadBytes;
  printf("<<<<<<<<<<Inicio servidor>>>>>>>>>>>\n");

  /* Create the FIFO if it does not exist */
  mknod(FIFO_FILE, S_IFIFO|0640, 0);
  strcpy(final, "fin");
  while(1) {
     ArchivoDesc = open(FIFO_FILE, O_RDONLY);
    cantidadBytes = read(ArchivoDesc, BusMensaje, sizeof(BusMensaje));
    BusMensaje[cantidadBytes] = '\0';
    printf("Mensaje recibido: \"%s\" y de tamaño %d\n", BusMensaje, (int)strlen (BusMensaje));
        fin = strcmp(BusMensaje, final);
      if (fin == 0) {
           close(ArchivoDesc);
          break;
        }
    }
  return 0;}
