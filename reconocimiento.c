#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reconocimiento.h"

void tokenizar(char *cadena, ecuacion_t *ecu){

    int contadorNum=0; //cuenta cant de digitos de un numero. Despues vuelve a cero.
    char numeroAux[10]; //cadena en donde se guardaran los digitos que componen a UN solo numero. //podriamos aclarar en el help que se aceptan numeros de hasta 8 digitos, por las dudas

    Token cargaEnArray;
    char *posicionCadena=cadena;


    strcpy(ecu->cadenaOriginal,cadena); //Guardo la cadena original en la estructura.
    ecu->cantTokens=0; //Empieza en cero, porque el array de tokens todavia no tiene nada.

    while( *(posicionCadena)!='\0'&&*(posicionCadena)!='\n'){

        if(ES_DIGITO( *(posicionCadena) )){

            while(ES_DIGITO( *(posicionCadena))){

                numeroAux[contadorNum]=*(posicionCadena); //((como lee de adelante para atrás, va guardando el numero bien))
                contadorNum++;
                posicionCadena++;
            }
            //Le pongo el '\0' despues del ultimo digito.
            numeroAux[contadorNum]='\0';

            //Le doy el tipo de tipo de token que voy a cargar en el array.
            cargaEnArray.tipo=DIGITO;

            //Paso la cadena numeroAux a atoi, para que la transforme en int, y lo guardo en el token.
            cargaEnArray.valor=atoi(numeroAux);

            //Guardo el token recien cargado en su posicion correspondiente, dentro del array de tokens (en la estructura ecuacion_t).
            ecu->ecuacion[ecu->cantTokens]=cargaEnArray;

            //Sumo en uno la cantidad de token, para saber cuantos indices del array hay cargados.
            ecu->cantTokens++;
            contadorNum=0;
        }

        if (ES_OPERACION(*posicionCadena)) {
            cargaEnArray.tipo = OPERACION;
            cargaEnArray.operacion = *posicionCadena;
            ecu->ecuacion[ecu->cantTokens] = cargaEnArray;
            ecu->cantTokens++;
            posicionCadena++;
        }


        if (ES_INCOGNITA(*posicionCadena)) {
            cargaEnArray.tipo = INCOGNITA;
            cargaEnArray.incognita = *posicionCadena;
            ecu->ecuacion[ecu->cantTokens] = cargaEnArray;
            ecu->cantTokens++;
            posicionCadena++;
        }

    }
}



//Esto se lo pedi a chatGPT para poder ver si efectivamente el array de ecuacion_t estaba bien cargado.
void mostrarTokens(ecuacion_t *ecu) {
    printf("Cadena original: %s\n", ecu->cadenaOriginal);
    printf("Cantidad de tokens: %d\n", ecu->cantTokens);

    for (int i = 0; i < ecu->cantTokens; i++) {
        Token t = ecu->ecuacion[i];
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
    printf("Cadena original: %s\n", ecu->cadenaOriginal);
    printf("\n\nCantidad de tokens en postfija: %d\n", ecu->cantTokensPostfijos);

    for (int i = 0; i < ecu->cantTokensPostfijos; i++) {
        Token t = ecu->ecuacionPostfija[i];
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
