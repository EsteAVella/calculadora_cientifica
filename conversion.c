#include "conversion.h"
#include "dibujo.h"

int precedencia(char op) {
    switch (op) {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '^':
            return 3;
        case '~':
            return 3;
        default:
            return 0;
    }
}

int asociatividad(char op) {
    // 0 = izquierda, 1 = derecha
    if (op == '^' || op == '~')
        return 1;
    return 0;
}

void aPostfijo(ecuacion_t* ecu) {
    Token pila[50];
    Token* pTope = pila - 1;  // apunta "antes" del inicio, xq tengo pila vacia
    Token* pIn = ecu->ecuacion;                // puntero de lectura
    Token* pFin = ecu->ecuacion + ecu->cantTokens;
    Token* pOut = ecu->ecuacionPostfija;       // puntero de escritura en salida

    ecu->cantTokensPostfijos = 0;

    for (; pIn < pFin; pIn++) {
        if (pIn->tipo == DIGITO || pIn->tipo == INCOGNITA) { //si es digito o incognita
            *pOut = *pIn; //voy escribiendo la ecuacion posfija
            pOut++;
            ecu->cantTokensPostfijos++; //aumento la cantidad de tokens posfijos
        }
        else if (pIn->tipo == OPERACION) {
            char op = pIn->operacion;

            if (op == '(') { //si es parentesis de apertura siempre se guarda arriba en la pila
                pTope++;
                *pTope = *pIn;
            }
            else if (op == ')') {
                while (pTope >= pila && pTope->operacion != '(') {
                    *pOut = *pTope;
                    pOut++;
                    ecu->cantTokensPostfijos++;
                    pTope--;
                }
                pTope--; // descartar '('
            }
            else {
                while (pTope >= pila && pTope->operacion != '(' &&              //mientras mi puntero de tope este en la pila, el operador no sea un '(',
                       (precedencia(pTope->operacion) > precedencia(op) ||      //la precendencia del operador del tope sea mayor a la que estoy guardando
                       (precedencia(pTope->operacion) == precedencia(op) &&     //o sean iguales, pero asociatividad p/izq
                        asociatividad(op) == 0))) {
                    *pOut = *pTope;                     //saco lo que haya en el puntero tope y lo escribo en la ecuacion posfija
                    pOut++;                             //incremento el puntero de escritura
                    ecu->cantTokensPostfijos++;               //incremento la cantidad de tokens posfijos
                    pTope--;                            //decremento el puntero tope, ya que saque un operador, esto se cumple en loop con las condiciones d arriba
                }
                pTope++;                    //cuando termino de sacar todos los operadores de mayor precedencia, etc, incremento el puntero tope
                *pTope = *pIn;              //y le guardo el valor del puntero de lectura
            }
        }
    }

    // Vaciar pila
    while (pTope >= pila) { //al final, vacio la pila de operadores que tenga
        *pOut = *pTope;         //voy escribiendo con lo del tope
        pOut++;                 //aumento la posicion de escritura
        ecu->cantTokensPostfijos++;   //aumento cantidad d etokens
        pTope--;                //decremento posicion de tope
    }
}
