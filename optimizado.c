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
    int escala;
    char archivos[25][201]; 
    int num_archivos = 0;
    char* direccion;
    int bool;
    char** matriz;
    crear_carpetas();
    leer_archivos(archivos, &num_archivos);
    printf("Se encontraron %d archivos:\n\n", num_archivos);
    for (int i = 0; i < num_archivos; i++) {
        start=clock();
        escala=tamano_file(archivos[i]);
        direccion=orientacion(archivos[i]);
        if(strstr(direccion, horizontal)!=NULL){
            printf("%s\n", archivos[i]);
            bool=buscar_string_horizontal(archivos[i]);
            printf("%d\n",bool);
            printf("tamaño: %d\n",escala);
            printf("orientacion: %s",direccion);   
        }else if(strstr(direccion, vertical)!=NULL){
            printf("%s\n", archivos[i]);
            matriz=crear_matriz(archivos[i], escala);
            bool=busqueda(matriz, archivos[i], escala);
            printf("%d\n",bool);
            printf("tamaño: %d\n",escala);
            printf("orientacion: %s",direccion);
            liberar_matriz(matriz, escala);
        }
        if(bool){
            moverArchivo(archivos[i], direccion, escala);
        }
        free(direccion);
        end=clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo de ejecución: %.10f segundos\n\n", cpu_time_used);
    }
    return 0;
}