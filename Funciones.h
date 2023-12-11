#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


//Descripcion de la funcion:      Se encarga de mover el archivo a un directorio seleccionado dentro de otro sub-directorio
//    Parametros:
//        nom_archivo(char*):     Nombre del archivo a mover en formato con terminacion .txt.
//        horiantacion(char*):    Nombre del primer directorio donde debe moverse el archivo.
//        tamano_i(int):          Tamaño de la matriz que genera la sopa de letras
//    Retorno:
//        no tiene retorno

void moverArchivo(char* nom_archivo, char* horiantacion, int tamano_i) {
    char src_path[100];
    char dest_path[100];
    const char*tamano;
    if (tamano_i == 200){
        tamano = "200x200";
    }else if (tamano_i == 100){
        tamano = "100x100";
    }else if (tamano_i == 50){
        tamano = "50x50";
    }
    const char* direc;
    if (strstr(horiantacion, "vertical")!=NULL){
        direc = "vertical";
    }else if (strstr(horiantacion, "horizontal")!=NULL){
        direc = "horizontal";
    }
    const char* archivo=(const char*)(nom_archivo);

    // Construir las rutas completas de origen y destino
    snprintf(src_path, sizeof(src_path), "%s", archivo);
    snprintf(dest_path, sizeof(dest_path), "%s/%s/%s", direc, tamano, archivo);

    // Mover el archivo
    if (rename(src_path, dest_path) != 0) {
        perror("Error al mover el archivo");
    } else {
        printf("Archivo movido exitosamente.\n");
    }
}


//Descripcion de la funcion:      Se encarga de crear los directorios y sub directorios los que despues seran cargados con sus elementos correspondientes
//    Parametros:
//        no recibe parametros
//    Retorno:
//        no tiene retorno

void crear_carpetas() {

    if (mkdir("vertical", 0755) != 0) {
        perror("Error creando carpeta vertical");
        return;
    }

    if (mkdir("horizontal", 0755) != 0) {
        perror("Error creando carpeta horizontal");
        return;
    }

    if (chdir("vertical") != 0) {
        perror("Error cambiando al directorio vertical");
        return;
    }

    if (mkdir("50x50", 0755) != 0 || mkdir("100x100", 0755) != 0 || mkdir("200x200", 0755) != 0) {
        perror("Error creando subcarpetas en vertical");
        return;
    }

    if (chdir("..") != 0) {
        perror("Error cambiando al directorio CWD");
        return;
    }

    if (chdir("horizontal") != 0) {
        perror("Error cambiando al directorio horizontal");
        return;
    }

    if (mkdir("50x50", 0755) != 0 || mkdir("100x100", 0755) != 0 || mkdir("200x200", 0755) != 0) {
        perror("Error creando subcarpetas en horizontal");
        return;
    }

    if (chdir("..") != 0) {
        perror("Error cambiando al directorio CWD");
        return;
    }
    printf("Estructura de carpetas creada exitosamente.\n");
}


//Descripcion de la funcion:      Se encarga de tomar una linea de la matriz y elimina los espacios entre las letras
//    Parametros:                 
//        linea(char*):           Linea a la cual se le eliminaran los espacios
//    Retorno:
//        char*:                  Se retorna una cadena de char igual a la linea recibida pero sin los espacios

char* limpiar_linea(char* linea) {
    char* limpio = linea;
    char* aux = linea;
    
    while (*aux) {
        if (!isspace(*aux)) {
            *limpio = *aux;
            limpio++;
        }
        aux++;
    }
    *limpio = '\0';
    return linea;
}


//Descripcion de la funcion:      Se encarga de determinar el tamaño de la matriz simetrica que genera el archivo  
//    Parametros:                 
//        archivo(char*):         Nombre del archivo a leer para determinar el tamaño de la matriz
//    Retorno:
//        int:                    Se retorna un enetero que determina el tamaño de la matriz que genera el archivo

int tamano_file(char* archivo) {
    FILE* fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo\n");
        return -1;
    }
    char linea[500];
    fgets(linea, 20, fp);
    fgets(linea, 500, fp);
    char* limpio;
    limpio=limpiar_linea(linea);
    fclose(fp);
    return strlen(limpio);
}



//Descripcion de la funcion:      Se encarga de determinar el tamaño la orientacion de la palabra dentro de la matriz  
//    Parametros:                 
//        archivo(char*):         Nombre del archivo a leer para determinar la orientacion
//    Retorno:
//        char*:                  Se entrega una cadena de char que determina la orientacion de la palabra dentro de la matriz

char* orientacion(char* archivo){
    FILE* fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo\n");
        return NULL;
    }
    char* linea = (char*) malloc(20 * sizeof(char));
    fgets(linea, 20, fp);
    fclose(fp);
    return linea;   
}


//Descripcion de la funcion:      Se encarga de poner en mayusculas una palabra recibida
//    Parametros:                 
//        texto(char*):           Texto que se reescribira en mayusculas
//    Retorno:
//        char*:                  Copia de la palabra recibida pero solo en mayusculas

char* mayusculas(const char* texto) {
    char* copia = malloc(strlen(texto) + 1); // Asignar memoria para la copia del texto
    if (copia == NULL) {
        printf("Error asignando memoria\n");
        return NULL;
    }
    strcpy(copia, texto); // Copiar el texto original a la copia
    char* punto = strchr(copia, '.');
    if (punto != NULL) {
        *punto = '\0';
    }
    char* aux = copia;
    while (*aux) {
        *aux = toupper(*aux);
        aux++;
    }
    return copia;
}


//Descripcion de la funcion:      Se encarga de buscar directamente en el archivo de texto de manera horizontal la palabra buscada
//    Parametros:                 
//        archivo(char*):         Archivo donde se busca la palabra seleccionada
//    Retorno:
//        char*:                  Retorna 0 para cuando no se encuentra la palabra y 1 para cuando si la encuentra

int buscar_string_horizontal(char* archivo) {
    FILE* fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 0;
    }
    char* string =mayusculas(archivo);
    char linea[500];
    fgets(linea, 20, fp);
    char* limpio;
    while (fgets(linea, 500, fp) != NULL) {
        limpio=limpiar_linea(linea);
        if (strstr(limpio, string) != NULL) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


//Descripcion de la funcion:      Se encarga de rotar la matriz
//    Parametros:                 
//        matriz(char**):         Matriz a rotar
//        escala(int):            Tamaño de la matriz a rotar         
//    Retorno:
//        char**:                 Retorna la matriz rotada

char** intercambiarElementos(char** matriz, int escala) {
    char** matrizRetorno = malloc(escala * sizeof(char*));
    if (matrizRetorno == NULL) {
        printf("Error asignando memoria\n");
        return NULL;
    }
    for (int i = 0; i < escala; i++) {
        matrizRetorno[i] = malloc(escala * sizeof(char));
        if (matrizRetorno[i] == NULL) {
            printf("Error asignando memoria\n");
            return NULL;
        }
    }
    for (int i = 0; i < escala; i++) {
        for (int j = 0; j < escala; j++) {
            matrizRetorno[j][i] = matriz[i][j];
        }
    }
    return matrizRetorno;
}


//Descripcion de la funcion:      Ve la cantidad de archivos .txt dentro del directorio donde se encuentra, y devuelve por referencia la cantidad de estos.
//    Parametros:                 
//        archivos(char**):       Lista de strings vacia a la cual se le cargara la informacion
//        num_archivos(int):      Numero que sera la cantidad de archivos .txt dentro del directorio
//    Retorno:
//        No tiene retorno directo

void leer_archivos(char archivos[][201], int* num_archivos) {
    char extension[5]=".txt";
    DIR *dir;
    struct dirent *ent;
    int i = 0;
    dir = opendir(".");
    if (dir == NULL) {
        printf("No se pudo abrir el directorio.\n");
        return;
    }
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) { 
            if (strstr(ent->d_name, extension) != NULL) {
                strcpy(archivos[i], ent->d_name); 
                i++;
            }
        }
    }
    *num_archivos = i;
    closedir(dir);
}


//Descripcion de la funcion:      Se encarga de crear una matriz de una escala seleccionada
//    Parametros:
//        archivo(char*):         Nombre del archivo del cual se crear una matriz a partir de la sopa de letras que este contiene
//        escala(int):            Tamaño de la matriz simetrica
//    Retorno:
//        char**:                 Matriz Creada a partir de la sopa de letras del tamaño seleccionado

char** crear_matriz(char* archivo, int escala) {
    int i, j;
    char** matriz = malloc(escala * sizeof(char*));
    for (i = 0; i < escala; i++) {
        matriz[i] = malloc(escala * sizeof(char));
    }
    FILE* fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo\n");
        return NULL;
    }
    char x[20];
    fgets(x, 20, fp);
    for (i = 0; i < escala; i++) {
        for (j = 0; j < escala; j++) {
            fscanf(fp, "%c ", &matriz[i][j]);
        }
    }
    fclose(fp);
    return matriz;
}

//Descripcion de la funcion:      Se encarga de buscar una palabra en horizontal dentro de una matriz
//    Parametros:
//        matriz(char**):         Matriz donde se busca la palabra
//        palabra(char*):         Palabra a buscar dentro de la matriz
//        escala(int):            Tamaño de la matriz
//   Retorno:
//        int:                    Retorna 0 para cuando no se encuentra la palabra y 1 para cuando si la encuentra

int buscar_palabra_horizontal(char** matriz, char* palabra, int escala) {
    int i, j, k, len;
    char* palabraM=mayusculas(palabra);
    len = strlen(palabraM);
    for (i = 0; i < escala; i++) {
        for (j = 0; j < escala - len + 1; j++) {
            int encontrado = 1;
            for (k = 0; k < len; k++) {
                if (matriz[i][j + k] != palabraM[k]) {
                    encontrado = 0;
                    break;
                }
            }
            if (encontrado) {
                return 1;
            }
        }
    }
    return 0;
}


//Descripcion de la funcion:      Libera la memoria asignada a las matricez
//    Parametros:
//        matriz(char**):         Matriz a liberar
//        escala(int):            Tamaño de la matriz
//    Retorno:
//        No tiene retorno

void liberar_matriz(char** matriz, int escala) {
    int i;
    for (i = 0; i < escala; i++) {
        free(matriz[i]);
    }
    free(matriz);
}


//Descripcion de la funcion:      Se encarga de buscar una palabra seleccionada verticalmente dentro de una matriz
//    Parametros:
//        matriz(char**):         Matriz donde se busca la palabra
//        palabra(char*):         Palabra a buscar dentro de la matriz
//        escala(int):            Tamaño de la matriz
//    Retorno:
//        int:                    Retorna 0 para cuando no se encuentra la palabra y 1 para cuando si la encuentra

int busqueda(char** matriz, char* palabra, int escala){
    int i, j, k, len;
    k=0;
    char * palabraM=mayusculas(palabra);
    len=strlen(palabraM);
    for (i=0; i<escala; i++){
        for(j=0; j<escala; j++){
            if (matriz[j][i]==palabraM[k]){
                k++;
                if (k==(len-1)){
                    return 1;
                }
            }else{
                k=0;
            }
            
        }
    }
    return 0;
}