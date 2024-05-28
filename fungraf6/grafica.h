#ifndef GRAFICA_H_INCLUDED
#define GRAFICA_H_INCLUDED

#include "ui.h"

Window          w;
GC              gc;
Datos           dat;

void initGrafica();
void closeGrafica();
void showGrafica();
void pintaGrafica();
void pintaEjes();
void pintaFuncion();


#endif // GRAFICA_H_INCLUDED
