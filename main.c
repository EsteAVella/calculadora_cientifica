#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conversion.h"
#include "dibujo.h"

int main(){

    char *cadena="3*(x+4)-65";
    ecuacion_t ecu;
    char a ='b';

 //ACA DIBUJAMOS EL INICIO
    dibujarInicio(a);

    tokenizar(cadena, &ecu);
    mostrarTokens(&ecu);
    aPostfijo(&ecu);
    mostrarPostfija(&ecu);

    return 0;
}
