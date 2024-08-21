/********************************************************
*Pontificia universidad Javeriana
*Autor: Sergio Lopez
*Materia: Sistemas Operativos
*Fecha: 20 de agosto del 2024
*Tema: Actividad primer parcial
  EL código presentado tiene varias funciones las cuales hacen uso de la libreria stdlib.h para la creacion de memoria dinamica y la liberacion de esta 
*********************************************************/

#define CAPACIDAD_INICIAL 4
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


/*Creación Estructura de Datos para el Vector*/
typedef struct vectorDinamico{
        int capacidad;
        int totalElementos;
        void **elementos;
} vectorDinamico;


//Función para agregar valores al vector dinamico
void vectorInicio(vectorDinamico *V){
        V->capacidad = CAPACIDAD_INICIAL;
        V->totalElementos = 0;
        V->elementos = malloc(sizeof(void *) * V->capacidad); 
}


//Función encargada de retornar la cantidad de elementos del vector dinamico
int totalVector(vectorDinamico *V){
        return V->totalElementos;
}


//Funcion encargada de cambiar el tamaño del vector dinamico
static void resizeVector(vectorDinamico *V, int capacidad){
        printf("Redimensión: %d a %d \n", V->capacidad, capacidad);

        void **elementos = realloc(V->elementos, sizeof(void *) * capacidad);
        if(elementos){
                V->elementos = elementos;
                V->capacidad = capacidad;
        }
}


//Función encargada de agregar un valor al vector dinamico, si este no tiene la capacidad, va a llamar
// la función resizeVector para aumentar la capacidad del vector dinamico
void addVector(vectorDinamico *V, void *elemento){
        if(V->capacidad == V->totalElementos)
                resizeVector(V, V->capacidad*2);
        V->elementos[V->totalElementos++] = elemento;
}


//Funcion encaragda de liberar la memoria dinamica usada
void freeVector(vectorDinamico *V){
        free(V->elementos);
}


//Funcion encargada de retornar un valor del vector dinamico en la posición del indice dado
void *getVector(vectorDinamico *V, int indice){
        if(indice >= 0 && indice < V->totalElementos)
                return V->elementos[indice];
        return NULL;
}

//Funcion encargada de asignar un valor al vector dinamico en la posición del indice dado
void setVector(vectorDinamico *V, int indice, void *elemento){
        if(indice >= 0 && indice < V->totalElementos)
                V->elementos[indice]=elemento;
}

// Funcion encargada de disminuir el vector dinamico por posiciones, dejando nulo en la posicion del indice dado 
void borrarVector(vectorDinamico *V, int indice){
        if(indice < 0 || indice >= V->totalElementos)
                return; 

        V->elementos[indice] = NULL;

        for(int i=indice; i<V->totalElementos-1; i++){
                V->elementos[i] = V->elementos[i+1];
                V->elementos[i+1] = NULL; 
        }
        V->totalElementos--;
        if(V->totalElementos>0 && V->totalElementos == V->capacidad/4)
                resizeVector(V, V->capacidad/2);
}

int main(){
        vectorDinamico editor;
        vectorInicio(&editor);

        addVector(&editor, "\nHola ");
        addVector(&editor, "Profesional ");
        addVector(&editor, "en " );
        addVector(&editor, "Formación ");
        addVector(&editor, "de");
        addVector(&editor, " ");
        addVector(&editor, "Ingenieria\n\n");

        int k = 0;
        for (int i = 0; i < totalVector(&editor); i++){
        printf("%s", (char *) getVector(&editor, k));
        k++;
        }

        for(int i=1; i<totalVector(&editor); i++)
        borrarVector(&editor, i);
        

        setVector(&editor, 1, "Buenos ");
        setVector(&editor, 2, "Dias \n");
        
        k=0;
        for (int i = 1; i < totalVector(&editor); i++){
        printf("%s ", (char *) getVector(&editor, k));
        k++;}


        return 0;
}
