#ifndef RECONOCIMIENTO_H_INCLUDED
#define RECONOCIMIENTO_H_INCLUDED

#define ES_DIGITO(c) ((c) >= '0' && (c) <= '9')

#define ES_OPERACION(c) ((c) == '+' || \
                         (c) == '-' || \
                         (c) == '*' || \
                         (c) == '/' || \
                         (c) == '(' || \
                         (c) == ')' || \
                         (c) == '^' || \
                         (c) == 'v' || \
                         (c) == '_')

#define ES_INCOGNITA(c) ( (c)=='x'||\
                          (c)=='X'||\
                          (c)=='y'||\
                          (c)=='Y')

#include "estructuras.h"

void tokenizar(char*, ecuacion_t*);
void mostrarTokens(ecuacion_t*);
void mostrarPostfija(ecuacion_t*);

#endif // RECONOCIMIENTO_H_INCLUDED
