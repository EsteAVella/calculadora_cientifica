#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reconocimiento.h"

//defini un tamanio maximo para los digitos de un numero, si quieren lo podemos cambiar
#define MAX_DIGITOS_NUMERO 32

void tokenizar(char *cadena, ecuacion_t *ecu){

    int contadorNum=0; //cuenta cant de digitos de un numero. Despues vuelve a cero.
    char* numeroAux = (char*)malloc(sizeof(char) * MAX_DIGITOS_NUMERO);

    Token cargaEnArray;
    char *posicionCadena=cadena;

    if (numeroAux == NULL) {
        printf("Error: No hay memoria para buffer de tokenizar.\n");
        return;
    }

    strcpy(ecu->cadenaOriginal,cadena); //Guardo la cadena original en la estructura.
    ecu->cantTokens=0; //Empieza en cero, porque el array de tokens todavia no tiene nada.

    while( *(posicionCadena)!='\0'&&*(posicionCadena)!='\n'){

        if(ES_DIGITO( *(posicionCadena) )){

            while(ES_DIGITO( *(posicionCadena))){
                if (contadorNum < MAX_DIGITOS_NUMERO - 1) {         //el -1 es para despues poner el \0
                    *(numeroAux + contadorNum) = *(posicionCadena); //((como lee de adelante para atrás, va guardando el numero bien))
                    contadorNum++;
                }
                posicionCadena++;
            }
            //Le pongo el '\0' despues del ultimo digito.
            *(numeroAux + contadorNum)='\0';

            //Le doy el tipo de tipo de token que voy a cargar en el array.
            cargaEnArray.tipo=DIGITO;

            //Paso la cadena numeroAux a atoi, para que la transforme en int, y lo guardo en el token.
            cargaEnArray.valor=atoi(numeroAux);

            //Guardo el token recien cargado en su posicion correspondiente, dentro del array de tokens (en la estructura ecuacion_t).
            *(ecu->ecuacion + ecu->cantTokens) = cargaEnArray;

            //Sumo en uno la cantidad de token, para saber cuantos indices del array hay cargados.
            ecu->cantTokens++;
            contadorNum=0;
        }
        if (ES_OPERACION(*posicionCadena)) {
            cargaEnArray.tipo = OPERACION;
            cargaEnArray.operacion = *posicionCadena;
            *(ecu->ecuacion + ecu->cantTokens) = cargaEnArray;
            ecu->cantTokens++;
            posicionCadena++;
        }
        if (ES_INCOGNITA(*posicionCadena)) {
            cargaEnArray.tipo = INCOGNITA;
            cargaEnArray.incognita = *posicionCadena;
            *(ecu->ecuacion + ecu->cantTokens) = cargaEnArray;
            ecu->cantTokens++;
            posicionCadena++;
        }
    }

    free(numeroAux);
}

void mostrarTokens(ecuacion_t *ecu) {
    Token t;

    printf("Cadena original: %s\n", ecu->cadenaOriginal);
    printf("Cantidad de tokens: %d\n", ecu->cantTokens);

    for (int i = 0; i < ecu->cantTokens; i++) {
        t = *(ecu->ecuacion + i);
        printf("Token %d -> ", i);

        switch (t.tipo) {
            case DIGITO:
                printf("DIGITO (%d)\n", t.valor);
                break;

            case OPERACION:
                printf("OPERACION (%c)\n", t.operacion);
                break;

            case INCOGNITA:
                printf("INCOGNITA (%c)\n", t.incognita);
                break;

            default:
                printf("DESCONOCIDO\n");
                break;
        }
    }
}


void mostrarPostfija(ecuacion_t *ecu) {
    Token t;

    printf("Cadena original: %s\n", ecu->cadenaOriginal);
    printf("\n\nCantidad de tokens en postfija: %d\n", ecu->cantTokensPostfijos);

    for (int i = 0; i < ecu->cantTokensPostfijos; i++) {
        t = *(ecu->ecuacionPostfija + i);
        printf("Token %d -> ", i);

        switch (t.tipo) {
            case DIGITO:
                printf("DIGITO (%d)\n", t.valor);
                break;

            case OPERACION:
                printf("OPERACION (%c)\n", t.operacion);
                break;

            case INCOGNITA:
                printf("INCOGNITA (%c)\n", t.incognita);
                break;

            default:
                printf("DESCONOCIDO\n");
                break;
        }
    }
}

int validarTokens(ecuacion_t* ecu) {
    Token* p = ecu->ecuacion;
    Token* fin = ecu->ecuacion + ecu->cantTokens;
    Token* anterior = NULL;
    char opActual, opAnterior;

    for (; p < fin; p++) {

        if (p->tipo == OPERACION && anterior == NULL) {
            opActual = p->operacion;
            if (opActual != '_' && opActual != '(') {
                return 0;
            }
        }

        if (anterior != NULL) {

            if ((anterior->tipo == DIGITO && p->tipo == DIGITO) ||
                (anterior->tipo == INCOGNITA && p->tipo == DIGITO) ||
                (anterior->tipo == DIGITO && p->tipo == INCOGNITA) ||
                (anterior->tipo == INCOGNITA && p->tipo == INCOGNITA)) {
                return 0;
            }

            if (anterior->tipo == OPERACION && p->tipo == OPERACION) {
                opAnterior = anterior->operacion;
                opActual = p->operacion;
                if (opAnterior != '(' && opActual != '_' && opActual != '(') {
                    return 0;
                }
            }

            if (anterior->tipo == OPERACION && p->tipo == INCOGNITA) {
                opAnterior = anterior->operacion;
                if (opAnterior == ')') {
                    return 0;
                }
            }

            if (anterior->tipo == OPERACION && p->tipo == DIGITO) {
                opAnterior = anterior->operacion;
                if (opAnterior == ')') {
                    return 0;
                }
            }

            if ((anterior->tipo == DIGITO || anterior->tipo == INCOGNITA) &&
                p->tipo == OPERACION) {
                opActual = p->operacion;
                if (opActual == '(') {
                    return 0;
                }
            }
        }

        anterior = p;
    }

    if (anterior != NULL && anterior->tipo == OPERACION) {
        opActual = anterior->operacion;
        if (opActual != ')' && opActual != '_') {
            return 0;
        }
    }

    return 1;
}
