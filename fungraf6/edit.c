#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn

#include "edit.h"
#include "ui.h"
#include "grafica.h"

void initEdit(){

    //
    // Creamos la ventana
    //
    w_edit = XCreateSimpleWindow(dpy,
                                win,
                                0,
                                0,
                                dat_win.ancho,
                                dat_win.alto,
                                0,
                                azul,
                                gris_claro);

    //
    // Grabamos sus datos
    //
    dat_edit.id          = w_edit;
    dat_edit.x           = 0;
    dat_edit.y           = 0;
    dat_edit.ancho       = dat_win.ancho;
    dat_edit.alto        = dat_win.alto;
    dat_edit.borde       = 0;
    dat_edit.color       = azul;
    dat_edit.back_color  = gris_claro;

    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, w_edit, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos el gc
    //
    gc_edit     = XCreateGC(dpy, w_edit, 0, 0);
    dat_edit.gc = gc_edit;

    //
    // Mapeamos la pantalla
    //
    XMapRaised(dpy, w_edit);

}

void initChks(){
    int i;;
    int x, y;

    //
    // Iniciamos los chk
    //
    x = 75;
    y = 105;
    i = 0;
    while(i<4){
        chk[i].id           = i;
        chk[i].x            = x;
        chk[i].y            = y + (i * 50) - 2;
        chk[i].ancho        = 25;
        chk[i].alto         = 25;
        chk[i].is_cheked    = False;

        i++;
    }

    //
    // Iniciamos en opt == 0, como predeterminada, sino  el antiguo chk_old
    //
    if(chk_opt < 0){
        setChkCheked(chk_old);
        chk_opt = chk_old;

    }
    else{
        chk[0].is_cheked = True;
        chk_opt = 0;
        chk_old = 0;
    }

    //
    // Iniciamos los controles
    //
    initControlsPolinomicos();

    //
    // Mapeamos la pantalla
    //
    XMapSubwindows(dpy, w_edit);
    XFlush(dpy);
}

void initControlsPolinomicos(){
    int                         i;
    int                         x, y, ancho, alto;
    unsigned long color         = azul;
    unsigned long back_color    = blanco;


    //
    // Iniciamos los ets
    //
    x       = 500 + 400 + 50;
    y       = 30;
    ancho   = 50;
    alto    = 30;

    //
    // et[0] = et_grado
    //
    sprintf(et[0].nombre, "et_grado");
    et[0].id = 0;
    et[0].padre     = w_edit;
    et[0].x         = x;
    et[0].y         = y;
    et[0].ancho     = ancho;
    et[0].alto      = alto;
    et[0].color     = color;
    et[0].back_color= back_color;
    et[0].xfs       = XLoadQueryFont(dpy, FONT_N_B);
    et[0].gc        = gc_edit;

    //
    // Coef.
    //
    x       = 500 + 210 + 70;
    y       = 30;
    ancho   = 50;
    alto    = 30;
    i       = 1;

    while(i<6){

        et[i].id        = i;
        et[i].padre     = w_edit;
        et[i].x         = x;
        et[i].y         = y + (int)(i * 60);
        et[i].ancho     = ancho;
        et[i].alto      = alto;
        et[i].color     = color;
        et[i].back_color= back_color;
        et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
        et[i].gc        = gc_edit;

        i++;
    }

    sprintf(et[1].nombre, "et_grado_4");
    sprintf(et[2].nombre, "et_grado_3");
    sprintf(et[3].nombre, "et_grado_2");
    sprintf(et[4].nombre, "et_grado_1");
    sprintf(et[5].nombre, "et_coef");


    //
    // limites
    //
    x       = 1550;
    y       = 90;
    ancho   = 75;
    alto    = 30;
    i       = 6;

    while(i<8){
        et[i].id        = i;
        et[i].padre     = w_edit;
        et[i].x         = x;
        et[i].y         = y + (int)((i - 6) * 40);
        et[i].ancho     = ancho;
        et[i].alto      = alto;
        et[i].color     = color;
        et[i].back_color= back_color;
        et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
        et[i].gc        = gc_edit;

        i++;
    }
    sprintf(et[6].nombre, "lim_inf");
    sprintf(et[7].nombre, "lim_sup");


    //
    // Epsilon
    //
    x       = 1500;
    y       = 300;
    ancho   = 100;
    alto    = 30;

    sprintf(et[8].nombre, "epsilon");
    et[8].id        = 8;
    et[8].padre     = w_edit;
    et[8].x         = x;
    et[8].y         = y;
    et[8].ancho     = ancho;
    et[8].alto      = alto;
    et[8].color     = color;
    et[8].back_color= back_color;
    et[8].xfs       = XLoadQueryFont(dpy, FONT_N_B);
    et[8].gc        = gc_edit;

    //
    // Colocamos todos los ets como activos solo si chk_opt = 0;
    //
    if(chk_opt == 0){
        setEtEnabled(True);
    }
    else{
        setEtEnabled(False);
    }
}

void closeEdit(){

    //
    // Grabamos el old_chk
    //
    chk_old = chk_opt;
    chk_opt = -1;


    //
    // Desactivamos los controles
    //
    setEtEnabled(False);

    XFreeGC(dpy, gc_edit);
    XUnmapWindow(dpy, w_edit);
    XDestroyWindow(dpy, w_edit);
}

void showEdit(){

    initEdit();
    initChks();
    pintaEdit();
    setFocusEt(0);
}

void setFocusEt(int opt){
    int i;

    i = 0;
    while(i<MAX_ET){
        if(i == opt){
            et[i].is_focused = True;
        }
        else{
            et[i].is_focused = False;
        }
        i++;
    }
}

void setEtEnabled(int activo){
    int i;

    if(activo == True){
        i = 0;
        while(i < MAX_ET){
            et[i].is_enabled = True;
            i++;
        }
    }
    else{
        i = 0;
        while(i < MAX_ET){
            et[i].is_enabled = False;
            i++;
        }
    }
}

void pintaEdit(){

    //
    // Redimensionamos la ventana, por si cambio de dimensiones
    //
    XMoveResizeWindow(dpy, w_edit, dat_win.x, dat_win.y, dat_win.ancho, dat_win.alto);

    XClearWindow(dpy, w_edit);
    pintaTipoFuncion();
    pintaDatosFuncion();
}

void pintaTipoFuncion(){
    int             i;
    int             x, y;
    char            msg[1024];
    XFontStruct     *xfs;

    //
    // Pintamos los cuadros
    //
    XSetLineAttributes( dpy, gc_edit, 1, LineSolid, CapRound, JoinMiter);
    XSetForeground(dpy, gc_edit, dat_edit.color);
    XDrawRectangle(dpy, w_edit, gc_edit, 13, 13, 450, 375);
    XDrawRectangle(dpy, w_edit, gc_edit, 15, 15, 450 - 4, 375 - 4);

    x = 50;
    y = 50;

    //
    // El tipo de funcion
    //
    xfs = XLoadQueryFont(dpy, FONT_N_B);
    sprintf(msg, "Elige el tipo de Funcion ...");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x, y, 275, 25);

    //
    // Los tipos de funcion
    //
    x = 125;
    y = 125;

    sprintf(msg, "1.- Funcion polinomica.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x, y, 275, 25);
    sprintf(msg, "2.- Funcion Trigonometrica.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x, y + 50, 275, 25);
    sprintf(msg, "3.- Funcion Exponencial.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x, y + 100, 275, 25);
    sprintf(msg, "4.- Funcion Logaritmica.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x, y + 150, 275, 25);


    //
    // pintamos  los chk
    //
    i = 0;
    while(i<4){
        if(chk[i].is_cheked == True){
            setClick(dpy, w_edit, gc_edit, chk[i].x, chk[i].y, chk[i].ancho, chk[i].alto);
        }
        else{
            setUnClick(dpy, w_edit, gc_edit, chk[i].x, chk[i].y, chk[i].ancho, chk[i].alto);
        }
        i++;
    }
}

void pintaDatosFuncion(){

    //
    // Segun chk pintamos  datos
    //
    if(chk[0].is_cheked){

        //
        // Si chk_opt != 0 activamos los controles
        //
        if(chk_opt != 0){
            setEtEnabled(True);
        }

        //
        // Cambiamos el valor de chk_opt
        //
        chk_opt = 0;

        //
        // Pintamos
        //
        pintaDatosPolinomicas();
    }

    //
    // Trigonometricos
    //
    if(chk[1].is_cheked){

        //
        // Si chk_opt == 0, desactivamos sus controles
        //
        if(chk_opt == 0 || chk_opt < 0){
            setEtEnabled(False);
        }

        //
        // Colocamos el nuevo valor de chk_opt
        //
        chk_opt = 1;

        //
        // Pintamos
        //
        pintaDatosTrigonometricos();
    }

    //
    // Exponenciales
    //
    if(chk[2].is_cheked){

        //
        // Si chk_opt == 0, desactivamos sus controles
        //
        if(chk_opt == 0 || chk_opt < 0){
            setEtEnabled(False);
        }

        //
        // Colocamos el nuevo valor de chk_opt
        //
        chk_opt = 2;

        //
        // Pintamos
        //
        pintaDatosExponenciales();
    }

    //
    // Logaritmicas
    //
    if(chk[3].is_cheked){

        //
        // Si chk_opt == 0, desactivamos sus controles
        //
        if(chk_opt == 0 || chk_opt < 0){
            setEtEnabled(False);
        }

        //
        // Colocamos el nuevo valor de chk_opt
        //
        chk_opt = 3;

        //
        // Pintamos
        //
        pintaDatosLogaritmicos();
    }



}

void pintaDatosPolinomicas(){
    int             x, y, ancho, alto;
    char            msg[1024];
    XFontStruct     *xfs;

    //
    // Pintamos los cuadros
    //
    XSetLineAttributes( dpy, gc_edit, 1, LineSolid, CapRound, JoinMiter);
    XSetForeground(dpy, gc_edit, dat_edit.color);

    //
    // Recuadro coeficientes
    //
    x       = 525;
    y       = 13;
    ancho   = 550;
    alto    = 375;
    XDrawRectangle(dpy, w_edit, gc_edit, x, y, ancho, alto);
    XDrawRectangle(dpy, w_edit, gc_edit, x + 2, y + 2, ancho - 4, alto - 4);

    //
    // Recuadro limites
    //
    x       = 1125;
    y       = 13;
    ancho   = 550;
    alto    = 200;
    XDrawRectangle(dpy, w_edit, gc_edit, x, y, ancho, alto);
    XDrawRectangle(dpy, w_edit, gc_edit, x + 2, y + 2, ancho - 4, alto - 4);

    //
    // Recuadro Epsilon
    //
    x       = 1125;
    y       = 260;
    ancho   = 550;
    alto    = 125;
    XDrawRectangle(dpy, w_edit, gc_edit, x, y, ancho, alto);
    XDrawRectangle(dpy, w_edit, gc_edit, x + 2, y + 2, ancho - 4, alto - 4);

    //
    // Coeficientes
    //
    x       = 550;
    y       = 13;

    xfs = XLoadQueryFont(dpy, FONT_N_B);

    sprintf(msg, "Indica el grado de la funcion (Max 4)");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 50, y + 37, 350, 25);
    setEditText(et[0]);

    sprintf(msg,"Coef. X(exp)4 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 100, 150, 25);
    setEditText(et[1]);

    sprintf(msg,"Coef. X(exp)3 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 160, 150, 25);
    setEditText(et[2]);

    sprintf(msg,"Coef. X(exp)2 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 220, 150, 25);
    setEditText(et[3]);

    sprintf(msg,"Coef. X ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 280, 150, 25);
    setEditText(et[4]);

    sprintf(msg,"Cte. ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 340, 150, 25);
    setEditText(et[5]);

    //
    // Limites
    //
    x       = 1125;
    y       = 13;
    sprintf(msg, "Indica los limites de la funcion...");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 37, 400, 25);

    sprintf(msg, "Indica el limite Inferior de la funcion.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 50, y + 100, 400, 25);
    setEditText(et[6]);

    sprintf(msg, "Indica el limite superior de la funcion.");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 50, y + 140, 400, 25);
    setEditText(et[7]);

    //
    // Epsilon
    //
    x       = 1125;
    y       = 260;
    sprintf(msg, "Indica el Epsilon de la funcion:");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 50, y + 60, 400, 25);
    setEditText(et[8]);

    //
    // Funcion
    //
    pintaFuncionPolinomica(15, 450, dat_win.ancho - 65, 200);

    //
    // Limites
    //
    pintaLimites(15, 700, (int)(2 * (dat_win.ancho / 3)), 150);

    //
    // Epsilon
    //
    pintaEpsilon((int)(2 * (dat_win.ancho / 3)) + 50 , 700, (int)(2 * (dat_win.ancho / 3)) -50, 150);

}

void pintaFuncionPolinomica(int x, int y , int ancho, int alto){
    XFontStruct *xfs;
    char        msg[1024], msg1[1024], msg2[1024];
    int         g1, g2, g3, g4, cte;
    char        *ptr;
    int         x_t, y_t, ancho_t, alto_t, ancho_c, len;

    //
    //Primero hago un borrado del cuadro
    //
    XSetForeground(dpy, gc_edit, dat_edit.back_color);
    XFillRectangle(dpy, w_edit, gc_edit, x + 2, y +2, ancho -4, alto -4);

    //
    //Dibujamos los cuadros
    //
    XSetLineAttributes( dpy, gc_edit, 1, LineSolid, CapRound, JoinMiter);
    XSetForeground(dpy, gc_edit, dat_edit.color);
    XDrawRectangle(dpy, w_edit, gc_edit, x -2, y -2, ancho, alto +4);
    XDrawRectangle(dpy, w_edit, gc_edit, x, y, ancho -4, alto);

    //
    // Renovamos los datos del polinomio
    //
    cte = g4 = g3 = g2 = g1 = 0;

    sscanf(et[1].msg, "%d", &g4);
    sscanf(et[2].msg, "%d", &g3);
    sscanf(et[3].msg, "%d", &g2);
    sscanf(et[4].msg, "%d", &g1);
    sscanf(et[5].msg, "%d", &cte);

    polinomio[0] = cte;
    polinomio[1] = g1;
    polinomio[2] = g2;
    polinomio[3] = g3;
    polinomio[4] = g4;


    //
    // empezamos con el grado 4
    //
    if(g4 == 0){
            sprintf( msg1, "f(x) =");}
    if(g4 < 0 && g4 != -1){sprintf(msg1, "f(x) = - %d*X(exp)4", abs(g4));}
    if(g4 > 0 && g4 != 1){sprintf(msg1, "f(x) = %d*X(exp)4", g4);}
    if(g4 == 1){sprintf(msg1, "f(x) = X(exp)4");}
    if(g4 == -1){sprintf(msg1, "f(x) = - X(exp)4");}

    //
    // Grado 3
    //
    if(g3 == 0){sprintf(msg2, " ");}
    if(g3 < 0 && g3 != -1){sprintf(msg2,  " - %d*X(exp)3", abs(g3));}
    if(g3 > 0 && g3 !=1){sprintf(msg2, " +%d*X(exp)3", g3);}
    if(g3 == 1){sprintf(msg2, " + X(exp)3");}
    if(g3 == -1){sprintf(msg2, " - X(exp)3");}

    ptr = strcat(msg1, msg2);   // Unimos msg1 y msg2
    strcpy(msg, ptr);           // pasamos lo que hay en ptr a msg
    strcpy(msg1, msg);          // Copiamos msg en msg1

    //
    // Grado 2
    //
    if(g2 == 0){sprintf(msg2, " ");}
    if(g2 < 0 && g2 != -1){sprintf(msg2,  " - %d*X(exp)2", abs(g2));}
    if(g2 > 0 && g2 !=1){sprintf(msg2, " + %d*X(exp)2", g2);}
    if(g2 == 1){sprintf(msg2, " + X(exp)2");}
    if(g2 == -1){sprintf(msg2, " - X(exp)2");}

    ptr = strcat(msg1, msg2);
    strcpy(msg, ptr);
    strcpy(msg1, msg);

    //
    // Grado 1
    //
    if(g1 == 0){sprintf(msg2, " ");}
    if(g1 < 0 && g1 != -1){sprintf(msg2,  " - %d*X", abs(g1));}
    if(g1 > 0 && g1 !=1){sprintf(msg2, " + %d*X", g1);}
    if(g1 == 1){sprintf(msg2, " + X");}
    if(g1 == -1){sprintf(msg2, " - X");}

    ptr = strcat(msg1, msg2);
    strcpy(msg, ptr);
    strcpy(msg1, msg);

    //
    // Cte
    //
    if(cte == 0){sprintf(msg2, " ");}
    if(cte > 0){sprintf(msg2, " + %d", cte);}
    if(cte < 0){sprintf(msg2, " - %d", abs(cte));}

    ptr = strcat(msg1, msg2);
    strcpy(msg, ptr);


    //
    // Si la longitud de ecuacion es mayor que cero, Presento la ecuacion centrada
    //
    if(strlen(msg) > 0){

        xfs = XLoadQueryFont(dpy, FONT_G);

        len     = strlen(msg);
        ancho_t = XTextWidth(xfs, msg, strlen(msg));
        ancho_c = (int) (ancho_t / len);
        ancho_t += (int) (2 * ancho_c);
        alto_t  = 50;
        x_t     = (int) (ancho / 2) - (ancho_t / 2);
        y_t     = (int) y + (alto / 2);

        setTexto(w_edit, gc_edit, msg, xfs, azul, x_t, y_t, ancho_t, alto_t);
    }


    XFlush(dpy);

}

void pintaLimites(int x, int y, int ancho, int alto){
    char    msg[1024];
    int     l0, l1;

    //
    //Leemos los limites
    //
}

void pintaEpsilon(int x, int y, int ancho, int alto){

}

void pintaDatosTrigonometricos(){

}

void pintaDatosExponenciales(){

}

void pintaDatosLogaritmicos(){

}

void setChkCheked(Window id){
    int i;

    i = 0;
    while(i< MAX_CHK){
        if(chk[i].id == id){
            chk[i].is_cheked = True;
        }
        else{
            chk[i].is_cheked = False;
        }
        i++;
    }
}

void editClick(XEvent ev){
    int i;
    int x, y, x0, x1, y0, y1;

    //
    // si la pulsacion es en chk, ponemos el nuevo chk como pulsado
    //
    i = 0;
    x = ev.xbutton.x;
    y = ev.xbutton.y;

    while(i < MAX_CHK){

        x0 = chk[i].x;
        x1 = chk[i].x + chk[i].ancho;
        y0 = chk[i].y;
        y1 = chk[i].y + chk[i].alto;

        if(x0 < x && x1 > x && y0 < y && y1 > y){
            setChkCheked(i);
            i = MAX_ET;
        }
        i++;
    }

    //
    // Si la pulsacion es en un et, le damos el foco
    //
    i = 0;
    while(i < MAX_ET){

        x0 = et[i].x;
        x1 = et[i].x + et[i].ancho;
        y0 = et[i].y;
        y1 = et[i].y + et[i].alto;

        if(x0 < x && x1 > x && y0 < y && y1 > y && et[i].is_enabled == True){
            setFocusEt(i);
            i = MAX_ET;
        }

        i++;
    }

    //
    // Redibujamos
    //
    pintaEdit();
}

void editKeyPress(XEvent ev){
    int     i;
    char    key;
    char    msg[1024];
    int     et_focused;
    int     len_msg;

    //
    // Obtenemos el  caracter
    //
    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);

    //
    // Buscamos si hay algun et con el foco
    //
    et_focused  = -1;
    i           = 0;
    while(i < MAX_ET){
        if(et[i].is_focused == True){
            et_focused = i;
            i = MAX_ET;
        }
        i++;
    }

    //
    // Si es la tecla ESC y no hay msgBox Salimos
    //
    if(ev.xkey.keycode == ESC && dat_msg_box.is_enabled == False){
        salir();
    }
    //
    // Si es ESC y Hay MsgBox lo Cerramos
    //
    else if(ev.xkey.keycode == ESC && dat_msg_box.is_enabled == True){
        hideMsgBox();
    }

    //
    // Si es BACK borramos la ultima letra
    //
    if(ev.xkey.keycode == BACK && et_focused >= 0){
        len_msg = strlen(et[et_focused].msg);
        if(len_msg > 0){
            strcpy(msg, et[et_focused].msg);
            msg[len_msg -1] = '\0';
            strcpy(et[et_focused].msg, msg);
        }
    }

    //
    // Si es la tecla ENTER o RUN avisamos al metodo KeyReturnPressed.
    //
    else if( (ev.xkey.keycode == ENTER || ev.xkey.keycode == RUN) && et_focused >= 0){

        etKeyReturnPressed(et[et_focused]);
    }

    //
    // Si es mayuscula
    //
    else if(ev.xkey.keycode == KEY_MAY){

    }
    //
    // Cualquier otra pulsacion
    //
    else{
        //
        // Interpretamos el teclado numerico
        //
        if(ev.xkey.keycode == KEY_0){key = '0';}
        if(ev.xkey.keycode == KEY_1){key = '1';}
        if(ev.xkey.keycode == KEY_2){key = '2';}
        if(ev.xkey.keycode == KEY_3){key = '3';}
        if(ev.xkey.keycode == KEY_4){key = '4';}
        if(ev.xkey.keycode == KEY_5){key = '5';}
        if(ev.xkey.keycode == KEY_6){key = '6';}
        if(ev.xkey.keycode == KEY_7){key = '7';}
        if(ev.xkey.keycode == KEY_8){key = '8';}
        if(ev.xkey.keycode == KEY_9){key = '9';}
        if(ev.xkey.keycode == KEY_MUL){key = '*';}
        if(ev.xkey.keycode == KEY_DIV){key = '/';}
        if(ev.xkey.keycode == KEY_SUM){key = '+';}
        if(ev.xkey.keycode == KEY_RES){key = '-';}
        if(ev.xkey.keycode == KEY_PTO){key = '.';}

        //
        // Añadimos el caracter al msg
        //
        if(strlen(et[et_focused].msg) < 1023 && et_focused >= 0){
            et[et_focused].msg[strlen(et[et_focused].msg)]      = key;
            et[et_focused].msg[strlen(et[et_focused].msg) +1]   = '\0';
        }
    }

    //
    // Refrescamos la pantalla
    //
    pintaEdit();

}

void etKeyReturnPressed(Datos etKeyReturnPressed){


    //
    // KeyPress del et[0]
    //
    if(strlen(et[etKeyReturnPressed.id].msg) == 0 && etKeyReturnPressed.id == et[0].id){
        setFocusEt(-1);
        showMsgBox(dat_edit, etKeyReturnPressed, "Debes de indicar el grado de la funcion...");
    }
    else if(isNumerico(et[etKeyReturnPressed.id].msg) == False && etKeyReturnPressed.id == et[0].id){
        setFocusEt(-1);
        showMsgBox(dat_edit, etKeyReturnPressed, "Debes de indicar un Numero...");
    }
    else if(atoi(et[etKeyReturnPressed.id].msg) > 4 && etKeyReturnPressed.id == et[0].id){
        setFocusEt(-1);
        showMsgBox(dat_edit, etKeyReturnPressed, "El grado no puede ser mayor de 4...");
    }
    else if(etKeyReturnPressed.id == et[0].id){

        //
        //Activamos todos los controles
        //
        et[1].is_enabled = True;
        et[2].is_enabled = True;
        et[3].is_enabled = True;
        et[4].is_enabled = True;
        et[5].is_enabled = True;

        //
        // Borramos todos los ceficientes
        //
        et[1].msg[0] = '\0';
        et[2].msg[0] = '\0';
        et[3].msg[0] = '\0';
        et[4].msg[0] = '\0';
        et[5].msg[0] = '\0';

        //
        // valor 0
        //
        if(atoi(et[0].msg) == 0){
            sprintf(et[1].msg,"0");
            sprintf(et[2].msg,"0");
            sprintf(et[3].msg,"0");
            sprintf(et[4].msg,"0");

            et[1].is_enabled = False;
            et[2].is_enabled = False;
            et[3].is_enabled = False;
            et[4].is_enabled = False;

            setFocusEt(5);
        }

        //
        // valor 1
        //
        if(atoi(et[0].msg) == 1){
            sprintf(et[1].msg,"0");
            sprintf(et[2].msg,"0");
            sprintf(et[3].msg,"0");

            et[1].is_enabled = False;
            et[2].is_enabled = False;
            et[3].is_enabled = False;

            setFocusEt(4);
        }

        //
        // valor 2
        //
        if(atoi(et[0].msg) == 1){
            sprintf(et[1].msg,"0");
            sprintf(et[2].msg,"0");
            sprintf(et[3].msg,"0");

            et[1].is_enabled = False;
            et[2].is_enabled = False;
            et[3].is_enabled = False;

            setFocusEt(4);
        }

        //
        // valor 3
        //
        if(atoi(et[0].msg) == 3){
            sprintf(et[1].msg,"0");

            et[1].is_enabled = False;

            setFocusEt(2);
        }

        //
        // valor 4
        //
        if(atoi(et[0].msg) == 4){
            setFocusEt(1);
        }
    }

    //
    // KeyPress del et[1]
    //
    if(etKeyReturnPressed.id == et[1].id){
        setFocusEt(2);
    }

    //
    // KeyPress del et[2]
    //
    if(etKeyReturnPressed.id == et[2].id){
        setFocusEt(3);
    }

    //
    // KeyPress del et[3]
    //
    if(etKeyReturnPressed.id == et[3].id){
        setFocusEt(4);
    }

    //
    // KeyPress del et[4]
    //
    if(etKeyReturnPressed.id == et[4].id){
        setFocusEt(5);
    }

    //
    // KeyPress del et[5]
    //
    if(etKeyReturnPressed.id == et[5].id){
        setFocusEt(6);
    }

    //
    // KeyPress del et[6]
    //
    if(etKeyReturnPressed.id == et[6].id){
        setFocusEt(7);
    }

    //
    // KeyPress del et[7]
    //
    if(etKeyReturnPressed.id == et[7].id){
        setFocusEt(8);
    }

    //
    // KeyPress del et[8]
    //
    if(etKeyReturnPressed.id == et[8].id){
        setFocusEt(-1);
    }

}
