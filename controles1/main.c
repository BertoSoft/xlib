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
    // Si w[0].is_enabled = false y pinchamo en cualeuier parte de w[0], traemos al frente getWindowsActiva()
    //
    if(ev.xbutton.window == w[1].id ||
       ev.xbutton.window == w[2].id ||
       ev.xbutton.window == w[3].id){

        DatosStates states = getWindowStates(getWindowsActiva());
        if(!w[0].is_enabled && states.is_hidden){

            setActiveWindow(getWindowsActiva());
        }
       }

    //
    // Si el foco esta en menu y w[0] esta habilitada
    //
    if(ev.xbutton.window == w[1].id && w[0].is_enabled == True){
        menuClick(ev);
    }

    //
    // Si el foco esta en open lo mandamos a openClick()
    //
    if(ev.xbutton.window == open.id){
        openClick(ev);
    }
}

void eventoFocusIn(XEvent ev){
    DatosStates states = getWindowStates(getWindowsActiva());

    if(ev.xfocus.window == w[0].id && w[0].is_enabled == False && !states.is_hidden){
        setActiveWindow(getWindowsActiva());
    }

}

void eventoFocusOut(XEvent ev){


}

void eventoConfigureNotify(XEvent ev){

    //
    // Si la w[0], cambia de tamaño
    //
    if(ev.xconfigure.window == w[0].id){
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

}
