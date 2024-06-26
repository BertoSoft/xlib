#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "ui.h"
#include "open.h"

void eventoKeyPress(XEvent ev);
void eventoExposure(XEvent ev);
void eventoButtonPress(XEvent ev);
void eventoFocusIn(XEvent ev);
void eventoFocusOut(XEvent ev);
void eventoConfigureNotify(XEvent ev);
void eventoClientMessage(XEvent ev);
void eventoPropertyNotify(XEvent ev);


int main(){
    XEvent ev;

    initUi();
    contador = 0;

    while(1){

        setFechaHora();

        //
        // Si hay algun evento lo procesamos
        //
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
                case PropertyNotify:
                    eventoPropertyNotify(ev);
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
    // Si pulsamos ESC en cualquiera pantalla inicial que tenga el foco salimos
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

    //
    // Si pulsamos ESC en la pantalla open, cerramos esa pantalla
    //
    if(ev.xfocus.window == open.id){
        if(ev.xkey.keycode == ESC){
            closeOpen();
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

    //
    // Si el foco esta en menu y w[0] esta habilitada
    //
    if(ev.xfocus.window == w[1].id && w[0].is_enabled == True){
        menuClick(ev);
    }
}

void eventoFocusIn(XEvent ev){

}

void eventoFocusOut(XEvent ev){

    //
    // Si la ventana activa pierde el foco se lo devolvemos
    //
    if(ev.xfocus.window == getWindowsActiva() && w[0].is_enabled == False){
        XSetInputFocus(dpy, getWindowsActiva(), RevertToParent, CurrentTime);
        XRaiseWindow(dpy, getWindowsActiva());
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

void eventoPropertyNotify(XEvent ev){
    DatosStates states, states_w0;
    Atom        wm_state = XInternAtom(dpy, "_NET_WM_STATE", True);


    char strEstadoOpen[1024];
    char strEstadoW0[1024];
    contador++;

    //
    // hay un XProperty  _NET_WM_STATE en la ventana activa y w[0].is_enabled == False
    //
    if(ev.xproperty.window == getWindowsActiva() && w[0].is_enabled == False && ev.xproperty.atom == wm_state){
        states      = getWindowStates(getWindowsActiva());
        states_w0   = getWindowStates(w[0].id);

        //
        // Si la ventana activa se minimiza la principal tb
        //
        if(states.is_hidden == True && states_w0.is_maximized == True){
            sprintf(strEstadoOpen, "hidden");
            sprintf(strEstadoW0, "Maximized");
        }

        //
        // Si la ventana activa se maximiza la principal tb
        //
        if(states.is_maximized == True && states_w0.is_hidden == True){
            sprintf(strEstadoOpen, "Maximized");
            sprintf(strEstadoW0, "Hidden");        }
    }

    //
    // Hay un xProperty _NET_WM_STATE en la w[0] y esta esta inactiva
    //
    if(ev.xproperty.window == w[0].id && w[0].is_enabled == False && ev.xproperty.atom == wm_state){
        states      = getWindowStates(getWindowsActiva());
        states_w0   = getWindowStates(w[0].id);

        //
        // Si w0 esta inactiva y se minimiza, minimizamos la ventana activa
        //
        if(states_w0.is_hidden == True && states.is_maximized == True){
            sprintf(strEstadoOpen, "Maximized");
            sprintf(strEstadoW0, "Hidden");
        }

        //
        // Si w0 esta inactiva y se maximiza, maximizamos la ventana activa
        //
        if(states_w0.is_maximized == True && states.is_hidden == True){
            sprintf(strEstadoOpen, "hidden");
            sprintf(strEstadoW0, "Maximized");        }
    }

    int k = 0;

}
