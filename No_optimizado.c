#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "Funciones.h"

int main() {
    clock_t start, end;
    double cpu_time_used;
    char horizontal[]="horizontal";
    char vertical[]="vertical";
    char* direccion;
    char archivos[25][201];
    char** matriz;
    int escala, bool;
    int num_archivos, i= 0;
    crear_carpetas();

    leer_archivos(archivos, &num_archivos);
    printf("Se encontraron %d archivos:\n\n", num_archivos);
    for (i = 0; i < num_archivos; i++) {
        start=clock();
        printf("%s\n", archivos[i]);
        escala=tamano_file(archivos[i]);
        direccion=orientacion(archivos[i]);
        matriz=crear_matriz(archivos[i], escala);
        if(strstr(direccion, horizontal)!=NULL){
            bool=buscar_palabra_horizontal(matriz, archivos[i], escala);
            printf("%d\n",bool);
            printf("tamaño: %d\n",escala);
            printf("orientacion: %s",direccion);
            
        }else if(strstr(direccion, vertical)!=NULL){
            char** matrizVolteada=intercambiarElementos(matriz, escala);
            bool=buscar_palabra_horizontal(matrizVolteada,archivos[i], escala);
            printf("%d\n",bool);
            printf("tamaño: %d\n",escala);
            printf("orientacion: %s",direccion);
            liberar_matriz(matrizVolteada, escala);
        }
        if(bool){
            moverArchivo(archivos[i], direccion, escala);
        }
        liberar_matriz(matriz, escala);
        free(direccion);
        end=clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo de ejecución: %.10f segundos\n\n", cpu_time_used);
    }
    return 0;
}