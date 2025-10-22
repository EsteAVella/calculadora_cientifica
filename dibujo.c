#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "reconocimiento.h"
#include "dibujo.h"

#define MAX_ECUACIONES 10
#define ARCHIVO_CONTADOR "contador.bin"
#define MAX_ARCHIVOS 10


void resolverEcuacion() {
    printf("[F] Resolver ecuacion\n");
}

//Funcion principal (menu)
void dibujarInicio(){

    char opcion;
    ecuacion_t ecuaciones[MAX_ECUACIONES];
    int cantEcuacionesActuales=0;

    do{
        //limpiarConsola();
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("[A] - Escribir ecuacion\n");
        printf("[B] - Ver ecuaciones de esta sesion\n");
        printf("[C] - Guardar y reiniciar sesion\n");
        printf("[D] - Leer ecuaciones guardadas\n");
        printf("[E] - Borrar ecuaciones guardadas\n");
        printf("[F] - Resolver ecuacion\n");
        printf("[H] - Ayuda\n");
        printf("[X] - Salir\n");
        printf("Seleccione una opcion: ");

        scanf(" %c", &opcion);
        opcion = toupper(opcion);

        switch(opcion) {
           case 'A':
                    if(cantEcuacionesActuales<=MAX_ECUACIONES-1){
                        escribirEcuacion(ecuaciones, &cantEcuacionesActuales);
                        cantEcuacionesActuales++;
                    }else{
                        eliminarEcuacion(ecuaciones, &cantEcuacionesActuales);
                    }
                    break;

           case 'B': verEcuaciones(ecuaciones, cantEcuacionesActuales);
                    break;

           case 'C': guardarReiniciar(ecuaciones,&cantEcuacionesActuales);
                    break;

           case 'D': leerEcuaciones(ecuaciones,&cantEcuacionesActuales);
                    break;


           case 'E': borrarEcuaciones(); break;
           case 'F': resolverEcuacion(); break;
           case 'H': ayuda(); break;
           case 'X': printf("Saliendo...\n"); break;
           default: printf("Opción no válida.\n");
        }

    } while(opcion != 'X');

}

//Funciones secundarias
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void pausa(){
    printf("\nPresione ENTER para volver al menu");
    getchar();
    getchar();
}
void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//Punto A
void escribirEcuacion(ecuacion_t *ecuacion, int *posicion) {

    //posicion es la cant. de ecuaciones cargadas, pero me parecio mas claro
    //ponerle posicion, ya que en ese numero se guardara la nueva ecuacion.

    char cadenaAux[50];
    char permitido='0';

    ecuacion_t *punteroEcuacion=ecuacion;

    punteroEcuacion = punteroEcuacion + (*posicion);

    limpiarBufferEntrada();

    while(permitido=='0'){
        printf("Ingrese la ecuacion: ");
        fgets(cadenaAux,sizeof(cadenaAux),stdin);
        permitido=verificarEcuacionEscrita(cadenaAux);
    }

    strcpy(punteroEcuacion->cadenaOriginal, cadenaAux);
}
char verificarEcuacionEscrita(char *ecuacion){

    char *punteroChar=ecuacion;

    while(*punteroChar!='\n'&&*punteroChar!='\0'){
        if (!(ES_DIGITO(*punteroChar) || ES_INCOGNITA(*punteroChar) || ES_OPERACION(*punteroChar))) {
            return '0';
        }
        punteroChar++;
    }
    *punteroChar='\0';
    return '1';
}
void eliminarEcuacion(ecuacion_t *ecuaciones, int *cantEcuaciones) {

    int posicion;
    ecuacion_t *punteroEcuacion=ecuaciones;

    printf("Lista de ecuaciones\n");
    verEcuaciones(ecuaciones, *cantEcuaciones);

    printf("\nIngrese el numero de la ecuacion a eliminar: ");
    scanf("%d", &posicion);

    limpiarBufferEntrada();

    if (posicion < 1 || posicion > *cantEcuaciones) {
        printf("Numero invalido. No se elimino ninguna ecuacion.\n");
        return;
    }

    // Convertimos el número del usuario a índice real
    posicion--;

    // Desplazar las ecuaciones siguientes una posición hacia arriba
    for (int i = posicion; i < *cantEcuaciones - 1; i++) {
        punteroEcuacion[i] = punteroEcuacion[i+1];
    }

    //Ahora queda limpiar el ultimo lugar, el cual esta repetido, despues de haber corrido todo
    //a la izquierda. Para eso, llevo el punteroEcuacion al ultimo lugar posible del array.
    punteroEcuacion= ecuaciones+ (*cantEcuaciones-1);

    punteroEcuacion->cadenaOriginal[0]='\0';
    punteroEcuacion->cantTokens=0;
    punteroEcuacion->cantTokensPostfijos=0;
    memset(punteroEcuacion->ecuacion, 0, sizeof(punteroEcuacion->ecuacion));
    memset(punteroEcuacion->ecuacionPostfija, 0, sizeof(punteroEcuacion->ecuacionPostfija));
    // Actualizar el contador global
    (*cantEcuaciones)--;

    printf("Ecuacion eliminada correctamente.\n");
}


//Punto B
void verEcuaciones(ecuacion_t *ecuacion, int cantidadEcuaciones) {

    int i=0;
    ecuacion_t *punteroEcuacion;
    punteroEcuacion=ecuacion;

    limpiarConsola();
    for(i = 0; i < cantidadEcuaciones; i++){
        printf("Ecuacion Nro.%d: %s\n", i + 1, punteroEcuacion->cadenaOriginal);
        punteroEcuacion++;
    }
}

//Punto C
void guardarReiniciar(ecuacion_t *ecuacion,int *cant) {
    int numeroArchivo = leerContador();
    FILE *pf;

    char nombreArchivo[30];
    ecuacion_t *punteroEcuacion;
    punteroEcuacion = ecuacion;

    sprintf(nombreArchivo,"ecuaciones-%d.txt",numeroArchivo);

    pf = fopen(nombreArchivo,"w");
    if(!pf){
        printf("Error al abrir el archivo");
        return;
    }

    for(int i = 0; i < *cant; i++){
        fprintf(pf,"%s\n", punteroEcuacion->cadenaOriginal);
        punteroEcuacion++;
    }
    fclose(pf);

    printf("la sesion fue guardada correctamente en %s \n",nombreArchivo);

    for(int i = 0; i < *cant; i++){
        ecuacion[i].cadenaOriginal[0] = '\0';
    }

    *cant = 0;
    numeroArchivo++;
    if (numeroArchivo > MAX_ARCHIVOS) numeroArchivo = 1;
    guardarContador(numeroArchivo);
    printf("Sesion actual reiniciada\n");

    limpiarBufferEntrada();
    pausa();
    limpiarConsola();
}

int leerContador(void) {

    FILE *pf = fopen(ARCHIVO_CONTADOR, "rb");
    int contador = 1;

    if(!pf){
        return -9;
    }

    if (pf) {
        if (fread(&contador, sizeof(int), 1, pf) != 1) {
            contador = 1;
        }
        fclose(pf);
    } else {
        // caso base cuando no hay archivo crearlo con el 1 solo
        pf = fopen(ARCHIVO_CONTADOR, "wb");
        if (pf) {
            fwrite(&contador, sizeof(int), 1, pf);
            fclose(pf);
        } else {
            printf("No se pudo crear %s\n", ARCHIVO_CONTADOR);
        }
    }
    //Chqueo que no se vaya de rango si es 10 lo mandamos a 1
    if (contador < 1 || contador > MAX_ARCHIVOS) contador = 1;
    return contador;
}

void guardarContador(int contador) {
    FILE *pf = fopen(ARCHIVO_CONTADOR, "wb");
    if (pf) {
        fwrite(&contador, sizeof(int), 1, pf);
        fclose(pf);
    } else {
        printf("No se pudo abrir %s para escribir el contador\n", ARCHIVO_CONTADOR);
    }
}

int abrirSesion(ecuacion_t *ecuaciones, int numeroSesion) {
    FILE *pf;
    char nombreArchivo[30];
    int i = 0;

    sprintf(nombreArchivo, "ecuaciones-%d.txt", numeroSesion);
    pf = fopen(nombreArchivo, "r");

    if (!pf) {
        printf("Error al abrir %s\n", nombreArchivo);
        return 0;
    }

    while (fgets(ecuaciones[i].cadenaOriginal, sizeof(ecuaciones[i].cadenaOriginal), pf)) {
        ecuaciones[i].cadenaOriginal[strcspn(ecuaciones[i].cadenaOriginal, "\n")] = '\0';
        i++;
        if (i >= MAX_ECUACIONES) break;
    }

    fclose(pf);
    return i;
}

//Punto D
void leerEcuaciones(ecuacion_t *ecuaciones, int *cantEcuaciones) {
    int cantidadGuardadas = leerContador();
    int numero;

    if (cantidadGuardadas <= 0) {
        printf("No hay sesiones guardadas.\n");
        return;
    }

    printf("\n=== SESIONES GUARDADAS ===\n");
    for (int i = 1; i <= cantidadGuardadas; i++) {
        printf("[%d] ecuaciones-%d.txt\n", i, i);
    }

    printf("\nSeleccione el número de sesión que desea abrir: ");
    scanf("%d", &numero);

    if (numero < 1 || numero > cantidadGuardadas) {
        printf("Número inválido.\n");
        return;
    }

    *cantEcuaciones = abrirSesion(ecuaciones, numero);
    printf("\nSe cargaron %d ecuaciones en la sesión actual.\n", *cantEcuaciones);
}

//Punto E
void borrarEcuaciones(int numero) {
    char op;
    printf("Esta completamente seguro de eliminar todos los archivos? (s/n): ");
    op = getchar();
    limpiarBufferEntrada(); // para limpiar el ENTER que queda en el buffer

    if (op == 'n' || op == 'N') {
        printf("No se eliminaron los archivos.\n");
        return; // salir de la función
    }

    int archivosBorrados = 0;
    char nombreArchivo[30];

    for (int i = 1; i <= MAX_ARCHIVOS; i++) {
        sprintf(nombreArchivo, "ecuaciones-%d.txt", i);
        if (remove(nombreArchivo) == 0) {
            archivosBorrados++;
        }
    }
    guardarContador(1);
    printf("Se eliminaron %d archivos.\n", archivosBorrados);
}

//Punto H
void ayuda() {
    limpiarConsola();
    printf("[H] Ayuda\n");
    printf("1. La calculadora tiene por ingresos solamente las letras x y \n");
    printf("2. No se puede dividir por 0\n");
    printf("3. Los operandos validos son (+ suma) (- resta) (* multiplicacion) (/ division) (^ potencia) (~ raiz) \n");
    printf("4. Se pueden utilizar parentesis, tenga cuidado que estos agrupan segun lo que se ingrese \n");
    pausa();
}
