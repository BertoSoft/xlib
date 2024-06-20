#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "ui.h"
#include "open.h"

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

    while(1){
        setFechaHora();
        while(QLength(dpy) > 0){
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
}

//
// Funciones
//
void eventoKeyPress(XEvent ev){

    //
    // Si pulsamos ESC en cualquiera pantalla salimos
    //
    if(ev.xfocus.window == w[0].id ||
       ev.xfocus.window == w[1].id ||
       ev.xfocus.window == w[2].id ||
       ev.xfocus.window == w[3].id){

            if(ev.xkey.keycode == ESC){
                closeUi();
                exit(0);
            }
    }
}

void eventoExposure(XEvent ev){

    if(ev.xfocus.window == w[0].id){
        pintaUi();
    }

    if(ev.xfocus.window == open.id){
        pintaOpen();
    }
}

void eventoButtonPress(XEvent ev){

    if(ev.xfocus.window == w[1].id && w[0].is_enabled == True){
        menuClick(ev);
    }
}

void eventoFocusIn(XEvent ev){

    if(ev.xfocus.window == w[0].id && w[0].is_enabled == False){

}

}

void eventoFocusOut(XEvent ev){

    //
    // Si la ventana activa pierde el foco se lo devolvemos
    //
    if(ev.xfocus.window == open.id && open.is_enabled == True){
        XSetInputFocus(dpy, open.id, RevertToParent, CurrentTime);
        XRaiseWindow(dpy, open.id);
    }
}

void eventoConfigureNotify(XEvent ev){

    if(ev.xfocus.window == w[0].id){
        resizeUi(ev);
    }
}

void eventoClientMessage(XEvent ev){

    //
    // Si se presiona el boton x de la ventana Principal y esta activa cerramos el programa
    //
    if(ev.xclient.window == w[0].id && ev.xclient.data.l[0] == cerrar_ventana && w[0].is_enabled == True){
        closeUi();
        exit(0);
    }

    //
    // Si se presiona el boton x de la ventana open cerramos esta ventana
    //
    if(ev.xclient.window == open.id && ev.xclient.data.l[0] == cerrar_ventana){
        closeOpen();
    }
}
