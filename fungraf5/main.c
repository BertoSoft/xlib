#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn


#include "ui.h"


int main(){
    XEvent  ev;
    char    key;

    //
    // Iniciamods la Xlib
    //
    initUi();
    initControls();

    //
    // Inicializamos los valores
    //
    opt_lanzadera   = 2;
    opt_funcion     = 0;

    while(1){

        XNextEvent(dpy, &ev);
        //
        // Eventos de la Ventana scr
        //
        if(ev.xfocus.window == win_scr){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                        refreshUi();
                    }
                    break;

                case ConfigureNotify:
                    resizeWin(ev);
                    break;
            }
        }

        //
        // Eventos de la ventana lanzadera
        //
        if( ev.xfocus.window == win_lanzadera ){
            switch(ev.type){
                case ButtonPress:
                    winLanzaderaClick(ev);
                    break;

                case KeyPress:
                    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);
                    if(key == ESC){
                        salir();
                    }
                    else{
                        winEditKeyPress(ev);
                    }
                    break;
            }
        }

        //
        // Eventos de la ventana win_Abrir
        //
        if( ev.xfocus.window == win_abrir){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                    }
                    break;

                case KeyPress:
                    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);
                    if(key == ESC) salir();
                    break;

                case ButtonPress:
                    break;
            }
        }

        //
        // Eventos de la ventana win_edit
        //
        if( ev.xfocus.window == win_edit){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                        refreshEdit();
                    }
                    break;

                case KeyPress:
                    winEditKeyPress(ev);
                    break;

                case ButtonPress:
                    winEditClick(ev);
                    break;

            }
        }

        //
        // Eventos de la ventana win_math
        //
        if( ev.xfocus.window == win_math){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                    }
                    break;

                case KeyPress:
                    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);
                    if(key == ESC) salir();
                    break;

                case ButtonPress:
                    break;
            }
        }

        //
        // Eventos de la ventana win_graf
        //
        if( ev.xfocus.window == win_graf){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                    }
                    break;

                case KeyPress:
                    key = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, ev.xkey.state & ShiftMask ? 1 : 0);
                    if(key == ESC) salir();
                    break;

                case ButtonPress:
                    break;
            }
        }
    }
}



