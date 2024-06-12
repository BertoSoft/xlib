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
    pintaEjes();

}

void pintaEjes(){
    double  xf_max, xf_min, yf_max, yf_min;
    int     xp_max, xp_min, yp_max, yp_min;
    int     xp, yp, i;
    double  it;
    double  pte_x, pte_y;
    int     cte_x, cte_y;
    char    valor[1024];
    int     iscero;

    //
    // Establecemos el color y los atributos de la linea
    //
    XSetForeground(dpy, gc_grafica, dat_grafica.color);
    XSetLineAttributes( dpy, gc_grafica, 1, LineSolid, CapRound, JoinMiter);

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
    if(xf_max > xf_min && epsilon > 0){

        //
        // Calculamos ptes y ctes
        //
        pte_x = (xp_max - xp_min) / (xf_max - xf_min);
        pte_y = (yp_max - yp_min) / (yf_min - yf_max);

        cte_x = xp_min - (pte_x * xf_min);
        cte_y = yp_min - (pte_y * yf_max);

        //
        // Eje X
        //
        if(cte_y >= yp_min && cte_y <= yp_max){

            //
            // Dibujamos el eje x
            //
            XDrawLine(dpy, w_grafica, gc_grafica, xp_min, cte_y, xp_max, cte_y);

            it  = xf_min;
            i   = 0;
            while(it <= (xf_max +epsilon)){

                //
                // Obtenemos el valor de xf con 4 decimales
                //
                sprintf(valor, "%0.4f", it);
                xp = (it * pte_x) + cte_x;

                //
                // Comprobamos si valor == 0.0000
                //
                if(strcmp(valor, "-0.0000") == 0 || strcmp(valor, "0.0000") == 0){
                    iscero = True;
                }
                else{
                    iscero = False;
                }

                //
                // Si no es cero pintamos el valor y la raya, grande si es par y pequeña si es impar
                //
                if(!iscero && i%2 == 0){
                    XDrawLine(dpy, w_grafica, gc_grafica, xp, cte_y -10, xp, cte_y +10);
                    XDrawString(dpy, w_grafica, gc_grafica, xp -20, cte_y +20, valor, strlen(valor));
                }
                else if(!iscero){
                    XDrawLine(dpy, w_grafica, gc_grafica, xp, cte_y -5, xp, cte_y +5);
                }

                //
                // Sumamos un nuevo intervalo a xf y a i
                //
                it += (xf_max -xf_min) / 20;
                i++;
            }
        }


        //
        // Eje Y
        //
        if(cte_x >= xp_min && cte_x <= xp_max){

            //
            // Dibujamos eje y
            //
            XDrawLine(dpy, w_grafica, gc_grafica, cte_x, yp_max, cte_x, yp_min);

            it  = yf_min;
            i   = 0;
            while(it <= (yf_max + epsilon)){

                //
                // Obtenemos el valor de Fx(xf), con 4 decimales
                //
                sprintf(valor, "%0.4f", it);
                yp = (it * pte_y) + cte_y;

                //
                // Miramos si es 0.0000
                //
                if(strcmp(valor, "-0.0000") == 0 || strcmp(valor, "0.0000") == 0){
                    iscero = True;
                }
                else{
                    iscero = False;
                }

                //
                // Si no es cero, pintamos raya grande si es par y pequeña si es impar
                //
                if(!iscero && i%2 == 0){
                    XDrawLine(dpy, w_grafica, gc_grafica, cte_x - 10, yp, cte_x + 10, yp);
                    XDrawString(dpy, w_grafica, gc_grafica, cte_x +20, yp +5, valor, strlen(valor));
                }
                else if(!iscero){
                    XDrawLine(dpy, w_grafica, gc_grafica, cte_x - 5, yp, cte_x + 5, yp);
                }

                //
                // Sumamos nuevo intervalo a Fx(xf)
                //
                it += (yf_max - yf_min) / 20;
                i++;
            }
        }
    }

}

void pintaFuncion(){
    double  xf_max, xf_min, yf_max, yf_min;
    int     xp_max, xp_min, yp_max, yp_min;
    int     xp, yp;
    double  it;
    double  pte_x, pte_y;
    int     cte_x, cte_y;

    //
    // Establecemos el color y los atributos de la linea
    //
    XSetForeground(dpy, gc_grafica, dat_grafica.color);
    XSetLineAttributes( dpy, gc_grafica, 1, LineSolid, CapRound, JoinMiter);


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
