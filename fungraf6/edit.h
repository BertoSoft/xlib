#ifndef EDIT_H_INCLUDED
#define EDIT_H_INCLUDED

#include "ui.h"

Window          w_edit;
GC              gc_edit;
Datos           dat_edit;

//
// Declaramos los datos de los controles
//
Datos   chk[4];
Datos   et[6];

void initEdit();
void initControls();
void closeEdit();
void showEdit();
void pintaEdit();
void pintaTipoFuncion();
void pintaDatosFuncion();
void pintaDatosPolinomicas();
void editClick(XEvent ev);


#endif // EDIT_H_INCLUDED
