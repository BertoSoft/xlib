#include <stdio.h>
#include <string.h>


#include "ui.h"
#include "w1.h"

void initUi(){

    //
    //  Iniciamos los valores
    //
    dpy     = XOpenDisplay((char *)0);
    scr     = DefaultScreen(dpy);
    blanco  = WhitePixel(dpy, scr);
    negro   = BlackPixel(dpy, scr);

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
    // Creamos la ventana Principal
    //
    dw = crearVentana(DefaultRootWindow(dpy), 0, 0, DisplayWidth(dpy, scr), DisplayHeight(dpy, scr), negro, blanco);

    //
    // Mapeamos la ventana Principal
    //
    XMapRaised(dpy, dw.id);

    //
    // Creamos el boton nueva ventana
    //
    dbtn.x           = dw.ancho -400;
    dbtn.y           = dw.alto -150;
    dbtn.ancho       = 200;
    dbtn.alto        = 50;
    dbtn.is_cheked   = False;
    dbtn.color       = negro;
    dbtn.back_color  = gris;
    sprintf(dbtn.texto, "Crear Nueva Ventana");

    //
    // Colocamos el id de w1 en -1
    //
    dw1.is_enabled = False;

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);

}

void resizeUi(XEvent ev){

    dw.x        = ev.xconfigure.x;
    dw.y        = ev.xconfigure.y;
    dw.alto     = ev.xconfigure.width;
    dw.ancho    = ev.xconfigure.height;

}

void pintaUi(){
    int             x, y, ancho, alto;
    XFontStruct     *xfs;

    xfs = XLoadQueryFont(dpy, FONT_N_B);

    //
    // Pintamos el btn
    //
    setUnClick(dpy, dw.id, dw.gc, dbtn.x, dbtn.y, dbtn.ancho, dbtn.alto);

    x       = dbtn.x + 10;
    y       = dbtn.y + (dbtn.alto / 2) +7;
    ancho   = dbtn.ancho - 20;
    alto    = 50;

    setTexto(dw.id, dw.gc, dbtn.texto, xfs, dbtn.color, x, y, ancho, alto);

}

void closeUi(){

    //
    //  Cerramos las ventanas
    //
    cerrarVentana(dw);

    //
    // Cerramos la conexion
    //
    XCloseDisplay(dpy);
}

void btn_click(){

    if(dw1.is_enabled == False){
        showW1();
        dw.is_enabled = False;
    }

}


DatosWindow crearVentana(Window padre, int x, int y, int ancho, int alto, unsigned long color, unsigned long back_color){
    DatosWindow dww;

    dww.id = XCreateSimpleWindow(dpy,
                            padre,
                            x,
                            y,
                            ancho,
                            alto,
                            0,
                            color,
                            back_color
                            );

    dww.padre        = padre;
    dww.x            = x;
    dww.y            = y;
    dww.ancho        = ancho;
    dww.alto         = alto;
    dww.is_enabled   = True;
    dww.color        = color;
    dww.back_color   = back_color;

    XSelectInput(dpy, dww.id, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | SubstructureNotifyMask | FocusChangeMask);

    dww.gc = XCreateGC(dpy, dww.id, 0, 0);

    cerrar_ventana = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, dww.id, &cerrar_ventana, 1);

    return dww;
}

void cerrarVentana(DatosWindow dww){

    XFreeGC(dpy, dww.gc);
    XUnmapWindow(dpy, dww.id);
    XDestroyWindow(dpy, dww.id);
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
    XSetForeground( d, gc, gris_oscuro);
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
    XSetForeground( d, gc, gris_oscuro);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

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

void setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto){
    int tam_msg_pixels, tam_caracter, n_max_carcters;
    int tam;

    if(strlen(msg) == 0){
        tam_msg_pixels = 0;
    }
    else{
        tam_msg_pixels  = XTextWidth(xfs, msg, strlen(msg));
    }
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
