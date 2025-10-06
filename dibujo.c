#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "reconocimiento.h"
#include "dibujo.h"

#define MAX_ECUACIONES 5

void guardarReiniciar() {
    printf("[C] Guardar y reiniciar sesion\n");
}
void leerEcuaciones() {
    printf("[D] Leer ecuaciones guardadas\n");
}
void borrarEcuaciones() {
    printf("[E] Borrar ecuaciones guardadas\n");
}
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

           case 'C': guardarReiniciar();
                    break;

           case 'D': leerEcuaciones();
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
    for(i=0;i<cantidadEcuaciones;i++){
        printf("Ecuacion Nro.%d: %s\n", i+1, punteroEcuacion->cadenaOriginal);
        punteroEcuacion++;
    }
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
