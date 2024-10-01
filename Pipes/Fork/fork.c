#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
 
 //Función encargada de crear los procesos padre e hijo 
void creacion_fork(int linea){
   pid_t proceso; 
   proceso = fork();
   if(proceso<0){
     printf("Error creación proceso\n");
     exit(1);
   }else if (proceso==0)
   printf("%d: Proceso =HIJO=: %d\n",linea, getpid());
   else
     printf("%d: Proceso =PADRE= %d\n",linea, getpid());
 }

//Función principal donde se va a realizar el ciclo de cuantas veces se va a entrar a crear procesos
 int main(int argc, char *argv[]){
   int p = (int) atoi(argv[1]);
   for(int i=0; i<p; i++)
     creacion_fork(i);
  
  
    printf("\n---\n\n");
    return 0;
 }
