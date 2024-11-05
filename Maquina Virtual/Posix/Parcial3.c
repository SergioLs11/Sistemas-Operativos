


pthread_mutex_t BLOQUEO;

void *funcion(void ¨job){
  pthread_mutex_lock(&BLOQUEO);
size_t IDh = *(size_t_ *)job;
int cont = 5*IDh;
printf("\nHilo %zu Iniciado. \n", IDh);
  
for(int i=0;i<300;i++){
cont *=(cont+1);
}
  
printf("\nHilo %zu finalizado. \n", IDh);
pthread_mutex_unlock(&BLOQUEO);
rerturn NULL;}

int main (){

  size_t jobs[5];
  pthread_t hilos[5];
  int i =0;
  int error;
  if (pthread_mutex_init(&BLOQUEO, NULL) !=0){
    printf("\n Fallo de inicializacion de MUTEX\n");
    return 1;
}
while (i<5){
jobs [i] =i;
error = pthread_create(&hilos[i], NULL, &funcion, jobs+i);
if(error !=0)
  printf("\nError en la creacion de los hilos: [%s]", strerror(error));
i++;
}
pthread_join(hilos[0], NULL);
pthread_join(hilos[1], NULL);
pthread_join(hilos[2], NULL);
pthread_join(hilos[3], NULL);
pthread_join(hilos[4], NULL);
pthread_mutex_destroy(&BLOQUEO);

return 0;}
