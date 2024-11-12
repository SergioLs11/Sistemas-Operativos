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

#define DATA_SIZE (1024*1024*64*3) // Definición del tamaño de memoria a utilizar

pthread_mutex_t MM_mutex; // Mutex global para sincronización
static double MEM_CHUNK[DATA_SIZE]; // Bloque de memoria para las matrices
double *mA, *mB, *mC; // Punteros a las matrices A, B y C

// Estructura para pasar parámetros a los hilos
struct parametros {
    int nH; // Número de hilos
    int idH; // ID del hilo
    int N; // Tamaño de la matriz
};

struct timeval start, stop; // Estructuras para medir el tiempo

// Función para llenar las matrices con valores
void llenar_matriz(int SZ) {
    srand48(time(NULL)); // Inicializar la semilla para la aleatoriedad
    for(int i = 0; i < SZ*SZ; i++) {
        mA[i] = 1.1*i; // Inicialización de mA
        mB[i] = 2.2*i; // Inicialización de mB
        mC[i] = 0; // Inicialización de mC
    }   
}

// Función para imprimir la matriz si su tamaño es pequeño
void print_matrix(int sz, double *matriz) {
    if(sz < 12) {
        for(int i = 0; i < sz*sz; i++) {
            if(i % sz == 0) printf("\n"); // Salto de línea para cada fila
            printf(" %.3f ", matriz[i]); // Imprime el valor con 3 decimales
        }   
    }
    printf("\n>-------------------->\n");
}

// Inicialización del tiempo para medir la ejecución
void inicial_tiempo() {
    gettimeofday(&start, NULL); // Toma el tiempo actual
}

// Función para finalizar el tiempo y mostrar el tiempo transcurrido
void final_tiempo() {
    gettimeofday(&stop, NULL); // Toma el tiempo final
    stop.tv_sec -= start.tv_sec; // Calcula la diferencia en segundos
    printf("\n:-> %9.0f µs\n", (double)(stop.tv_sec * 1000000 + stop.tv_usec)); // Muestra en microsegundos
}

// Función que ejecutará cada hilo para realizar la multiplicación de matrices
void *mult_thread(void *variables) {
    struct parametros *data = (struct parametros *)variables; // Casteo de la estructura pasada

    int idH = data->idH; // ID del hilo
    int nH  = data->nH; // Número total de hilos
    int N   = data->N; // Tamaño de la matriz
    int ini = (N / nH) * idH; // Índice inicial para el hilo
    int fin = (N / nH) * (idH + 1); // Índice final para el hilo

    // Bucle que realiza la multiplicación de matrices
    for (int i = ini; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            double *pA, *pB, sumaTemp = 0.0;
            pA = mA + (i * N); // Puntero a la fila de mA
            pB = mB + j; // Puntero a la columna de mB
            for (int k = 0; k < N; k++, pA++, pB += N) {
                sumaTemp += (*pA * *pB); // Producto y acumulación
            }
            mC[i * N + j] = sumaTemp; // Asignación del valor calculado a mC
        }
    }

    pthread_mutex_lock(&MM_mutex); // Bloqueo del mutex (aunque no hay operación crítica)
    pthread_mutex_unlock(&MM_mutex); // Desbloqueo del mutex
    pthread_exit(NULL); // El hilo finaliza su ejecución
}

// Función principal que ejecuta el programa
int main(int argc, char *argv[]) {
    if (argc < 2) { // Verifica si hay suficientes argumentos
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;   
    }

    int SZ = atoi(argv[1]); // Tamaño de la matriz
    int n_threads = atoi(argv[2]); // Número de hilos

    pthread_t p[n_threads]; // Array para almacenar los identificadores de los hilos
    pthread_attr_t atrMM; // Atributos de los hilos

    mA = MEM_CHUNK; // Asignación de puntero a mA
    mB = mA + SZ * SZ; // Asignación de puntero a mB
    mC = mB + SZ * SZ; // Asignación de puntero a mC

    llenar_matriz(SZ); // Llenado de las matrices
    print_matrix(SZ, mA); // Impresión de mA
    print_matrix(SZ, mB); // Impresión de mB

    inicial_tiempo(); // Inicialización del tiempo de ejecución
    pthread_mutex_init(&MM_mutex, NULL); // Inicialización del mutex
    pthread_attr_init(&atrMM); // Inicialización de los atributos del hilo
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Definición de los atributos para hilos unidos

    // Creación de los hilos
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *)malloc(sizeof(struct parametros)); // Asignación de memoria para parámetros
        datos->idH = j; // Asignación del ID del hilo
        datos->nH = n_threads; // Asignación del número total de hilos
        datos->N = SZ; // Asignación del tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Creación del hilo
    }

    // Espera a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL); // Unir los hilos
    }

    final_tiempo(); // Medir el tiempo final

    print_matrix(SZ, mC); // Imprimir el resultado de la multiplicación

    pthread_attr_destroy(&atrMM); // Liberar los atributos del hilo
    pthread_mutex_destroy(&MM_mutex); // Destruir el mutex
    pthread_exit(NULL); // Finalizar el proceso principal
}
