/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J. Corredor
	Fecha: Febrero 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3) // Define el tamaño de memoria usado para las matrices

pthread_mutex_t MM_mutex; // Declaración de un mutex para sincronización
static double MEM_CHUNK[DATA_SIZE]; // Array de memoria donde se almacenarán las matrices
double *mA, *mB, *mC; // Punteros para las matrices A, B y C

// Estructura para manejar los parámetros enviados a cada hilo
struct parametros {
    int nH; // Total de hilos que se utilizarán
    int idH; // Identificador único para cada hilo
    int N; // Tamaño de las matrices
};

struct timeval start, stop; // Estructuras para calcular el tiempo transcurrido

// Función para llenar las matrices con valores basados en el índice
void llenar_matriz(int SZ) { 
    srand48(time(NULL)); // Inicializa la semilla para la función drand48
    for(int i = 0; i < SZ*SZ; i++) {
        mA[i] = 1.1*i; // Llenado de la matriz A con valores
        mB[i] = 2.2*i; // Llenado de la matriz B con valores
        mC[i] = 0; // Inicialización de la matriz C con ceros
    }   
}

// Función para imprimir matrices en formato amigable si el tamaño es pequeño
void print_matrix(int sz, double *matriz) {
    if(sz < 12) {
        for(int i = 0; i < sz*sz; i++) {
            if(i % sz == 0) printf("\n"); // Inserta un salto de línea al final de cada fila
            printf(" %.3f ", matriz[i]); // Imprime cada valor con 3 decimales
        }   
    }
    printf("\n>-------------------->\n");
}

// Inicializa el temporizador al comienzo de la ejecución
void inicial_tiempo() {
    gettimeofday(&start, NULL); // Captura el tiempo inicial
}

// Calcula y muestra el tiempo transcurrido desde la llamada a inicial_tiempo
void final_tiempo() {
    gettimeofday(&stop, NULL); // Captura el tiempo final
    stop.tv_sec -= start.tv_sec; // Calcula la diferencia en segundos
    printf("\n %9.0f \n", (double)(stop.tv_sec * 1000000 + stop.tv_usec)); // Muestra el tiempo en microsegundos
}

// Función ejecutada por cada hilo para realizar la multiplicación de matrices
void *mult_thread(void *variables) {
    struct parametros *data = (struct parametros *)variables; // Casteo de los datos pasados al hilo

    int idH = data->idH; // ID del hilo
    int nH  = data->nH; // Número total de hilos
    int N   = data->N; // Tamaño de las matrices
    int ini = (N / nH) * idH; // Índice inicial que corresponde al hilo
    int fin = (N / nH) * (idH + 1); // Índice final que corresponde al hilo

    // Bucle para recorrer la matriz y hacer la multiplicación de matrices
    for (int i = ini; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            double *pA, *pB, sumaTemp = 0.0;
            pA = mA + (i * N); // Puntero a la fila de la matriz A
            pB = mB + j; // Puntero a la columna de la matriz B
            for (int k = 0; k < N; k++, pA++, pB += N) {
                sumaTemp += (*pA * *pB); // Realiza el producto y acumula el resultado
            }
            mC[i * N + j] = sumaTemp; // Guarda el resultado en la matriz C
        }
    }

    pthread_mutex_lock(&MM_mutex); // Bloquea el mutex (aunque no se utiliza de manera crítica aquí)
    pthread_mutex_unlock(&MM_mutex); // Desbloquea el mutex
    pthread_exit(NULL); // Finaliza la ejecución del hilo
}

// Función principal que maneja el flujo del programa
int main(int argc, char *argv[]) {
    if (argc < 2) { // Verifica si se proporcionaron los parámetros necesarios
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;   
    }

    int SZ = atoi(argv[1]); // Convierte el tamaño de la matriz desde los argumentos
    int n_threads = atoi(argv[2]); // Convierte el número de hilos desde los argumentos

    pthread_t p[n_threads]; // Array para almacenar los identificadores de los hilos
    pthread_attr_t atrMM; // Atributos de los hilos

    mA = MEM_CHUNK; // Asigna la memoria para mA
    mB = mA + SZ * SZ; // Asigna la memoria para mB
    mC = mB + SZ * SZ; // Asigna la memoria para mC

    llenar_matriz(SZ); // Llenado de las matrices A, B y C
    print_matrix(SZ, mA); // Imprime la matriz A
    print_matrix(SZ, mB); // Imprime la matriz B

    inicial_tiempo(); // Comienza a medir el tiempo de ejecución
    pthread_mutex_init(&MM_mutex, NULL); // Inicializa el mutex
    pthread_attr_init(&atrMM); // Inicializa los atributos del hilo
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Define el estado de los hilos como unidos

    // Crea los hilos para realizar la multiplicación de matrices
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *)malloc(sizeof(struct parametros)); // Asigna memoria para los datos del hilo
        datos->idH = j; // Asigna el ID del hilo
        datos->nH = n_threads; // Asigna el número total de hilos
        datos->N = SZ; // Asigna el tamaño de las matrices
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea el hilo
    }

    // Espera a que todos los hilos terminen su ejecución
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL); // Une cada hilo
    }

    final_tiempo(); // Mide el tiempo total de ejecución

    print_matrix(SZ, mC); // Imprime el resultado de la multiplicación de matrices

    pthread_attr_destroy(&atrMM); // Destruye los atributos de los hilos
    pthread_mutex_destroy(&MM_mutex); // Destruye el mutex
    pthread_exit(NULL); // Termina el proceso principal
}
