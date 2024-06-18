#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "ui.h"
#include "w1.h"

void eventoKeyPress(XEvent ev);
void eventoExposure(XEvent ev);
void eventoButtonPress(XEvent ev);
void eventoFocusIn(XEvent ev);
void eventoFocusOut(XEvent ev);
void eventoConfigureNotify(XEvent ev);
void eventoClientMessage(XEvent ev);


int main(){
    XEvent ev;

    initUi();
    pintaUi();

    while(1){
        XNextEvent(dpy, &ev);
        switch(ev.type){
            case Expose:
                if( ev.xexpose.count == 0 ){
                    eventoExposure(ev);
                }
                break;
            case KeyPress:
                eventoKeyPress(ev);
                break;
            case ButtonPress:
                eventoButtonPress(ev);
                break;
            case FocusIn:
                eventoFocusIn(ev);
                break;
            case FocusOut:
                eventoFocusOut(ev);
                break;
            case ConfigureNotify:
                eventoConfigureNotify(ev);
                break;
            case ClientMessage:
                eventoClientMessage(ev);
                break;
        }
    }
}

//
// Funciones
//
void eventoKeyPress(XEvent ev){

    //
    // Si pulsamos ESC en la pantalla principal salimos
    //
    if(ev.xfocus.window == dw.id && ev.xkey.keycode == ESC){
        closeUi();
        exit(0);
    }

    //
    // Si pulsamos ESC en la dw1, volvemos a dw
    //
    if(ev.xfocus.window == dw1.id && ev.xkey.keycode == ESC && dw1.is_enabled == True){
        closeW1();
    }

}

void eventoExposure(XEvent ev){

    if(ev.xfocus.window == dw.id){
        pintaUi();
    }

    if(ev.xfocus.window == dw1.id){
        pintaW1();
    }

}

void eventoButtonPress(XEvent ev){
    int x, y, x0, x1, y0, y1;

    x   = ev.xbutton.x;
    y   = ev.xbutton.y;

    //
    // Boton btn
    //
    x0  = dbtn.x;
    x1  = dbtn.x + dbtn.ancho;
    y0  = dbtn.y;
    y1  = dbtn.y + dbtn.alto;

    if(ev.xfocus.window == dw.id){
        if(x0 < x && x < x1){
            if(y0 < y && y < y1){
                btn_click();
            }
        }
    }

}

void eventoFocusIn(XEvent ev){

}

void eventoFocusOut(XEvent ev){

    if(ev.xfocus.window == dw1.id && dw1.is_enabled == True){
        XSetInputFocus(dpy, dw1.id, RevertToParent, CurrentTime);
        XRaiseWindow(dpy, dw1.id);
    }
}

void eventoConfigureNotify(XEvent ev){

    if(ev.xconfigure.window == dw.id && dw1.is_enabled == True){
        XRaiseWindow(dpy, dw1.id);
    }

}

void eventoClientMessage(XEvent ev){

    //
    // Si se presiona el boton x de la ventana Principal cerramos el programa
    //
    if(ev.xclient.window == dw.id && ev.xclient.data.l[0] == cerrar_ventana){
        closeUi();
        exit(0);
    }

    //
    // Si se presiona el boton x de la ventana w1 cerramos esta ventana
    //
    if(ev.xclient.window == dw1.id && ev.xclient.data.l[0] == cerrar_ventana){
        closeW1();
    }

}

