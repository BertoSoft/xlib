#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "ui.h"

//
// Aqui declaro todos los controles que aceptan eventos
//
Datos   chk_funcion[4];


void initUi(){

    // iniciamos las variables
    dpy             = XOpenDisplay((char *)0);
    scr             = DefaultScreen(dpy);
    blanco          = WhitePixel(dpy, scr);
    negro           = BlackPixel(dpy, scr);
    ancho_pantalla  = DisplayWidth(dpy, scr);
    alto_pantalla   = DisplayHeight(dpy, scr);

    //
    //Colores
    //
    azul        = colorPorNombre(dpy, "blue");
    purpura     = colorPorNombre(dpy, "medium purple");
    azure       = colorPorNombre(dpy, "azure");
    rojo        = colorPorNombre(dpy, "red");
    amarillo    = colorPorNombre(dpy, "yellow");
    gris        = colorPorNombre(dpy, "gray");
    gris_oscuro = colorPorNombre(dpy, "dim gray");
    gris_claro  = colorPorNombre(dpy, "light grey");

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

    dat_lanzadera.id            = win_scr;
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
    dat_win_graf.id          = 0;
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
                                gris_claro);

    //
    // Establecemos las propiedades de win_edit
    //
    dat_win_edit.id          = 0;
    dat_win_edit.x           = 0;
    dat_win_edit.y           = 0;
    dat_win_edit.ancho       = ancho_pantalla -90 -100;
    dat_win_edit.alto        = alto_pantalla -67;
    dat_win_edit.borde       = 0;
    dat_win_edit.color       = negro;
    dat_win_edit.back_color  = gris_claro;


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
    dat_win_abrir.id          = 0;
    dat_win_abrir.x           = 0;
    dat_win_abrir.y           = 0;
    dat_win_abrir.ancho       = ancho_pantalla -90 -100;
    dat_win_abrir.alto        = alto_pantalla -67;
    dat_win_abrir.borde       = 0;
    dat_win_abrir.color       = negro;
    dat_win_abrir.back_color  = azul;


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
    dat_win_math.id          = 0;
    dat_win_math.x           = 0;
    dat_win_math.y           = 0;
    dat_win_math.ancho       = ancho_pantalla -90 -100;
    dat_win_math.alto        = alto_pantalla -67;
    dat_win_math.borde       = 0;
    dat_win_math.color       = negro;
    dat_win_math.back_color  = amarillo;


    // Establecemos las ventanas
    XSetStandardProperties(dpy, win_scr, TITULO, TITULO, None, NULL, 0, NULL);

    // Establecemos los tipos de eventos que queremos en la ventana principal
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

    //
    // Ponemos la pantalla edit como favorita
    //
    opt_lanzadera = 1;
    //
    // Ponemos el foco en la ventana edit
    //
    sprintf(focus, "win_edit");
}

void refreshUi(){
    int     i = 0;
    XImage  *img[MAX_OPT_LANZADERA];
    char    *iconos[] = {
                        "./imagenes/abrir96.jpeg",
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
            pintaPolinomicas();
            break;
        case 1:
            pintaTrigonometricas();
            break;
        case 2:
            pintaExponenciales();
            break;
        case 3:
            pintaLogaritmicas();
            break;
    }

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

void pintaChkFunciones(){
    XFontStruct *xfs;
    char        msg[1024];
    int         h = 150;
    int         w = 200;
    int         i;

    //
    // Pintamos el texto
    //
    xfs = XLoadQueryFont(dpy, "12x24");
    XSetForeground(dpy, gc_edit, azul);

    sprintf(msg, "Elige el tipo de funcion...");
    setTexto(win_edit, gc_edit, msg, xfs, w, h, 400, 50);

    xfs = XLoadQueryFont(dpy, "9x15bold");

    sprintf(msg,"1.- Funcion polinomica.");
    setTexto(win_edit, gc_edit, msg, xfs, w +50, h +100, 400, 50);
    sprintf(msg,"2.- Funcion trigonometrica.");
    setTexto(win_edit, gc_edit, msg, xfs, w +50, h +150, 400, 50);
    sprintf(msg,"3.- Funcion exponencial.");
    setTexto(win_edit, gc_edit, msg, xfs, w +50, h +200, 400, 50);
    sprintf(msg,"4.- Funcion logaritmica.");
    setTexto(win_edit, gc_edit, msg, xfs, w +50, h +250, 400, 50);

    //
    // Pintamos lo chks
    //
    int jj = opt_funcion;
    i = 0;
    while(i < 4){
        if(i == opt_funcion){
            setClick(dpy, win_edit, gc_edit, w, h +80 +(50 * i), 25, 25);
        }
        else{
            setUnClick(dpy, win_edit, gc_edit, w, h +80 +(50 * i), 25, 25);
        }
        chk_funcion[i].x        = w;
        chk_funcion[i].y        = h + 80 +(50 * i);
        chk_funcion[i].ancho    = 25;
        chk_funcion[i].alto     = 25;
        i++;
    }
}

void pintaPolinomicas(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XSetForeground(dpy, gc_edit, dat_win_edit.back_color);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);
}

void pintaTrigonometricas(){
    int         x0 = (int)(dat_win_edit.ancho/2);
    int         y0 = (int)(dat_win_edit.alto/2);

    XSetForeground(dpy, gc_edit, rojo);
    XFillRectangle(dpy, win_edit, gc_edit, x0 + 22, 22, x0 -46, y0 +154);
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

void setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h2 y v2 claras
    //
    XSetForeground( d, gc, blanco);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

    //
    // La linea h1 y v1 oscuras
    //
    XSetForeground( d, gc, negro);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1
}

void setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h1 y v1 claras
    //
    XSetForeground( d, gc, blanco);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1

    //
    // La linea h2 y v2 oscuras
    //
    XSetForeground( d, gc, negro);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

}

void win_lanzadera_click(XEvent ev){
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
                XMapRaised(dpy, win_abrir);
                sprintf(focus, "win_abrir");
                break;
            case 1:
                XMapRaised(dpy, win_edit);
                sprintf(focus, "win_edit");
                break;
            case 2:
                XMapRaised(dpy, win_math);
                sprintf(focus, "win_math");
                break;
            case 3:
                XMapRaised(dpy, win_graf);
                sprintf(focus, "win_graf");
                break;
            case 4:
                salir();
                break;
        }
    }

    //
    // Refrescamos la pantalla
    //
    refreshUi();
}

void win_edit_click(XEvent ev){
    int i;
    int x, y, ancho, alto;

    //
    // Comprobamos las chk
    //
    x       = chk_funcion[0].x;
    y       = chk_funcion[0].y;
    ancho   = chk_funcion[0].ancho;
    alto    = chk_funcion[0].alto;

    if(ev.xbutton.x >= x && ev.xbutton.x <= (x + ancho)){
        i = 0;
        while(i<4){
            x       = chk_funcion[i].x;
            y       = chk_funcion[i].y;
            ancho   = chk_funcion[i].ancho;
            alto    = chk_funcion[i].alto;

            if(ev.xbutton.y >= y && ev.xbutton.y <= (y + alto)){
                opt_funcion = i;
                i = 4;
            }
            i++;
        }
    }

    //
    // Refrescamos la pantalla
    //
    int jj = opt_funcion;
    refreshEdit();
}

void setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, int x0, int y0, int ancho, int alto){
    int tam_msg_pixels, tam_caracter, n_max_carcters;
    int tam;

    tam_msg_pixels  = XTextWidth(xfs, msg, strlen(msg));
    tam_caracter    = (int) (tam_msg_pixels / strlen(msg));
    n_max_carcters  = (int) (ancho / tam_caracter);

    if(strlen(msg) <= n_max_carcters){
        tam = strlen(msg);
    }
    else{
        tam = n_max_carcters;
    }

    XSetFont(dpy, gc_edit, xfs->fid);

    XDrawString(dpy, win_edit, gc_edit, x0, y0, msg, tam);
}
