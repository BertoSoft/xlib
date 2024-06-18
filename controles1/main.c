#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "ui.h"

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
    // Si pulsamos ESC en cualquiera pantalla salimos
    //
    if(ev.xfocus.window == dv_scr.id ||
       ev.xfocus.window == dv_menu.id ||
       ev.xfocus.window == dv_w.id ||
       ev.xfocus.window == dv_inf.id){

            if(ev.xkey.keycode == ESC){
                closeUi();
                exit(0);
            }
    }
}

void eventoExposure(XEvent ev){

}

void eventoButtonPress(XEvent ev){

}

void eventoFocusIn(XEvent ev){

}

void eventoFocusOut(XEvent ev){

}

void eventoConfigureNotify(XEvent ev){

}

void eventoClientMessage(XEvent ev){

    //
    // Si se presiona el boton x de la ventana Principal cerramos el programa
    //
    if(ev.xclient.window == dv_scr.id && ev.xclient.data.l[0] == cerrar_ventana){
        closeUi();
        exit(0);
    }

}
