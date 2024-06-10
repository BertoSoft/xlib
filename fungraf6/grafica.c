#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    pintaFuncion();
    //pintaEjes();

}

void pintaEjes(){


}

void pintaFuncion(){
    double  xf_max, xf_min, yf_max, yf_min;
    int     xp_max, xp_min, yp_max, yp_min;
    int     xp, yp;
    double  it;
    double  pte_x, pte_y;
    int     cte_x, cte_y;
    char    valor[1024];

    //
    // Cargamos la fuente y establecemos el color
    //
    XSetForeground(dpy, gc_grafica, dat_grafica.color);

    //
    // Maximos y minimos de la pantalla
    //
    xp_max  = dat_grafica.ancho - 40;
    xp_min  = 40;
    yp_max  = dat_grafica.alto -40;
    yp_min  = 40;

    //
    // Maximos y minimos de la funcion
    //
    if(lim1 > lim0 && epsilon > 0){
        xf_max  = lim1;
        xf_min  = lim0;
        yf_max  = maxFx();
        yf_min  = minFx();
    }
    else{
        xf_max  = 0;
        xf_min  = 0;
        yf_max  = 0;
        yf_min  = 0;
    }

    //
    // Si hay Funcion la representamos
    //
    XSetLineAttributes( dpy, gc_grafica, 1, LineSolid, CapRound, JoinMiter);
    XDrawLine(dpy, w_grafica, gc_grafica, xp_min, yp_max, xp_max, yp_max);
    XDrawLine(dpy, w_grafica, gc_grafica, xp_min, yp_min, xp_min, yp_max);
    XDrawLine(dpy, w_grafica, gc_grafica, xp_max, yp_min, xp_max, yp_max);
    XDrawLine(dpy, w_grafica, gc_grafica, xp_min, yp_min, xp_max, yp_min);




    if(xf_max > xf_min && epsilon > 0){

        pte_x = (xp_max - xp_min) / (xf_max - xf_min);
        pte_y = (yp_max - yp_min) / (yf_min - yf_max);

        cte_x = xp_min - (pte_x * xf_min);
        cte_y = yp_min - (pte_y * yf_max);

        it = xf_min;
        while(it < xf_max + epsilon){

            xp = (it * pte_x) + cte_x;
            yp = (valFx(it) * pte_y) + cte_y;

            XDrawPoint(dpy, w_grafica, gc_grafica, xp , yp);

            it += epsilon;
        }












    it  = xf_min;
    while(it <= xf_max + epsilon){

        xp = (it * pte_x) + cte_x;

        sprintf(valor, "%f", it);
        XDrawString(dpy, w_grafica, gc_grafica, xp -20, yp_max +20, valor, strlen(valor));
        it += (xf_max - xf_min) / 7;
    }



    it  = yf_min;
    while(it <= yf_max){

        yp = (it * pte_y) + cte_y;

        sprintf(valor, "%f", it);
        XDrawString(dpy, w_grafica, gc_grafica, xp_min -20, yp, valor, strlen(valor));
        it += (yf_max - yf_min) / 7;
    }
















    }



}

double maxFx(){
    double maximo;
    double it, fx;

    maximo  = valFx(lim0);

    //
    // Recorremos desde lim0 hasta lim1, de epsilon en epsilon
    //
    it      = lim0;
    while(it <= lim1){
        fx = valFx(it);
        if(maximo < fx){
            maximo = fx;
        }
        it += epsilon;
    }

    //
    // Miramos el valor de lim1
    //
    fx = valFx(lim1);
    if(maximo < fx){
        maximo = fx;
    }

    return (double) maximo;
}

double minFx(){
    double minimo;
    double it, fx;

    minimo  = valFx(lim1);

    //
    // Recorremos desde lim0 hasta lim1, de epsilon en epsilon
    //
    it      = lim0;
    while(it <= lim1){
        fx = valFx(it);
        if(minimo > fx){
            minimo = fx;
        }
        it += epsilon;
    }

    //
    // Miramos el valor de lim1
    //
    fx = valFx(lim1);
    if(minimo > fx){
        minimo = fx;
    }

    return (double) minimo;
}

double valFx(double x){
    double fx;
    int cte, g1, g2, g3, g4;

    cte = polinomio[0];
    g1  = polinomio[1];
    g2  = polinomio[2];
    g3  = polinomio[3];
    g4  = polinomio[4];


    fx = ( (g4 * pow(x, 4) ) + ( g3 * pow(x, 3) ) + ( g2 * pow(x, 2) ) + ( g1 * x ) + ( cte ) );

    return fx;
}
