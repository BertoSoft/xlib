
#include "open.h"
#include "ui.h"

void initOpen(){
    int x, y, ancho, alto;

    //
    // Colocamos la ventana principal como inactiva
    //
    w[0].is_enabled = False;

    //
    // Creamos la ventana
    //
    ancho   = 800;
    alto    = 600;
    x       = (int)((DisplayWidth(dpy, scr) / 2) - (ancho / 2));
    y       = (int)(DisplayHeight(dpy, scr) / 2) -(alto / 2);

    open = crearVentana(DefaultRootWindow(dpy), x, y, ancho, alto, negro, blanco);

    //
    // La mepeamos
    //
    XMapRaised(dpy, open.id);

    //
    // La centramos
    //
    XMoveResizeWindow(dpy, open.id, x, y, ancho, alto);

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);
}

void showOpen(){

    initOpen();
    pintaOpen();
}

void pintaOpen(){

}

void closeOpen(){

    cerrarVentana(open);
    open.is_enabled     = False;
    w[0].is_enabled     = True;
}
