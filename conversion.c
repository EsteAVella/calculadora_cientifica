#include "conversion.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_PILA 50

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
    Token pila[TAM_PILA];
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

float resolverEcuacionEvaluada(ecuacion_t* ecu, float valorX, float valorY){
    float pila[50];
    float* pTope = pila - 1; // punteor que apunta al tope de la pila, el ultimo elemento
    Token* pIn = ecu->ecuacionPostfija; //puntero de lectura p/apilado
    Token* pFin = ecu->ecuacionPostfija + ecu->cantTokensPostfijos;

    for (; pIn < pFin; pIn++) { //uso for asi xq yo se que estoy recorriendo, no tengo que estar atento a una condicion de corte como x ej \0
        if (pIn->tipo == DIGITO) { //si es un digito, a la pila
            *(++pTope) = pIn->valor;
        }
        else if (pIn->tipo == INCOGNITA) { //si es una incognita, le asigno valor y a la pila
            if (pIn->incognita == 'x' || pIn->incognita == 'X')
                *(++pTope) = valorX;
            else if (pIn->incognita == 'y' || pIn->incognita == 'Y')
                *(++pTope) = valorY;
        }
        else if (pIn->tipo == OPERACION) { //si es una operacion agarro los 2 ultimos operandos,
            float b = *pTope; pTope--;
            float a = *pTope; pTope--;

            switch (pIn->operacion) { //switch segun operador, en todos los casos aumento el puntero de tope para que no pise lo que este en la pila
                case '+': *(++pTope) = a + b; break;
                case '-': *(++pTope) = a - b; break;
                case '*': *(++pTope) = a * b; break;
                case '/': *(++pTope) = a / b; break;
                case '^': *(++pTope) = powf(a, b); break;
                case '~': *(++pTope) = powf(b, 1.0/a); break;
            }
        }
    }
    return *pTope; // resultado final
}
