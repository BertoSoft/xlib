#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "ui.h"
#include "grafica.h"


void initGrafica(){

    w_grafica = XCreateSimpleWindow(dpy,
                            win,
                            0,
                            0,
                            dat_win.ancho,
                            dat_win.alto,
                            0,
                            blanco,
                            negro);

    dat_grafica.id          = w_grafica;
    dat_grafica.x           = 0;
    dat_grafica.y           = 0;
    dat_grafica.ancho       = dat_win.ancho;
    dat_grafica.alto        = dat_win.alto;
    dat_grafica.borde       = 0;
    dat_grafica.color       = blanco;
    dat_grafica.back_color  = negro;


    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, w_grafica, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos el gc
    //
    gc_grafica = XCreateGC(dpy, w_grafica, 0, 0);

    //
    // Mapeamos la pantalla
    //
    XMapRaised(dpy, w_grafica);
}

void closeGrafica(){

    XFreeGC(dpy, gc_grafica);
    XUnmapWindow(dpy, w_grafica);
    XDestroyWindow(dpy,w_grafica);
}

void showGrafica(){

    initGrafica();
    pintaGrafica();
}

void pintaGrafica(){

    //
    // Redimensionamos la ventana, por si cambio de dimensiones
    //
    XMoveResizeWindow(dpy, w_grafica, dat_win.x, dat_win.y, dat_win.ancho, dat_win.alto);

    dat_grafica.ancho   = dat_win.ancho;
    dat_grafica.alto    = dat_win.alto;

    pintaEjes();

}

void pintaEjes(){
    int         ip, iff;
    int         x0, y0;
    int         intervalof;
    int         xpmin, xpmax, ypmin, intervalop;
    char        msg[1024];
    XFontStruct *xfs;

    xfs = XLoadQueryFont(dpy, FONT_N);

    x0 = (int) (dat_win.ancho/2);
    y0 = (int) (dat_win.alto/2);

    xpmin   = 0;
    xpmax   = dat_win.ancho;
    ypmin   = dat_win.alto;


    XSetForeground(dpy, gc_grafica, dat_grafica.color);

    XDrawLine(dpy, w_grafica, gc_grafica, x0, 0, x0, y0 * 2);   // Eje y
    XDrawLine(dpy, w_grafica, gc_grafica, 0, y0, x0 * 2, y0);   // Eje x

    ip          = x0;
    iff         = 0;
    intervalof  = (int) ((0 + 10) / 5);
    intervalop  = (int) ((x0 - 0) / 5);

    //
    // Desde 0 hasta xmin
    //
    while(ip > xpmin){
        if(iff == 0){
            msg[0] = '\0';
        }
        else{
            sprintf(msg, "%d", iff);
        }
        XDrawLine(dpy, w_grafica, gc_grafica, ip, y0 - 10, ip, y0 + 10);
        setTexto(w_grafica, gc_grafica, msg, xfs, dat_grafica.color, ip -5, y0 + 25, 100, 25);
        iff -= intervalof;
        ip  -= intervalop;
    }

    ip          = x0;
    iff         = 0;
    intervalof  = (int) ((10 - 0) / 5);
    intervalop  = (int) ((xpmax - x0) / 5);

    //
    // Desde 0 hasta xmax
    //
    while(ip < xpmax){
        if(iff == 0){
            msg[0] = '\0';
        }
        else{
            sprintf(msg, "%d", iff);
        }
        XDrawLine(dpy, w_grafica, gc_grafica, ip, y0 - 10, ip, y0 + 10);
        setTexto(w_grafica, gc_grafica, msg, xfs, dat_grafica.color, ip -5, y0 + 25, 100, 25);
        iff += intervalof;
        ip  += intervalop;
    }

    ip          = y0;
    iff         = 0;
    intervalof  = (int) ((0 + 100) / 5);
    intervalop  = (int) ((ypmin - y0) / 5);

    //
    // Desde 0 hasta ymin
    //
    while(ip < ypmin){
        if(iff == 0){
            msg[0] = '\0';
        }
        else{
            sprintf(msg, "%d", iff);
        }
        XDrawLine(dpy, w_grafica, gc_grafica, x0 - 10, ip, x0 + 10, ip);
        setTexto(w_grafica, gc_grafica, msg, xfs, dat_grafica.color, x0 + 20, ip + 5, 100, 25);
        iff -= intervalof;
        ip  += intervalop;
    }

    ip          = y0;
    iff         = 0;
    intervalof  = (int) ((100 - 0) / 5);
    intervalop  = (int) ((y0 - 0) / 5);

    //
    // Desde 0 hasta ymax
    //
    while(ip > 0){
        if(iff == 0){
            msg[0] = '\0';
        }
        else{
            sprintf(msg, "%d", iff);
        }
        XDrawLine(dpy, w_grafica, gc_grafica, x0 - 10, ip, x0 + 10, ip);
        setTexto(w_grafica, gc_grafica, msg, xfs, dat_grafica.color, x0 + 20, ip + 5, 100, 25);
        iff += intervalof;
        ip  -= intervalop;
    }


}

void pintaFuncion(){

}
