#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "estructuras.h"
#include "reconocimiento.h"
#include "dibujo.h"

#define MAX_ECUACIONES 10
#define ARCHIVO_CONTADOR "contador.bin"
#define MAX_ARCHIVOS 10
#define MAX_VALORES 10
#define LISTA_ARCHIVO "nombreArchivos.txt"

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
           case 'B': verEcuaciones(ecuaciones, cantEcuacionesActuales);break;
           case 'C': guardarReiniciar(ecuaciones,&cantEcuacionesActuales);break;
           case 'D': leerEcuaciones(ecuaciones,&cantEcuacionesActuales);break;
           case 'E': borrarEcuaciones(); break;
           case 'F': resolverEcuacion(ecuaciones, &cantEcuacionesActuales); break;
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

//Funciones compartidas entre C, D, E
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

//Punto A
void escribirEcuacion(ecuacion_t *ecuacion, int *posicion) {

    //posicion es la cant. de ecuaciones cargadas, pero me parecio mas claro
    //ponerle posicion, ya que en ese numero se guardara la nueva ecuacion.

    char cadenaAux[50];
    char permitido='0', opcionAyuda, opcionEdicion;

    ecuacion_t *punteroEcuacion=ecuacion;

    punteroEcuacion = punteroEcuacion + (*posicion);

    limpiarBufferEntrada();

    while(permitido=='0'){
        printf("Ingrese la ecuacion: ");
        fgets(cadenaAux,sizeof(cadenaAux),stdin);
        permitido=verificarEcuacionEscrita(cadenaAux);

        if(permitido=='0'){
            printf("\nLa ecuacion escrita es invalida. Desea ver la AYUDA? s/n: ");
            scanf(" %c", &opcionAyuda);
            if(tolower(opcionAyuda)=='s')
                ayuda();
        }
    }

    limpiarConsola();
    printf("\n La ecuacion ingresada es: %s", cadenaAux);
    printf("\nDesea editarla (s/n)? ");
    scanf(" %c", &opcionEdicion);
    limpiarBufferEntrada();
    if(tolower(opcionEdicion)=='s'){
        edicionEcuacion(cadenaAux);
        printf("\n La nueva ecuacion ingresada es: %s", cadenaAux);
    }

    strcpy(punteroEcuacion->cadenaOriginal, cadenaAux);
    tokenizar(punteroEcuacion->cadenaOriginal, punteroEcuacion);
    aPostfijo(punteroEcuacion);
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

void edicionEcuacion(char* ecuacionAnterior){
    char nuevaEcuacion[50];
    char permitido='0';

    do{
        printf("\nIngrese la nueva ecuacion: ");
        fgets(nuevaEcuacion,sizeof(nuevaEcuacion),stdin);

        if (nuevaEcuacion[0] == '\n') {
            printf("No se realizaron cambios. Se mantiene la ecuacion anterior.\n");
            return;
        }
        permitido=verificarEcuacionEscrita(nuevaEcuacion);
    }while(permitido=='0');

    strcpy(ecuacionAnterior, nuevaEcuacion);
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

    FILE *pf;
    ecuacion_t *punteroEcuacion;
    punteroEcuacion = ecuacion;

    int cantidadActual = contarSesionesGuardadas();
    if(cantidadActual >= MAX_ARCHIVOS){
        printf("Ya hay %d sesiones guardadas. Elimine antes de guardar otra.\n",MAX_ARCHIVOS);
        pausa();
        limpiarBufferEntrada();
        return;
    }

    char *nombreArchivo = pedirNombreArchivo();
    if(!nombreArchivo) return;

    pf = fopen(nombreArchivo,"w");
    if(!pf){
        printf("Error al abrir el archivo");
        free(nombreArchivo);
        return;
    }

    for(int i = 0; i < *cant; i++){
        fprintf(pf,"%s\n", punteroEcuacion->cadenaOriginal);
        punteroEcuacion++;
    }
    fclose(pf);

    printf("la sesion fue guardada correctamente en %s \n",nombreArchivo);

    //guardamos los nombres en el archivo aux

    guardarNombre(nombreArchivo);

    for(int i = 0; i < *cant; i++){
        ecuacion[i].cadenaOriginal[0] = '\0';
    }

    *cant = 0;

    free(nombreArchivo);

    printf("Sesion actual reiniciada\n");
    pausa();
    limpiarBufferEntrada();
    limpiarConsola();
}

//Punto D
void leerEcuaciones(ecuacion_t *ecuaciones, int *cantEcuaciones) {
    FILE *registro = fopen(LISTA_ARCHIVO,"r");

    if(!registro){
        printf("No hay sesiones guardadas.\n");
        return;
    }
    char nombres [10][100];
    int total = 0;

    while (fgets(nombres[total], sizeof(nombres[total]), registro) && total < 10) {
        nombres[total][strcspn(nombres[total], "\n")] = '\0';
        total++;
    }

    fclose(registro);

    if (total == 0) {
        printf("No hay sesiones guardadas.\n");
        return;
    }

    int numero;

    listarSesionesGuardadas();
    printf("\nSeleccione el número de sesión que desea abrir: ");
    scanf("%d", &numero);
    limpiarBufferEntrada();

    if (numero < 1 || numero > total) {
        printf("Número inválido.\n");
        return;
    }

    FILE *pf = fopen(nombres[numero - 1], "r");
    if (!pf) {
        printf("Error al abrir el archivo %s\n", nombres[numero - 1]);
        return;
    }

    *cantEcuaciones = 0;
    char linea[100];
    while (fgets(linea, sizeof(linea), pf) && *cantEcuaciones < MAX_ECUACIONES) {
        linea[strcspn(linea, "\n")] = '\0';
        strcpy(ecuaciones[*cantEcuaciones].cadenaOriginal, linea);
        (*cantEcuaciones)++;
    }
    fclose(pf);

    printf("\nSe cargaron %d ecuaciones desde '%s'.\n", *cantEcuaciones, nombres[numero - 1]);
    pausa();
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
    FILE *registro = fopen(LISTA_ARCHIVO, "r");
    if (!registro) {
        printf("No hay sesiones guardadas.\n");
        return;
    }

    char nombreArchivo[100];
    int borrados = 0;
    while (fgets(nombreArchivo, sizeof(nombreArchivo), registro)) {
        nombreArchivo[strcspn(nombreArchivo, "\n")] = '\0';
        if (remove(nombreArchivo) == 0) {
            borrados++;
        }
    }
    fclose(registro);

    remove(LISTA_ARCHIVO); // borra también el registro de nombres

    printf("Se eliminaron %d archivos de sesión.\n", borrados);
}

//PUNTO F
void resolverEcuacion(ecuacion_t* ecuaciones, int* cantEcuaciones){
    int nroEcuacion, nroOperacion;
    bool tieneX = 0;
    bool tieneY = 0;
    verEcuaciones(ecuaciones, *cantEcuaciones);
    do{
        printf("Selecciona una de las ecuaciones mostradas para resolver: \n");
        scanf("%d", &nroEcuacion);
    }while(nroEcuacion<1 || nroEcuacion > *cantEcuaciones);
    limpiarConsola();
    ecuacion_t* ecuacionActual = ecuaciones + nroEcuacion - 1;
    printf("Seleccione el tipo de resolucion para la ecuacion: %s\n", ecuacionActual->cadenaOriginal);
    printf("1- Ingresar una tabla de valores\n");
    printf("2- Ingresar un valor y generar el resto de la tabla\n");
    do{
        scanf("%d", &nroOperacion);
    }while(nroOperacion != 1 && nroOperacion != 2);
    printf("Opcion seleccionada: %d\n", nroOperacion);
    int cantTokens = ecuacionActual->cantTokens;
    for(int i = 0; i < cantTokens; i++){
        Token t = *(ecuacionActual->ecuacion + i);
        if(t.tipo == INCOGNITA){
            if(tolower(t.incognita) == 'x'){
                tieneX = 1;
            } else if(tolower(t.incognita)== 'y'){
                tieneY = 1;
            }
        }
    }
    dibujarTabla(tieneX, tieneY, ecuacionActual, nroOperacion);
}
void dibujarTabla(bool tieneX, bool tieneY, ecuacion_t* ecuacion, int nroOperacion){
    int cantidadValores;
    float* valoresX = NULL;
    float* valoresY = NULL;
    if(nroOperacion == 1){
        do{
            printf("¿Cuantos valores quiere darle a la incognita? (El maximo es %d)\n", MAX_VALORES);
            scanf("%d", &cantidadValores);
        }while(cantidadValores < 1 || cantidadValores > MAX_VALORES);
        if(tieneX){
            valoresX = malloc(sizeof(float) * cantidadValores);
            if (valoresX == NULL) {
                printf("Error: No hay memoria.\n");
                return;
            }
            printf("Ingrese los valores para reemplazar x:\n");
            for(int i = 0; i<cantidadValores; i++){
                printf("Valor %d: \n", i+1);
                scanf("%f", valoresX + i);
            }
        }
        if(tieneY){
            valoresY = malloc(sizeof(float) * cantidadValores);
            if (valoresY == NULL) {
                printf("Error: No hay memoria.\n");
                return;
            }
            printf("Ingrese los valores para reemplazar y:\n");
            for(int i = 0; i<cantidadValores; i++){
                printf("Valor %d: \n", i+1);
                scanf("%f", valoresY + i);
            }
        }
    } else if(nroOperacion == 2){
        cantidadValores = 11;
        if(tieneX){
            valoresX = malloc(sizeof(float) * cantidadValores);
            if (valoresX == NULL) {
                printf("Error: No hay memoria.\n");
                return;
            }
            float valorCentralX;
            printf("Ingrese un valor para reemplazar a x:\n");
            scanf("%f", &valorCentralX);
            for(int i = 0; i < 11; i++){
                *(valoresX + i) = valorCentralX + (i - 5);
            }
        }
        if(tieneY){
            valoresY = malloc(sizeof(float) * cantidadValores);
            if (valoresY == NULL) {
                printf("Error: No hay memoria.\n");
                return;
            }
            float valorCentralY;
            printf("Ingrese un valor para reemplazar a y:\n");
            scanf("%f", &valorCentralY);
            for(int i = 0; i < 11; i++){
                *(valoresY + i) = valorCentralY + (i - 5);
            }
        }
    }
    limpiarConsola();

    printf("Ecuacion: %s\n", ecuacion->cadenaOriginal);
    printf("\n================ TABLA DE RESULTADOS =================\n");

    if (tieneX) {
        printf("%-15s", "Valor X");
    }
    if (tieneY) {
        printf("%-15s", "Valor Y");
    }
    printf("%15s\n", "Resultado");

    printf("----------------------------------------------\n");

    for (int i = 0; i < cantidadValores; i++) {

        float valX_fila = 0.0f;
        float valY_fila = 0.0f;

        if (tieneX) {
            valX_fila = *(valoresX + i);
        }
        if (tieneY) {
            valY_fila = *(valoresY + i);
        }

        float resultado = resolverEcuacionEvaluada(ecuacion, valX_fila, valY_fila);

        if (tieneX) {
            printf("%-15.2f", valX_fila);
        }
        if (tieneY) {
            printf("%-15.2f", valY_fila);
        }
        printf("%15.2f\n", resultado);
    }
    printf("=====================================================\n");
    free(valoresX);
    free(valoresY);
}

//Punto H
void ayuda() {
    limpiarConsola();
    printf("[H] Ayuda\n");
    printf("1. La calculadora tiene por ingresos solamente las letras x y \n");
    printf("2. No se puede dividir por 0\n");
    printf("3. Los operandos validos son (+ suma) (- resta) (* multiplicacion) (/ division) (^ potencia) (v raiz) \n");
    printf("4. Se pueden utilizar parentesis, tenga cuidado que estos agrupan segun lo que se ingrese \n");
    printf("5. La calculadora acepta el operador de resta unario, pero al ingresarlo se lo debe distinguir de la siguiente forma:\n '-' (resta binaria), '_' (resta unaria)\n");
    printf("6. Si se desea hacer una multiplicacion, esta debe aparecer de forma estrica, con el operador (*). Invalido: 7x // Pemitido: 7*x \n");
    pausa();
}

char* pedirNombreArchivo(){

    char buffer[100];
    printf("Ingrese nombre para el archivo de texto: ");
    scanf("%99s",buffer);

    char *nombre = malloc(strlen(buffer)+5);
    if(!nombre){
        printf("Error al reservar memoria.\n");
        return NULL;
    }
    sprintf(nombre,"%s.txt",buffer);

    return nombre;
}

void listarSesionesGuardadas() {
    FILE *registro = fopen(LISTA_ARCHIVO, "r");
    if (!registro) {
        printf("No hay sesiones guardadas aún.\n");
        return;
    }

    char nombre[100];
    int i = 1;
    printf("\n=== SESIONES GUARDADAS ===\n");
    while (fscanf(registro, "%99s", nombre) == 1) {
        printf("[%d] %s\n", i++, nombre);
    }

    fclose(registro);
}

int contarSesionesGuardadas() {
    FILE *registro = fopen(LISTA_ARCHIVO, "r");
    if (!registro) return 0; // si no existe, aún no hay sesiones

    int contador = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), registro)) {
        contador++;
    }

    fclose(registro);
    return contador;
}

void guardarNombre(char* nombreArchivo){

    FILE *registro = fopen(LISTA_ARCHIVO,"a");
    if(registro){
        fprintf(registro,"%s\n",nombreArchivo);
        fclose(registro);
    }
}
