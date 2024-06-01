#ifndef EDIT_H_INCLUDED
#define EDIT_H_INCLUDED

#include "ui.h"

Window          w_edit;
GC              gc_edit;
Datos           dat_edit;

void initEdit();
void initChks();
void initControlsPolinomicos();
void closeEdit();
void closeControlsPolinomicos();
void showEdit();
void setFocusEt(int opt);
void pintaEdit();
void pintaTipoFuncion();
void pintaDatosFuncion();
void pintaDatosPolinomicas();
void pintaDatosTrigonometricos();
void editClick(XEvent ev);


#endif // EDIT_H_INCLUDED
