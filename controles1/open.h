#ifndef OPEN_H_INCLUDED
#define OPEN_H_INCLUDED

#include "ui.h"

//
// Variables
//
int     max_et;
char    ruta[1024];
int     fila_seleccionada;


//
// Controles
//
DatosEt et;

void        initOpen();
void        initControles();
void        showOpen();
void        pintaOpen();
void        pintaLabels();
void        pintaTabla();
void        pintaBotones();
void        closeOpen();
void        openClick(XEvent ev);
void        tablaClick(int fila);
void        tablaDoubleClick(XEvent ev);

DatosDir    getListado(char *ruta);

#endif // OPEN_H_INCLUDED
