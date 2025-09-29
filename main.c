#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conversion.h"

int main(){

    char *cadena="3*(x+4)-65";
    ecuacion_t ecu;

    tokenizar(cadena, &ecu);
    mostrarTokens(&ecu);
    aPostfijo(&ecu);
    mostrarPostfija(&ecu);
}
