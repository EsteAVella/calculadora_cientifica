#ifndef DIBUJO_H_INCLUDED
#define DIBUJO_H_INCLUDED

#include "conversion.h"

//Funcion principal (menu)
void dibujarInicio();

//Funciones secundarias
void limpiarConsola();
void pausa();
void limpiarBufferEntrada();

//Punto A
void escribirEcuacion(ecuacion_t*, int*);
char verificarEcuacionEscrita(char*);
void eliminarEcuacion(ecuacion_t*, int*);

//Punto B
void verEcuaciones(ecuacion_t*, int);

void guardarReiniciar(ecuacion_t*,int*);


//Punto H
void ayuda();
#endif // DIBUJO_H_INCLUDED
