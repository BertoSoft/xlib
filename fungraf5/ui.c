#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn

#include "ui.h"

#define BTN_LANZADERA       100
#define MAX_OPT_LANZADERA   6
#define MAX_TXT             6

//
// Aqui declaro todos los controles que aceptan eventos
//
Datos   chk[4];
Datos   txt[MAX_TXT];


void initUi(){

    //
    // Iniciamos las ventanas
    //
    initWindows();

    //
    // Establecemos las ventanas
    //
    XSetStandardProperties(dpy, win_scr, TITULO, TITULO, None, NULL, 0, NULL);

    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, win_scr, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask );
    XSelectInput(dpy, win_lanzadera, ExposureMask | ButtonPressMask | KeyPressMask);
    XSelectInput(dpy, win_graf, ExposureMask | ButtonPressMask | KeyPressMask);
    XSelectInput(dpy, win_edit, ExposureMask | ButtonPressMask | KeyPressMask);
    XSelectInput(dpy, win_abrir, ExposureMask | ButtonPressMask | KeyPressMask);
    XSelectInput(dpy, win_math, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos los gcs
    //
    gc_scr          = XCreateGC(dpy, win_scr, 0, 0);
    gc_lanzadera    = XCreateGC(dpy, win_lanzadera, 0, 0);
    gc_graf         = XCreateGC(dpy, win_graf, 0, 0);
    gc_edit         = XCreateGC(dpy, win_edit, 0, 0);
    gc_abrir        = XCreateGC(dpy, win_abrir, 0, 0);
    gc_math         = XCreateGC(dpy, win_math, 0, 0);

    //
    // Establecemos el valor minimo de win_scr
    //
    XSizeHints *tam_minimo = XAllocSizeHints();

    tam_minimo->flags        = PMinSize;
    tam_minimo->min_width    = ANCHO_MINIMO;
    tam_minimo->min_height   = ALTO_MINIMO;
    XSetWMNormalHints(dpy, win_scr, tam_minimo);

    XFree(tam_minimo);

    //
    // Borramos la pantalla
    //
    XClearWindow(dpy, win_scr);

    //
    // Mapeamos las pantallas
    //
    XMapRaised(dpy, win_scr);
    XMapRaised(dpy, win_edit);
    XMapRaised(dpy, win_lanzadera);

}

void initWindows(){

    //
    // iniciamos las variables
    //
    dpy             = XOpenDisplay((char *)0);
    scr             = DefaultScreen(dpy);
    blanco          = WhitePixel(dpy, scr);
    negro           = BlackPixel(dpy, scr);
    ancho_pantalla  = DisplayWidth(dpy, scr);
    alto_pantalla   = DisplayHeight(dpy, scr);

    //
    //Iniciamos los colores
    //
    azul        = colorPorNombre(dpy, "blue");
    purpura     = colorPorNombre(dpy, "medium purple");
    azure       = colorPorNombre(dpy, "azure");
    rojo        = colorPorNombre(dpy, "red");
    amarillo    = colorPorNombre(dpy, "yellow");
    gris        = colorPorNombre(dpy, "gray");
    gris_oscuro = colorPorNombre(dpy, "DarkSlateGray");
    gris_claro  = colorPorNombre(dpy, "LightGray");

    //
    // creamos la ventana principal
    //
    win_scr = XCreateSimpleWindow(dpy,
                                DefaultRootWindow(dpy),
                                0,
                                0,
                                ancho_pantalla,
                                alto_pantalla,
                                0,
                                blanco,
                                gris_oscuro);

    dat_scr.id          = win_scr;
    dat_scr.x           = 0;
    dat_scr.y           = 0;
    dat_scr.ancho       = ancho_pantalla;
    dat_scr.alto        = alto_pantalla;
    dat_scr.borde       = 0;
    dat_scr.color       = blanco;
    dat_scr.back_color  = gris_oscuro;

    //
    // creamos la ventana lanzadera
    //
    win_lanzadera = XCreateSimpleWindow(dpy,
                                        win_scr,
                                        ancho_pantalla - 90 -100,
                                        0,
                                        100,
                                        alto_pantalla -67,
                                        0,
                                        negro,
                                        purpura);

    dat_lanzadera.id            = win_lanzadera;
    dat_lanzadera.x             = ancho_pantalla -90 -100;
    dat_lanzadera.y             = 0;
    dat_lanzadera.ancho         = 100;
    dat_lanzadera.borde         = 0;
    dat_lanzadera.alto          = alto_pantalla -67;
    dat_lanzadera.color         = negro;
    dat_lanzadera.back_color    = purpura;

    //
    // creamos la ventana grafica
    //
    win_graf = XCreateSimpleWindow(dpy,
                            win_scr,
                            0,
                            0,
                            ancho_pantalla -90 -100,
                            alto_pantalla - 67,
                            0,
                            negro,
                            rojo);

    //
    // Establecemos las propiedades de win_graf
    //
    dat_win_graf.id          = win_graf;
    dat_win_graf.x           = 0;
    dat_win_graf.y           = 0;
    dat_win_graf.ancho       = ancho_pantalla -90 -100;
    dat_win_graf.alto        = alto_pantalla -67;
    dat_win_graf.borde       = 0;
    dat_win_graf.color       = negro;
    dat_win_graf.back_color  = rojo;

    //
    // creamos la ventana edit
    //
    win_edit = XCreateSimpleWindow(dpy,
                                win_scr,
                                0,
                                0,
                                ancho_pantalla -90 -100,
                                alto_pantalla - 67,
                                0,
                                negro,
                                gris);

    //
    // Establecemos las propiedades de win_edit
    //
    dat_win_edit.id          = win_edit;
    dat_win_edit.x           = 0;
    dat_win_edit.y           = 0;
    dat_win_edit.ancho       = ancho_pantalla -90 -100;
    dat_win_edit.alto        = alto_pantalla -67;
    dat_win_edit.borde       = 0;
    dat_win_edit.color       = negro;
    dat_win_edit.back_color  = gris;


    //
    // creamos la ventana Abrir
    //
    win_abrir = XCreateSimpleWindow(dpy,
                                win_scr,
                                0,
                                0,
                                ancho_pantalla -90 -100,
                                alto_pantalla - 67,
                                0,
                                negro,
                                azul);

    //
    // Establecemos las propiedades de win_abrir
    //
    dat_win_abrir.id          = win_abrir;
    dat_win_abrir.x           = 0;
    dat_win_abrir.y           = 0;
    dat_win_abrir.ancho       = ancho_pantalla -90 -100;
    dat_win_abrir.alto        = alto_pantalla -67;
    dat_win_abrir.borde       = 0;
    dat_win_abrir.color       = negro;
    dat_win_abrir.back_color  = azul;

    //
    // creamos la ventana Guardar
    //
    win_guardar = XCreateSimpleWindow(dpy,
                                win_scr,
                                0,
                                0,
                                ancho_pantalla -90 -100,
                                alto_pantalla - 67,
                                0,
                                negro,
                                amarillo);

    //
    // Establecemos las propiedades de win_abrir
    //
    dat_win_guardar.id          = win_guardar;
    dat_win_guardar.x           = 0;
    dat_win_guardar.y           = 0;
    dat_win_guardar.ancho       = ancho_pantalla -90 -100;
    dat_win_guardar.alto        = alto_pantalla -67;
    dat_win_guardar.borde       = 0;
    dat_win_guardar.color       = negro;
    dat_win_guardar.back_color  = amarillo;

    //
    // creamos la ventana math
    //
    win_math = XCreateSimpleWindow(dpy,
                                win_scr,
                                0,
                                0,
                                ancho_pantalla -90 -100,
                                alto_pantalla - 67,
                                0,
                                negro,
                                amarillo);

    //
    // Establecemos las propiedades de win_math
    //
    dat_win_math.id          = win_math;
    dat_win_math.x           = 0;
    dat_win_math.y           = 0;
    dat_win_math.ancho       = ancho_pantalla -90 -100;
    dat_win_math.alto        = alto_pantalla -67;
    dat_win_math.borde       = 0;
    dat_win_math.color       = negro;
    dat_win_math.back_color  = amarillo;

}

void initControls(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);
    XFontStruct *xfs;
    int         x, y, ancho, alto;


    //
    //Iniciamos el txt_grado
    //
    xfs = XLoadQueryFont(dpy, FONT_G);

    sprintf(txt[0].nombre, "txt_grado");
    txt[0].x            = x0 + 100 +485 ;
    txt[0].y            = y0 -(y0 -150);
    txt[0].ancho        = 100;
    txt[0].alto         = 50;
    txt[0].color        = negro;
    txt[0].xfs          = xfs;
    txt[0].id           = win_edit;
    txt[0].gc           = gc_edit;
    txt[0].is_enabled   = True;

    //
    //Iniciamos el txt_x(Exp)4
    //
    x       = x0 +100 +100;
    y       = y0 -(y0 -250);
    ancho   = 100;
    alto    = 50;

    sprintf(txt[1].nombre, "txt_x4");
    txt[1].x            = x;
    txt[1].y            = y;
    txt[1].ancho        = ancho;
    txt[1].alto         = alto;
    txt[1].color        = negro;
    txt[1].xfs          = xfs;
    txt[1].id           = win_edit;
    txt[1].gc           = gc_edit;
    txt[1].is_enabled   = True;

    //
    //Iniciamos el txt_x(Exp)3
    //
    sprintf(txt[2].nombre, "txt_x3");
    txt[2].x            = x;
    txt[2].y            = y +75;
    txt[2].ancho        = ancho;
    txt[2].alto         = alto;
    txt[2].color        = negro;
    txt[2].xfs          = xfs;
    txt[2].id           = win_edit;
    txt[2].gc           = gc_edit;
    txt[2].is_enabled   = True;

    //
    //Iniciamos el txt_x(Exp)2
    //
    sprintf(txt[3].nombre, "txt_x2");
    txt[3].x            = x;
    txt[3].y            = y +150;
    txt[3].ancho        = ancho;
    txt[3].alto         = alto;
    txt[3].color        = negro;
    txt[3].xfs          = xfs;
    txt[3].id           = win_edit;
    txt[3].gc           = gc_edit;
    txt[3].is_enabled   = True;

    //
    //Iniciamos el txt_x(Exp)1
    //
    sprintf(txt[4].nombre, "txt_x1");
    txt[4].x            = x;
    txt[4].y            = y +225;
    txt[4].ancho        = ancho;
    txt[4].alto         = alto;
    txt[4].color        = negro;
    txt[4].xfs          = xfs;
    txt[4].id           = win_edit;
    txt[4].gc           = gc_edit;
    txt[4].is_enabled   = True;

    //
    //Iniciamos el txt_x(Exp)0
    //
    sprintf(txt[5].nombre, "txt_x0");
    txt[5].x            = x;
    txt[5].y            = y +300;
    txt[5].ancho        = ancho;
    txt[5].alto         = alto;
    txt[5].color        = negro;
    txt[5].xfs          = xfs;
    txt[5].id           = win_edit;
    txt[5].gc           = gc_edit;
    txt[5].is_enabled   = True;

    //
    // Colocamos como activo el foco en txt[0]
    //
    setFocus(txt[0].nombre);
}

void refreshUi(){
    int     i = 0;
    XImage  *img[MAX_OPT_LANZADERA];
    char    *iconos[] = {
                        "./imagenes/abrir96.jpeg",
                        "./imagenes/guardar96.jpeg",
                        "./imagenes/edit96.png",
                        "./imagenes/math96.png",
                        "./imagenes/grafica96.png",
                        "./imagenes/salir96.jpeg"
                        };

    //
    // Recolocamos todas las ventanas
    //
    XMoveResizeWindow(dpy, win_graf, dat_win_graf.x, dat_win_graf.y, dat_win_graf.ancho, dat_win_graf.alto);
    XMoveResizeWindow(dpy, win_edit, dat_win_edit.x, dat_win_edit.y, dat_win_edit.ancho, dat_win_edit.alto);
    XMoveResizeWindow(dpy, win_abrir, dat_win_abrir.x, dat_win_abrir.y, dat_win_abrir.ancho, dat_win_abrir.alto);
    XMoveResizeWindow(dpy, win_math, dat_win_math.x, dat_win_math.y, dat_win_math.ancho, dat_win_math.alto);

    XMoveResizeWindow(dpy, win_lanzadera, dat_lanzadera.x, dat_lanzadera.y, dat_lanzadera.ancho, dat_lanzadera.alto);

    //
    // refresh Lanzadera
    //
    i = 0;
    while(i < MAX_OPT_LANZADERA){
        //
        // Cargamos las imagenes
        //
        img[i] = loadImagen(dpy, win_lanzadera, iconos[i]);
        if(img[i]){
            XPutImage(dpy, win_lanzadera, gc_lanzadera, img[i], 0, 0, 4, 4 + (i * 100), 92, 92);
        }
        //
        // creamos los efectos boton presionado
        //
        if(i == opt_lanzadera){
            setClick(dpy, win_lanzadera, gc_lanzadera, 3, 3 + (i*BTN_LANZADERA), dat_lanzadera.ancho -4, BTN_LANZADERA -4);
        }
        else{
            setUnClick(dpy, win_lanzadera, gc_lanzadera, 3, 3 + (i*BTN_LANZADERA), dat_lanzadera.ancho -4, BTN_LANZADERA -4);
        }
        i++;
    }

    XMapRaised(dpy, win_scr);
    XFlush(dpy);
}

void refreshEdit(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XClearWindow(dpy, win_edit);

    //
    // Dibujo las pantalla funciones
    //
    pintaChkFunciones();

    //
    // Pintamos la pantalla de datos
    //
    switch(opt_funcion){
        case 0:
            setFocus(getOldFocus(win_edit));
            pintaPolinomicas();
            pintaEcuacion();
            break;
        case 1:
            setFocus("");
            pintaTrigonometricas();
            break;
        case 2:
            setFocus("");
            pintaExponenciales();
            break;
        case 3:
            setFocus("");
            pintaLogaritmicas();
            break;
    }

    //
    // Pintamos la ecuacion
    //

    //
    // Refrescamos el foco, con el ultimo control que tenia el foco
    //
    refreshFocus();

    //
    // Creo los cuadros
    //
    XSetForeground(dpy, gc_edit, negro);
    setUnClick(dpy, win_edit, gc_edit, 20, 20, x0 -40, y0 +160);
    setUnClick(dpy, win_edit, gc_edit, x0 +20, 20, x0 -40, y0 +160);
    setClick(dpy, win_edit, gc_edit, 20, y0 + 200, dat_win_edit.ancho - 40, y0 - 220);

    //
    // Limpiamos los bordes
    //
    XSetForeground(dpy, gc_edit, dat_win_edit.back_color);
    XFillRectangle(dpy, win_edit, gc_edit, 0, 0, 19, dat_win_edit.alto);
    XFillRectangle(dpy, win_edit, gc_edit, 0, 0, dat_win_edit.ancho, 19);
    XFillRectangle(dpy, win_edit, gc_edit, 0, dat_win_edit.alto - 16, dat_win_edit.ancho, 17);
    XFillRectangle(dpy, win_edit, gc_edit, dat_win_edit.ancho -19, 0, 19, dat_win_edit.alto);
    XFillRectangle(dpy, win_edit, gc_edit, 0, y0 + 182, dat_win_edit.ancho, 18);
    XFillRectangle(dpy, win_edit, gc_edit, x0 -19, 0, 38, y0 + 183);
}

void refreshFocus(){
    int i;

    //
    // comprobamos si el del foco es un txt
    //
    i = 0;
    while(i<MAX_TXT){
        if(strcmp(focus, txt[i].nombre) == 0){
            //
            // Tenemos que dibujar una barra al final
            //
            setCursor(txt[i]);
        }
        i++;
    }
}

void pintaChkFunciones(){
    XFontStruct *xfs;
    char        msg[1024];
    int         h = 150;
    int         w = 200;
    int         i;

    //
    // Pintamos el texto
    //
    xfs = XLoadQueryFont(dpy, FONT_G);
    XSetForeground(dpy, gc_edit, azul);

    sprintf(msg, "Elige el tipo de funcion...");
    setTexto(win_edit, gc_edit, msg, xfs, azul, w, h, 400, 50);

    xfs = XLoadQueryFont(dpy, FONT_N_B);

    sprintf(msg,"1.- Funcion polinomica.");
    setTexto(win_edit, gc_edit, msg, xfs, azul, w +50, h +100, 400, 50);
    sprintf(msg,"2.- Funcion trigonometrica.");
    setTexto(win_edit, gc_edit, msg, xfs, azul, w +50, h +150, 400, 50);
    sprintf(msg,"3.- Funcion exponencial.");
    setTexto(win_edit, gc_edit, msg, xfs, azul, w +50, h +200, 400, 50);
    sprintf(msg,"4.- Funcion logaritmica.");
    setTexto(win_edit, gc_edit, msg, xfs, azul, w +50, h +250, 400, 50);

    //
    // Pintamos lo chks
    //
    i = 0;
    while(i < 4){
        if(i == opt_funcion){
            setClick(dpy, win_edit, gc_edit, w, h +80 +(50 * i), 25, 25);
        }
        else{
            setUnClick(dpy, win_edit, gc_edit, w, h +80 +(50 * i), 25, 25);
        }
        chk[i].x        = w;
        chk[i].y        = h + 80 +(50 * i);
        chk[i].ancho    = 25;
        chk[i].alto     = 25;
        i++;
    }
}

void pintaPolinomicas(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);
    char        msg[1024];
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    int         grado;
    int         g4, g3, g2, g1, cte;

    //
    // Primero hago un borrado del cuadro
    //
    XSetForeground(dpy, gc_edit, dat_win_edit.back_color);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);

    //
    // Pedimos el grado de la funcion
    //
    xfs = XLoadQueryFont(dpy, FONT_G);

    XSetForeground(dpy, gc_edit, azul);
    sprintf(msg, "Indica el grado de la funcion ( Max. 4 )");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x0 + 100, y0 -(y0 - 150), 485, 50);

    //
    // dibujamos el txt[0], el txt_grado
    //
    setEditText(win_edit, gc_edit, xfs, txt[0]);

    //
    // colocamos los indices de las potencias
    //
    XSetForeground(dpy, gc_edit, azul);

    x       = x0 +100 +100 +150;
    y       = y0 -(y0 -250);
    ancho   = 100;
    alto    = 100;

    sprintf(msg, "X(Exp)4");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x, y, ancho, alto);
    setEditText(win_edit, gc_edit, xfs, txt[1]);

    sprintf(msg, "X(Exp)3");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x, y +75, ancho, alto);
    setEditText(win_edit, gc_edit, xfs, txt[2]);

    sprintf(msg, "X(Exp)2");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x, y +150, ancho, alto);
    setEditText(win_edit, gc_edit, xfs, txt[3]);

    sprintf(msg, "X");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x, y +225, ancho, alto);
    setEditText(win_edit, gc_edit, xfs, txt[4]);

    sprintf(msg, "Cte.");
    setTexto(win_edit, gc_edit, msg, xfs, azul, x, y +300, ancho, alto);
    setEditText(win_edit, gc_edit, xfs, txt[5]);

    //
    // Segun el valor de grado inhabilitamos algunos txt
    //
    sscanf(txt[0].msg, "%d", &grado);
    if(strcmp(txt[0].msg, "") ==  0){
        txt[1].is_enabled = False;
        txt[2].is_enabled = False;
        txt[3].is_enabled = False;
        txt[4].is_enabled = False;
        txt[5].is_enabled = False;
    }
    else if(strcmp(txt[0].msg, "0") == 0){  // estamos ante una cte
        txt[1].is_enabled = False;
        txt[2].is_enabled = False;
        txt[3].is_enabled = False;
        txt[4].is_enabled = False;
        txt[5].is_enabled = True;
    }
    else if(strcmp(txt[0].msg, "1") == 0){ // Ecuacion primer grado
        txt[1].is_enabled = False;
        txt[2].is_enabled = False;
        txt[3].is_enabled = False;
        txt[4].is_enabled = True;
        txt[5].is_enabled = True;
    }
    else if(strcmp(txt[0].msg, "2") == 0){ // Ec. segundo grado
        txt[1].is_enabled = False;
        txt[2].is_enabled = False;
        txt[3].is_enabled = True;
        txt[4].is_enabled = True;
        txt[5].is_enabled = True;
    }
    else if(strcmp(txt[0].msg, "3") == 0){  // ec. tercer grado
        txt[1].is_enabled = False;
        txt[2].is_enabled = True;
        txt[3].is_enabled = True;
        txt[4].is_enabled = True;
        txt[5].is_enabled = True;
    }
    else if(strcmp(txt[0].msg, "4") == 0){  // ec. cuarto grado
        txt[1].is_enabled = True;
        txt[2].is_enabled = True;
        txt[3].is_enabled = True;
        txt[4].is_enabled = True;
        txt[5].is_enabled = True;
    }
    else{
        txt[1].is_enabled = False;
        txt[2].is_enabled = False;
        txt[3].is_enabled = False;
        txt[4].is_enabled = False;
        txt[5].is_enabled = False;
    }

    //
    // Segun los valores pintamos la ecuacion
    //
    cte = g4 = g3 = g2 = g1 = 0;

    sscanf(txt[1].msg, "%d", &g4);
    sscanf(txt[2].msg, "%d", &g3);
    sscanf(txt[3].msg, "%d", &g2);
    sscanf(txt[4].msg, "%d", &g1);
    sscanf(txt[5].msg, "%d", &cte);

    polinomio[0] = cte;
    polinomio[1] = g1;
    polinomio[2] = g2;
    polinomio[3] = g3;
    polinomio[4] = g4;

}

char *polinomioToChar(){
    char msg[1024], msg1[1024], msg2[1024];
    char *ptr, *ptr1;
    int cte, g1, g2, g3 ,g4;

    msg[0] = '\0';

    //
    // Establecemos el grado del polinomio
    //

    cte = polinomio[0];
    g1  = polinomio[1];
    g2  = polinomio[2];
    g3  = polinomio[3];
    g4  = polinomio[4];

    //
    // empezamos con el grado 4
    //
    if(g4 == 0){ sprintf( msg1, "f(x) =");}
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

    ptr1 = strcat(msg1, msg2);
    strcpy(msg, ptr1);
    strcpy(msg1, msg);

    //
    // Grado 2
    //
    if(g2 == 0){sprintf(msg2, " ");}
    if(g2 < 0 && g2 != -1){sprintf(msg2,  " - %d*X(exp)2", abs(g2));}
    if(g2 > 0 && g2 !=1){sprintf(msg2, " + %d*X(exp)2", g2);}
    if(g2 == 1){sprintf(msg2, " + X(exp)2");}
    if(g2 == -1){sprintf(msg2, " - X(exp)2");}

    ptr1 = strcat(msg1, msg2);
    strcpy(msg, ptr1);
    strcpy(msg1, msg);

    //
    // Grado 1
    //
    if(g1 == 0){sprintf(msg2, " ");}
    if(g1 < 0 && g1 != -1){sprintf(msg2,  " - %d*X", abs(g1));}
    if(g1 > 0 && g1 !=1){sprintf(msg2, " + %d*X", g1);}
    if(g1 == 1){sprintf(msg2, " + X");}
    if(g1 == -1){sprintf(msg2, " - X");}

    ptr1 = strcat(msg1, msg2);
    strcpy(msg, ptr1);
    strcpy(msg1, msg);

    //
    // Cte
    //
    if(cte == 0){sprintf(msg2, " ");}
    if(cte > 0){sprintf(msg2, " + %d", cte);}
    if(cte < 0){sprintf(msg2, " - %d", abs(cte));}

    ptr1 = strcat(msg1, msg2);
    strcpy(msg, ptr1);
    strcpy(msg1, msg);

    ptr = &msg[0];

    return ptr;
}

void pintaTrigonometricas(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XSetForeground(dpy, gc_edit, rojo);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);

    setFocus("");
}

void pintaExponenciales(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XSetForeground(dpy, gc_edit, amarillo);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);
}

void pintaLogaritmicas(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XSetForeground(dpy, gc_edit, azul);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);
}

void pintaEcuacion(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);
    char        msg[1024];
    char        *ptr;
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    int         len, ancho_c;

    //
    //Primero hago un borrado del cuadro
    //
    XSetForeground(dpy, gc_edit, dat_win_edit.back_color);
    XFillRectangle(dpy, win_edit, gc_edit, 20, y0 + 200, dat_win_edit.ancho - 40, y0 - 220);

    //
    // Compruebo polinomio
    //
    if(opt_funcion == 0){
        ptr = polinomioToChar();
        strcpy(msg, ptr);
    }

    //
    // Si la longitud de ecuacion es mayor que cero, Presento la ecuacion centrada
    //
    if(strlen(msg) > 0){

        xfs = XLoadQueryFont(dpy, FONT_G);

        len     = strlen(msg);
        ancho   = XTextWidth(xfs, msg, strlen(msg));
        ancho_c = (int) (ancho / len);
        ancho   += (int) (2 * ancho_c);
        alto    = 50;
        x       = (int) x0 - (ancho / 2);
        y       = (int) y0 + (y0 / 2) + 110;

        setTexto(win_edit, gc_edit, msg, xfs, azul, x, y, ancho, alto);
    }
}

void closeUi(){

    XFreeGC(dpy, gc_lanzadera);
    XFreeGC(dpy, gc_abrir);
    XFreeGC(dpy, gc_edit);
    XFreeGC(dpy, gc_math);
    XFreeGC(dpy, gc_graf);
    XFreeGC(dpy, gc_scr);

    XUnmapWindow(dpy, win_lanzadera);
    XUnmapWindow(dpy, win_abrir);
    XUnmapWindow(dpy, win_edit);
    XUnmapWindow(dpy, win_math);
    XUnmapWindow(dpy, win_graf);
    XUnmapWindow(dpy, win_scr);

    XDestroyWindow(dpy,win_lanzadera);
    XDestroyWindow(dpy,win_abrir);
    XDestroyWindow(dpy,win_edit);
    XDestroyWindow(dpy,win_math);
    XDestroyWindow(dpy,win_graf);
    XDestroyWindow(dpy,win_scr);

    XCloseDisplay(dpy);
}

void resizeWin(XEvent ev){
    int x0;
    int y0;
    int x, y;
    int i;

    dat_scr.x       = ev.xconfigure.x;
    dat_scr.y       = ev.xconfigure.y;
    dat_scr.ancho   = ev.xconfigure.width;
    dat_scr.alto    = ev.xconfigure.height;

    dat_win_abrir.x       = 1;
    dat_win_abrir.y       = 1;
    dat_win_abrir.ancho   = dat_scr.ancho -107;
    dat_win_abrir.alto    = dat_scr.alto -4;

    dat_win_edit.x       = 1;
    dat_win_edit.y       = 1;
    dat_win_edit.ancho   = dat_scr.ancho -107;
    dat_win_edit.alto    = dat_scr.alto -4;

    dat_win_math.x       = 1;
    dat_win_math.y       = 1;
    dat_win_math.ancho   = dat_scr.ancho -107;
    dat_win_math.alto    = dat_scr.alto -4;

    dat_win_graf.x       = 1;
    dat_win_graf.y       = 1;
    dat_win_graf.ancho   = dat_scr.ancho -107;
    dat_win_graf.alto    = dat_scr.alto -4;

    dat_lanzadera.x       = dat_scr.ancho -103;
    dat_lanzadera.y       = 1;
    dat_lanzadera.ancho   = 100;
    dat_lanzadera.alto    = dat_scr.alto -4;

    //
    // Control txt_grado
    //
    x0 = (int)(dat_win_edit.ancho/2);
    y0 = (int)(dat_win_edit.alto/2);

    txt[0].x        = x0 + 100 +485 ;
    txt[0].y        = y0 -(y0 -150);

    //
    // Resto de controles
    //
    x = x0 +100 +100;
    y = y0 -(y0 -250);

    i = 1;
    while(i < MAX_TXT){
        txt[i].x = x;
        txt[i].y = y +((i -1) * 75);
        i++;
    }
}

void salir(){

    closeUi();
    exit(0);
}

unsigned long colorPorNombre( Display *dis, char *nombre ){
  XColor color, temp;

  XAllocNamedColor( dis,
                    DefaultColormap(dis,DefaultScreen(dis)),
                    nombre,
                    &color,
                    &temp );

  return( color.pixel );
}

XImage *loadImagen(Display *display, Window w, char *ruta){
    Pixmap      pix;
    Imlib_Image imlib_img;
    int         ancho, alto;
    Screen      *scr;
    XImage      *x_img;

    //
    // Leemos el archivo
    //
    imlib_img = imlib_load_image(ruta);

    //
    // Si el archivo no existe, devolvemos nulo
    //
    if(imlib_img == 0x0){
        x_img = NULL;
    }
    //
    // Si el archivo existe devolvemos *XImage
    //
    else{

    //
    // Establecemos los contextos
    //
    imlib_context_set_image(imlib_img);
    ancho   = imlib_image_get_width();
    alto    = imlib_image_get_height();

    //
    // Obtenemos la pantalla por defecto y creamos el pixmap vacío
    //
    scr = DefaultScreenOfDisplay(display);
    pix = XCreatePixmap(display, w, ancho, alto, DefaultDepthOfScreen(scr));

    //
    // Establecemos valores de imlib
    //
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisualOfScreen(scr));
    imlib_context_set_colormap(DefaultColormapOfScreen(scr));
    imlib_context_set_drawable(pix);

    //
    // Copiamos la imagen imlib en pixmap
    //
    imlib_render_image_on_drawable(0, 0);

    //
    // Pasamos el pixmap a XImage
    //
    x_img = XGetImage(display, pix, 0, 0, ancho, alto, 0xFFFFFFFF, ZPixmap);

    XFreePixmap(display, pix);
    }

    return x_img;
}

char *getFocus(){
    char nombre[1024];
    char *ptr;

    strcpy(nombre, focus);
    ptr = &nombre[0];

    return ptr;
}

char *getOldFocus(Window w){
    char nombre[1024];
    char *ptr;

    if(w == dat_win_edit.id){
        strcpy(nombre, old_focus_edit);
    }

    ptr = &nombre[0];

    return ptr;
}

void setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h2 y v2 claras
    //
    XSetForeground( d, gc, gris_claro);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

    //
    // La linea h1 y v1 oscuras
    //
    XSetForeground( d, gc, gris_oscuro);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1
}

void setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h1 y v1 claras
    //
    XSetForeground( d, gc, gris_claro);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1

    //
    // La linea h2 y v2 oscuras
    //
    XSetForeground( d, gc, gris_oscuro);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

}

void setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto){
    int tam_msg_pixels, tam_caracter, n_max_carcters;
    int tam;

    tam_msg_pixels  = XTextWidth(xfs, msg, strlen(msg));
    if(tam_msg_pixels > 0){
        tam_caracter    = (int) (tam_msg_pixels / strlen(msg));
        n_max_carcters  = (int) (ancho / tam_caracter);
    }
    else{
        tam_caracter    = 0;
        n_max_carcters  = 0;
    }


    if(strlen(msg) <= n_max_carcters){
        tam = strlen(msg);
    }
    else{
        tam = n_max_carcters;
    }

    XSetFont(dpy, gc, xfs->fid);
    XSetForeground(dpy, gc, color);
    XDrawString(dpy, w, gc, x0, y0, msg, tam);
}

void setEditText(Window w, GC gc, XFontStruct *xfs, Datos txt_texto){
    int     x   = txt_texto.x +5;
    int     y   = txt_texto.y -(int) (txt_texto.alto / 2) - 10;
    int     i, j;
    int     ancho_msg, max_caracters, ancho_caracter, len_msg;
    char    msg_visual[1024];

    //
    // Dibujamos el cuadro blanco
    //
    XSetForeground(dpy, gc, blanco);
    XFillRectangle(dpy, w, gc, x, y, txt_texto.ancho, txt_texto.alto);

    //
    // Obtenemos los max caracteres a mostrar, y le restamos 2
    //
    len_msg     = strlen(txt_texto.msg);
    ancho_msg   = XTextWidth(xfs, txt_texto.msg, len_msg);

    if(ancho_msg > 0){
        ancho_caracter  = (int) (ancho_msg / len_msg);
        max_caracters   = (int) (txt_texto.ancho / ancho_caracter);
    }
    else{
        ancho_caracter  = 0;
        max_caracters   = 0;
    }
    max_caracters -= 2;
    if(max_caracters < 0) max_caracters = 0;
    if(max_caracters > 1022) max_caracters = 1022;

    //
    // Ahora si el texto es muy grande, mostramos solo el final
    //
    if(len_msg > 0 && len_msg >= max_caracters){
        i = max_caracters -1;
        j = 0;
        msg_visual[max_caracters +1]   = '\0';
        while(j < max_caracters){
            msg_visual[i] = txt_texto.msg[len_msg -1 -j];
            i--;
            j++;
        }
    }
    else{
        strcpy(msg_visual, txt_texto.msg);
    }

    setClick(dpy, w, gc, x, y, txt_texto.ancho, txt_texto.alto);
    setTexto(w, gc, msg_visual, txt_texto.xfs, txt_texto.color, x +8, txt_texto.y, txt_texto.ancho, txt_texto.alto);
}

void setCursor(Datos txt){
    int     x, y;
    int     ancho_msg, ancho_caracter, max_caracters;

    ancho_msg           = XTextWidth(txt.xfs, txt.msg, strlen(txt.msg));
    if(ancho_msg > 0){
        ancho_caracter  = (int) (ancho_msg / strlen(txt.msg));
        max_caracters   = (int) (txt.ancho / ancho_caracter) - 1;
    }
    else{
        ancho_caracter = 0;
    }

    XSetLineAttributes( dpy, txt.gc, 3, LineSolid, CapRound, JoinMiter);
    XSetForeground(dpy, txt.gc, txt.color);

    if(strlen(txt.msg) > max_caracters -1){
        x = (int) txt.x + txt.ancho - (2 * ancho_caracter);
        y = txt.y -(int) (txt.alto / 2) - 10;
    }
    else{
        x = (strlen(txt.msg) * ancho_caracter) + txt.x + (int) (ancho_caracter/ 2);
        y = txt.y -(int) (txt.alto / 2) - 10;
    }

    XDrawLine(dpy, txt.id, txt.gc, x + 10, y +10, x +10, y + txt.alto -10);
}

void setFocus(char *nombre){
    int i;
    int focus_changed = False;

    //
    // Colocamos los valores is focused
    //
    i = 0;
    while(i<MAX_TXT){
        if(strcmp(nombre, txt[i].nombre) == 0 && txt[i].is_enabled == True){
            txt[i].is_focused   = True;
            focus_changed       = True;
        }
        else{
            txt[i].is_focused = False;
        }
        i++;
    }

    if(strlen(nombre) > 0 && focus_changed == True){
        strcpy(focus, nombre);
        strcpy(old_focus_edit, nombre);
    }
    else{
        sprintf(focus, "-1");
    }
}

void setOldFocus(Window w, char *nombre){

    if(w == dat_win_edit.id){
        strcpy(old_focus_edit, nombre);
    }
}

void winLanzaderaClick(XEvent ev){
    int i, j, opt;

    //
    // Obtenemos la opt pulsada
    //
    i       = 0;
    j       = 0;
    opt     = -1;
    while(i < MAX_OPT_LANZADERA){
        if(j < ev.xbutton.y && ev.xbutton.y < j +BTN_LANZADERA){
            opt = i;
            i   = MAX_OPT_LANZADERA;
        }
        j += BTN_LANZADERA;
        i++;
    }

    //
    // Si la pulsacion selecciono alguna opt ejecutamos la opt
    //
    if(opt >= 0){
        opt_lanzadera = opt;
        switch(opt){
            case 0:
                setFocus(dat_win_abrir.nombre);
                XMapRaised(dpy, win_abrir);
                break;
            case 1:
                setFocus(dat_win_guardar.nombre);
                XMapRaised(dpy, win_guardar);
                break;
            case 2:
                setFocus(getOldFocus(win_edit));
                XMapRaised(dpy, win_edit);
                break;
            case 3:
                setFocus(dat_win_math.nombre);
                XMapRaised(dpy, win_math);
                break;
            case 4:
                setFocus(dat_win_graf.nombre);
                XMapRaised(dpy, win_graf);
                break;
            case 5:
                salir();
                break;
        }
    }

    //
    // Refrescamos la pantalla
    //
    refreshUi();
}

void winLanzaderaKeyPress(XEvent ev){
    winEditKeyPress(ev);
}

void winEditClick(XEvent ev){
    int i, x, y, ancho, alto;

    //
    // Comprobamos las chk
    //
    x       = chk[0].x;
    ancho   = chk[0].ancho;

    if(ev.xbutton.x >= x && ev.xbutton.x <= (x + ancho)){
        i = 0;
        while(i<4){
            y       = chk[i].y;
            alto    = chk[i].alto;

            if(ev.xbutton.y >= y && ev.xbutton.y <= (y + alto)){
                opt_funcion = i;
                i = 4;
            }
            i++;
        }
    }

    //
    //Comprobamos los controles, si alguno se clicka, le damos el foco
    //
    i = 0;
    while(i < MAX_TXT){
        x       = txt[i].x + 6;
        y       = txt[i].y -(int) (txt[i].alto / 2) - 10;
        ancho   = txt[i].ancho;
        alto    =txt[i].alto;

        if(ev.xbutton.x >= x && ev.xbutton.x <= (x + ancho)){
            if(ev.xbutton.y >= y && ev.xbutton.y <= (y + alto)){
                setFocus(txt[i].nombre);
                i = MAX_TXT;
            }
        }
        i++;
    }


    //
    // Refrescamos la pantalla
    //
    refreshEdit();
}

void winEditKeyPress(XEvent ev){
    int     i, n_foco, len_msg;
    char    key;
    char    msg[1024];


    //
    // Obtenemos el  caracter
    //
    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);

    //
    // Comprobamos si hay algun txt con el foco
    //
    i       = 0;
    n_foco  = -1;
    while(i<MAX_TXT){
        if(strcmp(txt[i].nombre, focus) == 0 && txt[i].is_focused == True){
            n_foco = i;
            i = MAX_TXT;
        }
        i++;
    }

    //
    // Si es la tecla ESC salimos
    //
    if(ev.xkey.keycode == ESC){
        salir();
    }

    //
    // Si es BACK borramos la ultima letra
    //
    if(ev.xkey.keycode == BACK && n_foco >= 0){
        len_msg = strlen(txt[n_foco].msg);
        if(len_msg > 0){
            strcpy(msg, txt[n_foco].msg);
            msg[len_msg -1] = '\0';
            strcpy(txt[n_foco].msg, msg);
        }
    }
    //
    // Si es ENTER pasamos el foco al siguiente control activo, y si es el ultimo
    //
    else if(ev.xkey.keycode == ENTER || ev.xkey.keycode == RUN){
        if(n_foco >= 0 && n_foco < MAX_TXT -1 && txt[n_foco + 1].is_enabled == True){
            setFocus(txt[n_foco + 1].nombre);
        }
        else if(n_foco >= 0 && n_foco < MAX_TXT -2 && txt[n_foco + 2].is_enabled == True){
            setFocus(txt[n_foco + 2].nombre);
        }
        else if(n_foco >= 0 && n_foco < MAX_TXT -3 && txt[n_foco + 3].is_enabled == True){
            setFocus(txt[n_foco + 3].nombre);
        }
        else if(n_foco >= 0 && n_foco < MAX_TXT -4 && txt[n_foco + 4].is_enabled == True){
            setFocus(txt[n_foco + 4].nombre);
        }
        else if(n_foco >= 0 && n_foco < MAX_TXT -5 && txt[n_foco + 5].is_enabled == True){
            setFocus(txt[n_foco + 5].nombre);
        }
        else{
            setFocus(dat_win_edit.nombre);
            setOldFocus(win_edit, "");
        }
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
        if(strlen(txt[n_foco].msg) < 1023 && n_foco >= 0){
            txt[n_foco].msg[strlen(txt[n_foco].msg)]      = key;
            txt[n_foco].msg[strlen(txt[n_foco].msg) +1]   = '\0';
        }
    }

    //
    // Refrescamos la pantalla
    //
    refreshEdit();
}


