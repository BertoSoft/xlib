#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "edit.h"
#include "ui.h"
#include "grafica.h"

#define MAX_ET 6

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

void initControls(){
    int i;;
    int x, y, ancho, alto;

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
    // Iniciamos los ets
    //
    x       = 500 + 400 + 70;
    y       = 35;
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
                            dat_edit.color,
                            blanco);

    XSelectInput(dpy, et[0].id, ButtonPressMask | KeyPressMask);

    sprintf(et[0].nombre, "et_grado");
    et[0].x         = x;
    et[0].y         = y;
    et[0].ancho     = ancho;
    et[0].alto      = alto;
    et[0].color     = dat_edit.color;
    et[0].xfs       = XLoadQueryFont(dpy, FONT_N_B);
    et[0].gc        = XCreateGC(dpy, et[0].id, 0, 0);

    //
    // Coef.
    //
    x       = 500 + 210 + 70;
    y       = 35;
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
                                dat_edit.color,
                                blanco);

        XSelectInput(dpy, et[1].id, ButtonPressMask | KeyPressMask);

        sprintf(et[i].nombre, "et_x4");
        et[i].x         = x;
        et[i].y         = y + (int)(i * 60);
        et[i].ancho     = ancho;
        et[i].alto      = alto;
        et[i].color     = dat_edit.color;
        et[i].xfs       = XLoadQueryFont(dpy, FONT_N_B);
        et[i].gc        = XCreateGC(dpy, et[i].id, 0, 0);

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

void showEdit(){

    initEdit();
    initControls();
    chk[0].is_cheked = True;
    pintaEdit();
}

void pintaEdit(){

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
    int             x, y, ancho, alto;

    //
    // Pintamos los cuadros
    //
    x       = 500;
    y       = 13;
    ancho   = 600;
    alto    = 375;
    XSetLineAttributes( dpy, gc_edit, 1, LineSolid, CapRound, JoinMiter);
    XSetForeground(dpy, gc_edit, dat_edit.color);
    XDrawRectangle(dpy, w_edit, gc_edit, x, y, ancho, alto);
    XDrawRectangle(dpy, w_edit, gc_edit, x + 2, y + 2, ancho - 4, alto - 4);

    //
    // Segun chk pintamos  datos
    //
    if(chk[0].is_cheked){pintaDatosPolinomicas();}

}

void pintaDatosPolinomicas(){
    int             i;
    int             x, y;
    char            msg[1024];
    XFontStruct     *xfs;

    x       = 525;
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

    sprintf(msg,"Coef. X(exp)4 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 220, 150, 25);
    setEditText(et[3]);

    sprintf(msg,"Coef. X(exp)4 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 280, 150, 25);
    setEditText(et[4]);

    sprintf(msg,"Coef. X(exp)4 ");
    setTexto(w_edit, gc_edit, msg, xfs, dat_edit.color, x + 100, y + 340, 150, 25);
    setEditText(et[5]);
}

void editClick(XEvent ev){
    int i;

    //
    // chks
    //
    i = 0;
    while(i < 4){
        if(ev.xfocus.window == chk[i].id){
            chk[i].is_cheked = True;
        }
        else{
            chk[i].is_cheked = False;
        }
        i++;
    }

    pintaEdit();
}
