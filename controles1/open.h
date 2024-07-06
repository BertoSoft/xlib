#ifndef OPEN_H_INCLUDED
#define OPEN_H_INCLUDED

#include "ui.h"

#define MAX_BTN_OPEN 2

//
// Variables
//
char        ruta[1024];
char        archivo[1024];
int         fila_seleccionada;
DatosBoton  btn_open[2];


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
void        openButtonPress(XEvent ev);
void        openButtonRelease(XEvent ev);
void        openDoubleClick(XEvent ev);

void        tablaClick(int fila);
void        tablaDoubleClick(int fila);
void        btnACeptarClick();
void        btnCancelarClick();


#endif // OPEN_H_INCLUDED
