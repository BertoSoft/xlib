#ifndef EDIT_H_INCLUDED
#define EDIT_H_INCLUDED

#include "ui.h"

Window          w_edit;
GC              gc_edit;
Datos           dat_edit;
int             chk_opt;
int             chk_old;

void initEdit();
void initChks();
void initControlsPolinomicos();
void closeEdit();
void closeControlsPolinomicos();
void showEdit();
void setFocusEt(int opt);
void setEtEnabled(int activo);
void pintaEdit();
void pintaTipoFuncion();
void pintaDatosFuncion();
void pintaDatosPolinomicas();
void pintaDatosExponenciales();
void pintaDatosLogaritmicos();
void pintaDatosTrigonometricos();
void setChkCheked(Window id);
void editClick(XEvent ev);
void editKeyPress(XEvent ev);
void etKeyReturnPressed(Datos et);

#endif // EDIT_H_INCLUDED
