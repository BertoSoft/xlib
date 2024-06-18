
#include <X11/Xlib.h>

#include "ui.h"
#include "w1.h"

void initW1(){
    int x, y, ancho, alto;

    //
    // Creamos la ventana
    //
    ancho   = 800;
    alto    = 600;
    x       = (int)((DisplayWidth(dpy, scr) / 2) - (ancho / 2));
    y       = (int)(DisplayHeight(dpy, scr) / 2) -(alto / 2);

    dw1 = crearVentana(DefaultRootWindow(dpy), x, y, ancho, alto, negro, blanco);

    //
    // La mapeamos
    //
    XMapRaised(dpy, dw1.id);

    //
    // La centramos
    //
    XMoveResizeWindow(dpy, dw1.id, 540, 240, ancho, alto);

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);

}

void pintaW1(){

}

void closeW1(){

    cerrarVentana(dw1);
    dw1.is_enabled  = False;
    dw.is_enabled   = True;
}

void showW1(){

    initW1();
    pintaW1();


}
