#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

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
    XSelectInput(dpy, w_edit, VisibilityChangeMask | ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos el gc
    //
    gc_edit = XCreateGC(dpy, w_edit, 0, 0);

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
        chk[i].id = XCreateSimpleWindow(dpy,
                            w_edit,
                            x,
                            y + (i * 50),
                            20,
                            20,
                            0,
                            dat_edit.color,
                            dat_edit.back_color);

        XSelectInput(dpy, chk[i].id, ButtonPressMask);

        chk[i].x            = x - 2;
        chk[i].y            = y + (i * 50) - 2;
        chk[i].ancho        = 24;
        chk[i].alto         = 24;
        chk[i].is_cheked    = False;

        i++;
    }

    //
    // Iniciamos en opt == 0, como predeterminada
    //
    chk[0].is_cheked = True;
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
    et[0].id = XCreateSimpleWindow(dpy,
                                w_edit,
                                x,
                                y ,
                                ancho,
                                alto,
                                0,
                                color,
                                blanco);

    XSelectInput(dpy, et[0].id, ButtonPressMask | KeyPressMask);

    sprintf(et[0].nombre, "et_grado");
    et[0].x         = x;
    et[0].y         = y;
    et[0].ancho     = ancho;
    et[0].alto      = alto;
    et[0].color     = color;
    et[0].back_color= back_color;
    et[0].xfs       = XLoadQueryFont(dpy, FONT_N_B);
    et[0].gc        = XCreateGC(dpy, et[0].id, 0, 0);

    //
    // Coef.
    //
    x       = 500 + 210 + 70;
    y       = 30;
    ancho   = 50;
    alto    = 30;
    i       = 1;

    while(i<6){

        et[i].id = XCreateSimpleWindow(dpy,
                                    w_edit,
                                    x,
                                    y + (int)(i * 60),
                                    ancho,
                                    alto,
                                    0,
                                    color,
                                    blanco);

        XSelectInput(dpy, et[i].id, ButtonPressMask | KeyPressMask);

        et[i].x         = x;
        et[i].y         = y + (int)(i * 60);
        et[i].ancho     = ancho;
        et[i].alto      = alto;
        et[i].color     = color;
        et[i].back_color= back_color;
        et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
        et[i].gc        = XCreateGC(dpy, et[i].id, 0, 0);

        i++;
    }

    //
    // limites
    //
    x       = 1550;
    y       = 90;
    ancho   = 75;
    alto    = 30;
    i       = 6;

    while(i<8){
        et[i].id = XCreateSimpleWindow(dpy,
                                    w_edit,
                                    x,
                                    y + (int)((i - 6) * 40),
                                    ancho,
                                    alto,
                                    0,
                                    color,
                                    blanco);

        XSelectInput(dpy, et[i].id, ButtonPressMask | KeyPressMask);

        et[i].x         = x;
        et[i].y         = y + (int)((i - 6) * 40);
        et[i].ancho     = ancho;
        et[i].alto      = alto;
        et[i].color     = color;
        et[i].back_color= back_color;
        et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
        et[i].gc        = XCreateGC(dpy, et[i].id, 0, 0);

        i++;
    }

    //
    // Epsilon
    //
    x       = 1500;
    y       = 300;
    ancho   = 100;
    alto    = 30;
    et[8].id = XCreateSimpleWindow(dpy,
                                w_edit,
                                x,
                                y,
                                ancho,
                                alto,
                                0,
                                color,
                                blanco);

    XSelectInput(dpy, et[8].id, ButtonPressMask | KeyPressMask);

    et[i].x         = x;
    et[i].y         = y;
    et[i].ancho     = ancho;
    et[i].alto      = alto;
    et[i].color     = color;
    et[i].back_color= back_color;
    et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
    et[i].gc        = XCreateGC(dpy, et[8].id, 0, 0);

    //
    // Colocamos todos los ets como activos
    //
    i = 0;
    while(i < MAX_ET){
        et[i].is_enabled = True;
        i++;
    }

    //
    // Mapeamos la pantalla
    //
    XMapSubwindows(dpy, w_edit);
    XFlush(dpy);

}

void closeEdit(){

    XFreeGC(dpy, gc_edit);
    XUnmapWindow(dpy, w_edit);
    XDestroyWindow(dpy, w_edit);
}

void closeControlsPolinomicos(){
    int i;

    i = 0;
    while(i<MAX_ET){

        XFreeGC(dpy, et[i].gc);
        XUnmapWindow(dpy, et[i].id);
        XDestroyWindow(dpy, et[i].id);
        et[i].is_enabled = False;
        i++;
    }
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

void pintaEdit(){

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
        pintaDatosPolinomicas();
    }

    //
    // Trigonometricos
    //
    if(chk[1].is_cheked){
        pintaDatosTrigonometricos();
    }

}

void pintaDatosPolinomicas(){
    int             x, y, ancho, alto;
    char            msg[1024];
    XFontStruct     *xfs;

    //
    // Iniciamos los controles
    //

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

}

void pintaDatosTrigonometricos(){


}

void editClick(XEvent ev){
    int i;

    //
    // Cerramos los controles del antiguo chk pulsado
    //
    if(chk[0].is_cheked == True){
            closeControlsPolinomicos();
    }


    //
    // ponemos el nuevo chk como pulsado
    //
    i = 0;
    while(i < MAX_CHK){
        if(ev.xfocus.window == chk[i].id){
            chk[i].is_cheked = True;
        }
        else{
            chk[i].is_cheked = False;
        }
        i++;
    }

    //
    // Iniciamos los controles del nuevo chk
    //
    if(chk[0].is_cheked == True){
            initControlsPolinomicos();
    }

    pintaEdit();
}
