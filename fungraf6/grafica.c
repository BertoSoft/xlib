#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "ui.h"
#include "grafica.h"


void initGrafica(){

    w = XCreateSimpleWindow(dpy,
                            win,
                            0,
                            0,
                            dat_win.ancho,
                            dat_win.alto,
                            0,
                            blanco,
                            negro);

    dat.id          = w;
    dat.x           = 0;
    dat.y           = 0;
    dat.ancho       = dat_win.ancho;
    dat.alto        = dat_win.alto;
    dat.borde       = 0;
    dat.color       = blanco;
    dat.back_color  = negro;


    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, w, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos el gc
    //
    gc = XCreateGC(dpy, w, 0, 0);

    //
    // Mapeamos la pantalla
    //
    XMapRaised(dpy, w);
}

void closeGrafica(){

    XFreeGC(dpy, gc);
    XUnmapWindow(dpy, w);
    XDestroyWindow(dpy,w);
}

void showGrafica(){

    initGrafica();
    pintaGrafica();
}

void pintaGrafica(){

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
    xpmax   = dat.ancho;
    ypmin   = dat.alto;


    XSetForeground(dpy, gc, dat.color);

    XDrawLine(dpy, w, gc, x0, 0, x0, y0 * 2);
    XDrawLine(dpy, w, gc, 0, y0, x0 * 2, y0);

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
        XDrawLine(dpy, w, gc, ip, y0 - 10, ip, y0 + 10);
        setTexto(w, gc, msg, xfs, dat.color, ip -5, y0 + 25, 100, 25);
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
        XDrawLine(dpy, w, gc, ip, y0 - 10, ip, y0 + 10);
        setTexto(w, gc, msg, xfs, dat.color, ip -5, y0 + 25, 100, 25);
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
        XDrawLine(dpy, w, gc, x0 - 10, ip, x0 + 10, ip);
        setTexto(w, gc, msg, xfs, dat.color, x0 + 20, ip + 5, 100, 25);
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
        XDrawLine(dpy, w, gc, x0 - 10, ip, x0 + 10, ip);
        setTexto(w, gc, msg, xfs, dat.color, x0 + 20, ip + 5, 100, 25);
        iff += intervalof;
        ip  -= intervalop;
    }


}

void pintaFuncion(){

}
