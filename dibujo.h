#ifndef DIBUJO_H_INCLUDED
#define DIBUJO_H_INCLUDED

#include <stdbool.h>
#include "conversion.h"

//Funcion principal (menu)
void dibujarInicio();

//Funciones secundarias
void limpiarConsola();
void pausa();
void limpiarBufferEntrada();
int  leerContador();
void guardarContador(int);
int abrirSesion(ecuacion_t*, int);

//Punto A
void escribirEcuacion(ecuacion_t*, int*);
char verificarEcuacionEscrita(char*);
void edicionEcuacion(char*);
void eliminarEcuacion(ecuacion_t*, int*);

//Punto B
void verEcuaciones(ecuacion_t*, int);

//Punto C
void guardarReiniciar(ecuacion_t*,int*);

//Punto D
void leerEcuaciones(ecuacion_t*,int*);

//Punto E
void borrarEcuaciones();

//PUNTO F
void resolverEcuacion(ecuacion_t*,int*);
void dibujarTabla(bool, bool, ecuacion_t*);

//Punto H
void ayuda();
#endif // DIBUJO_H_INCLUDED
