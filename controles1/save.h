#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#include "ui.h"

#define MAX_BTN_SAVE    2
#define MAX_ET_SAVE     1

//
// Variables
//
DatosEt     et_archivo;
char        ruta[1024];
char        archivo[1024];
int         fila_seleccionada;
DatosBoton  btn_save[2];


//
// Funciones de save
//
void showSave();
void initSave();
void initControlesSave();
void closeSave();
void pintaSave();
void pintaLabelsSave();
void pintaEtsSave();
void pintaTablaSave();
void pintaBotonesSave();

void saveButtonPress(XEvent ev);
void saveButtonRelease(XEvent ev);
void saveDoubleClick(XEvent ev);
void saveKeyPress(XEvent ev);

void saveTablaClick(int fila);
void saveTablaDoubleClick(int fila);
void saveBtnGuardarClick();
void saveBtnCancelarClick();
void saveEtReturnPressed();
void saveEtKeyPress(unsigned int key_code);
void saveEtClick();

#endif // SAVE_H_INCLUDED
