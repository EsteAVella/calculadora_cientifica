#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conversion.h"
#include "dibujo.h"

int main(){

    char *cadena="3+27-3*x";
    ecuacion_t ecu;

    dibujarInicio();

    tokenizar(cadena, &ecu);
    mostrarTokens(&ecu);
    aPostfijo(&ecu);
    mostrarPostfija(&ecu);
    printf("\n\nEl resultado de evaluar %s, con x = %.2f es: %.2f", cadena, 4.00, resolverEcuacionEvaluada(&ecu, 4, 0));
}
