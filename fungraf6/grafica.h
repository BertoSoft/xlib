#ifndef GRAFICA_H_INCLUDED
#define GRAFICA_H_INCLUDED

#include "ui.h"

Window          w_grafica;
GC              gc_grafica;
Datos           dat_grafica;

void initGrafica();
void closeGrafica();
void showGrafica();
void pintaGrafica();
void pintaEjes();
void pintaFuncion();


#endif // GRAFICA_H_INCLUDED
