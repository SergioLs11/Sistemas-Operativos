/**************************************************************************************
Pontificia Universidad Javeriana
Autor: Sergio Andrés López Sánchez
Fecha: 11/11/2024
Materia: Sistemas Operativos
Tema: Proyecto final del curso
Este proyecto se basa en un sistema cliente servidor, en el cual se van a tener publicadores y suscriptores, los publicadores son los encargados de publicar noticias de distintos intereses, mientras que los suscriptores van a proporcionar los temas que están interesados en recibir, la cominucación se va a realizar mediante un sistema de comunicación, el cual va a ser el encargado de filtrar las noticias que deban ir a cada suscriptor.
**************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SUSCRIPTORES 10 // Máximo número de suscriptores
#define MAX_TEMAS 5         // Máximo número de temas por suscriptor

// Función para el proceso Publicador
void publicador(const char* pipePSC, int timeN) {
    int fd = open(pipePSC, O_WRONLY);   // Abrir el pipe en modo escritura
    if (fd == -1) {
        perror("Error al abrir el pipe para escritura");
        return;
    }

    char noticia[256];
    printf("Ingrese una noticia en formato [Tipo: Noticia.], o 'salir' para terminar:\n");
    while (1) {
        printf("> ");
        fgets(noticia, sizeof(noticia), stdin); // Leer la noticia
        noticia[strcspn(noticia, "\n")] = 0;  // Eliminar el salto de línea

        // Permitir al usuario salir
        if (strcmp(noticia, "salir") == 0) break;

        // Validar el formato de la noticia
        if (strlen(noticia) < 4 || noticia[1] != ':' || noticia[strlen(noticia) - 1] != '.') {
            fprintf(stderr, "Formato incorrecto. Asegúrese de usar [Tipo: Noticia.]\n");
            continue;
        }

        // Enviar noticia a través del pipe
        write(fd, noticia, strlen(noticia) + 1);
        printf("Noticia enviada: %s\n", noticia);

        // Esperar timeN segundos antes de la próxima noticia
        sleep(timeN);
    }

    close(fd);
}

// Función para el proceso Suscriptor
void suscriptor(char *pipeSSC) {
    int fd;
    char buffer[100];
    char suscripciones[10];

    // Abrir el pipe de comunicación con el sistema de comunicación en modo no bloqueante
    fd = open(pipeSSC, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error abriendo el pipeSSC para el suscriptor");
        exit(EXIT_FAILURE);
    }

    // Solicitar tópicos de suscripción
    printf("Ingrese los tópicos a suscribirse (Ejemplo: P A C): ");
    fgets(suscripciones, sizeof(suscripciones), stdin);

    // Enviar los tópicos al sistema de comunicación
    int fd_sus = open("pipePSC", O_WRONLY);
    if (fd_sus == -1) {
        perror("Error abriendo el pipePSC para enviar suscripciones");
        close(fd);
        exit(EXIT_FAILURE);
    }
    write(fd_sus, suscripciones, strlen(suscripciones));
    close(fd_sus);

    printf("Esperando noticias...\n");

    // Leer noticias en bucle con espera activa ajustada
    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Noticia recibida: %s\n", buffer);
        } else if (bytes_read == -1) {
            usleep(100000);  // Espera de 0.1 segundos antes de reintentar
        }
    }
    close(fd);
}

// Función para el sistema de comunicación
void sistema_comunicacion(const char* pipePSC, const char* pipeSSC, int timeF) {
    mkfifo(pipePSC, 0666);
    mkfifo(pipeSSC, 0666);

    // Abrir los pipes en modo lectura y escritura 
    int fdPSC = open(pipePSC, O_RDONLY);
    int fdSSC = open(pipeSSC, O_WRONLY);

    if (fdPSC == -1 || fdSSC == -1) {
        perror("Error al abrir los pipes");
        return;
    }

    printf("Sistema de comunicación iniciado.\n");

    // Array para almacenar las suscripciones de los suscriptores
    char suscripciones[MAX_SUSCRIPTORES][MAX_TEMAS];
    int num_suscriptores = 0;

    char buffer[256];
    while (1) {
        // Leer del pipePSC (puede ser una noticia o una suscripción)
        if (read(fdPSC, buffer, sizeof(buffer)) > 0) {
            buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea al final, si existe

            // Verificar si el mensaje es una suscripción o una noticia
            if (buffer[1] == ' ') { // Si el segundo carácter es espacio, interpretarlo como suscripción
                strncpy(suscripciones[num_suscriptores], buffer, MAX_TEMAS - 1);
                suscripciones[num_suscriptores][MAX_TEMAS - 1] = '\0'; // Asegurar terminación de cadena
                printf("Suscripciones del suscriptor %d registradas: %s\n", num_suscriptores + 1, buffer);
                num_suscriptores++;
            } else { // Interpretar como noticia
                char tema = buffer[0];  // Primer carácter del mensaje es el tema
                printf("Noticia recibida: %s\n", buffer);

                // Enviar la noticia solo a los suscriptores interesados en ese tema
                for (int i = 0; i < num_suscriptores; i++) {
                    if (strchr(suscripciones[i], tema)) { // Verificar si el suscriptor está suscrito al tema
                        write(fdSSC, buffer, strlen(buffer) + 1);
                        printf("Noticia enviada a suscriptor %d: %s\n", i + 1, buffer);
                    }
                }
            }
        }

        // Esperar el tiempo antes de finalizar si no recibe mensajes nuevos
        sleep(timeF);
    }

    close(fdPSC);
    close(fdSSC);
    unlink(pipePSC);
    unlink(pipeSSC);
}


// Función principal para manejar el modo de ejecución
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s [publicador|suscriptor|sistema] [opciones]\n", argv[0]);
        return 1;
    }

    char* modo = argv[1];
    if (strcmp(modo, "publicador") == 0) {
        char* pipePSC = NULL;
        int timeN = 1;

        // Parsear argumentos
        for (int i = 2; i < argc; i += 2) {
            if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];
            else if (strcmp(argv[i], "-t") == 0) timeN = atoi(argv[i + 1]);
        }

        if (pipePSC != NULL) {
            publicador(pipePSC, timeN);
        } else {
            fprintf(stderr, "Error: Falta el argumento -p para el pipePSC\n");
        }
    } else if (strcmp(modo, "suscriptor") == 0) {
        char* pipeSSC = NULL;

        // Parsear argumentos
        for (int i = 2; i < argc; i += 2) {
            if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];
        }

        if (pipeSSC != NULL) {
            suscriptor(pipeSSC);
        } else {
            fprintf(stderr, "Error: Falta el argumento -s para el pipeSSC\n");
        }
    } else if (strcmp(modo, "sistema") == 0) {
        char* pipePSC = NULL;
        char* pipeSSC = NULL;
        int timeF = 1;

        // Parsear argumentos
        for (int i = 2; i < argc; i += 2) {
            if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];
            else if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];
            else if (strcmp(argv[i], "-t") == 0) timeF = atoi(argv[i + 1]);
        }

        if (pipePSC != NULL && pipeSSC != NULL) {
            sistema_comunicacion(pipePSC, pipeSSC, timeF);
        } else {
            fprintf(stderr, "Error: Faltan argumentos -p y/o -s\n");
        }
    } else {
        fprintf(stderr, "Modo no reconocido. Use publicador, suscriptor o sistema.\n");
        return 1;
    }

    return 0;
}
