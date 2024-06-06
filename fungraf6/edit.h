#ifndef EDIT_H_INCLUDED
#define EDIT_H_INCLUDED

#include "ui.h"

Window          w_edit;
GC              gc_edit;
Datos           dat_edit;
int             chk_opt;
int             chk_old;
int             polinomio[5];

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
void pintaFuncionPolinomica(int x, int y, int ancho, int alto);
void pintaLimites(int x, int y, int ancho, int alto);
void pintaEpsilon(int x, int y, int ancho, int alto);
void pintaDatosExponenciales();
void pintaDatosLogaritmicos();
void pintaDatosTrigonometricos();
void setChkCheked(Window id);
void editClick(XEvent ev);
void editKeyPress(XEvent ev);
void etKeyReturnPressed(Datos et);

#endif // EDIT_H_INCLUDED
