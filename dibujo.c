#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define TAM 30

//Funciones Aux para dibujar el menu
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

// funciones que falta implementar pero seria la firma
char* escribirEcuacion() {
    char *eq = malloc(TAM *sizeof(char));
    char *ini = eq;

    if(eq == NULL){
        perror("malloc");
        exit(1);
    }

    limpiarConsola();
    printf("[A] Escribir ecuación\n");

    printf("\n Ingrese la ecuacion \n");
    scanf("%s", eq);

    printf("\n");
    printf("%s",ini);
    pausa();

    return eq;
}

void verEcuaciones() {

    limpiarConsola();
    printf("Usted esta viendo las ecuaciones guardadas\n");
    printf("1. x+y \n");
    printf("2. x*2+y/3 \n");
    printf("3. x^2+y~3 \n");
    pausa();
}

void guardarReiniciar() {
    printf("[C] Guardar y reiniciar sesión\n");
}

void leerEcuaciones() {
    printf("[D] Leer ecuaciones guardadas\n");
}

void borrarEcuaciones() {
    printf("[E] Borrar ecuaciones guardadas\n");
}

void resolverEcuacion() {
    printf("[F] Resolver ecuación\n");
}

void ayuda() {
    limpiarConsola();
    printf("[H] Ayuda\n");
    printf("1. La calculadora tiene por ingresos solamente las letras x y \n");
    printf("2. No se puede dividir por 0\n");
    printf("3. Los operandos validos son (+ suma) (- resta) (* multiplicacion) (/ division) (^ potencia) (~ raiz) \n");
    printf("4. Se pueden utilizar parentesis, tenga cuidado que estos agrupan segun lo que se ingrese \n");
    pausa();
}

void dibujarInicio(char accion){

    char opcion;

    do{
        limpiarConsola();
        printf("\n===== MENÚ PRINCIPAL =====\n");
        printf("[A] - Escribir ecuación\n");
        printf("[B] - Ver ecuaciones de esta sesión\n");
        printf("[C] - Guardar y reiniciar sesión\n");
        printf("[D] - Leer ecuaciones guardadas\n");
        printf("[E] - Borrar ecuaciones guardadas\n");
        printf("[F] - Resolver ecuación\n");
        printf("[H] - Ayuda\n");
        printf("[X] - Salir\n");
        printf("Seleccione una opción: ");

        scanf(" %c", &opcion);
        opcion = toupper(opcion);

        switch(opcion) {
           case 'A': escribirEcuacion(); break;
           case 'B': verEcuaciones(); break;
           case 'C': guardarReiniciar(); break;
           case 'D': leerEcuaciones(); break;
           case 'E': borrarEcuaciones(); break;
           case 'F': resolverEcuacion(); break;
           case 'H': ayuda(); break;
           case 'X': printf("Saliendo...\n"); break;
           default: printf("Opción no válida.\n");
        }

    } while(opcion != 'X');

}
