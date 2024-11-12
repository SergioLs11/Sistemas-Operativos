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

// Define el tamaño de la memoria utilizada para las matrices
#define DATA_SIZE (1024*1024*64*3) 

pthread_mutex_t MM_mutex; // Mutex para control de acceso a la memoria compartida
static double MEM_CHUNK[DATA_SIZE]; // Array para almacenar las matrices A, B y C
double *mA, *mB, *mC; // Punteros a las matrices A, B y C

// Estructura que contiene los parámetros necesarios para cada hilo
struct parametros {
    int nH; // Número total de hilos
    int idH; // Identificador del hilo
    int N; // Tamaño de las matrices (NxN)
};

struct timeval start, stop; // Estructuras para almacenar el tiempo de inicio y final

// Función para llenar las matrices con valores según el índice
void llenar_matriz(int SZ) { 
    srand48(time(NULL)); // Inicializa la semilla para generar números aleatorios
    for (int i = 0; i < SZ * SZ; i++) {
        mA[i] = 1.1 * i; // Llenado de la matriz A con valores incrementales
        mB[i] = 2.2 * i; // Llenado de la matriz B con valores incrementales
        mC[i] = 0; // Inicializa la matriz C con ceros
    }   
}

// Función para imprimir matrices (útil cuando el tamaño es pequeño)
void print_matrix(int sz, double *matriz) {
    if (sz < 12) { // Si el tamaño es menor a 12, imprime la matriz completa
        for (int i = 0; i < sz * sz; i++) {
            if (i % sz == 0) printf("\n"); // Salto de línea por cada fila
            printf(" %.3f ", matriz[i]); // Imprime el valor de cada elemento
        }   
    }
    printf("\n>-------------------->\n");
}

// Inicializa el temporizador para medir el tiempo de ejecución
void inicial_tiempo() {
    gettimeofday(&start, NULL); // Obtiene el tiempo actual
}

// Calcula y muestra el tiempo transcurrido desde inicial_tiempo
void final_tiempo() {
    gettimeofday(&stop, NULL); // Obtiene el tiempo final
    stop.tv_sec -= start.tv_sec; // Resta los segundos
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec * 1000000 + stop.tv_usec)); // Muestra el tiempo en microsegundos
}

// Función que se ejecuta en cada hilo para realizar la multiplicación de matrices
void *mult_thread(void *variables) {
    struct parametros *data = (struct parametros *)variables; // Casteo de los datos pasados al hilo

    int idH = data->idH; // ID del hilo
    int nH  = data->nH; // Número total de hilos
    int N   = data->N; // Tamaño de las matrices
    int ini = (N / nH) * idH; // Índice inicial asignado a cada hilo
    int fin = (N / nH) * (idH + 1); // Índice final asignado al hilo

    // Bucle para multiplicar las matrices A y B y almacenar el resultado en C
    for (int i = ini; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            double *pA, *pB, sumaTemp = 0.0;
            pA = mA + (i * N); // Puntero al inicio de la fila i de la matriz A
            pB = mB + (j * N); // Puntero al inicio de la columna j de la matriz B
            // Realiza el producto punto entre fila de A y columna de B
            for (int k = 0; k < N; k++, pA++, pB++) {
                sumaTemp += (*pA * *pB); // Suma el producto de los elementos correspondientes
            }
            mC[i * N + j] = sumaTemp; // Guarda el resultado en la matriz C
        }
    }

    // Bloqueo y desbloqueo del mutex (aunque no se está utilizando para sincronización real en este código)
    pthread_mutex_lock(&MM_mutex); 
    pthread_mutex_unlock(&MM_mutex); 
    pthread_exit(NULL); // Termina la ejecución del hilo
}

// Función principal donde se inicializan y gestionan los hilos
int main(int argc, char *argv[]) {
    // Verifica si se proporcionaron los argumentos necesarios
    if (argc < 2) {
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;   
    }

    // Convierte los parámetros de la línea de comandos
    int SZ = atoi(argv[1]); // Tamaño de la matriz (N)
    int n_threads = atoi(argv[2]); // Número de hilos a utilizar

    // Array para almacenar los identificadores de los hilos
    pthread_t p[n_threads];
    pthread_attr_t atrMM; // Atributos para los hilos

    // Asigna memoria para las matrices A, B y C en el arreglo MEM_CHUNK
    mA = MEM_CHUNK;
    mB = mA + SZ * SZ;
    mC = mB + SZ * SZ;

    // Llama a la función para llenar las matrices con datos
    llenar_matriz(SZ);
    
    // Imprime las matrices A y B si el tamaño es pequeño
    print_matrix(SZ, mA);
    print_matrix(SZ, mB);

    // Inicia el conteo de tiempo
    inicial_tiempo();

    // Inicializa el mutex y los atributos del hilo
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Establece el estado de los hilos como "joinable"

    // Crea los hilos para realizar la multiplicación de matrices
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *)malloc(sizeof(struct parametros)); // Asigna memoria para los datos del hilo
        datos->idH = j; // Asigna el ID del hilo
        datos->nH = n_threads; // Asigna el número total de hilos
        datos->N = SZ; // Asigna el tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea el hilo
    }

    // Espera a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL); // Une cada hilo, esperando a que termine
    }

    // Calcula y muestra el tiempo total de ejecución
    final_tiempo();

    // Imprime la matriz resultante (C) si el tamaño es pequeño
    print_matrix(SZ, mC);

    // Libera los recursos utilizados por los atributos de los hilos y el mutex
    pthread_attr_destroy(&atrMM); 
    pthread_mutex_destroy(&MM_mutex); 
    pthread_exit(NULL); // Termina el hilo principal
}
