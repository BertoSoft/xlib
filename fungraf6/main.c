#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn

#include "ui.h"

int main(){
    XEvent  ev;

    initUi();

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
                    pintaUi();
                    }
                    break;

                case ConfigureNotify:
                    resizeWin(ev);
                    break;
            }
        }

        //
        // Eventos de la ventana menu
        //
        if( ev.xfocus.window == win_menu ){
            switch(ev.type){
                case ButtonPress:
                    break;

                case KeyPress:
                    if(ev.xkey.keycode == ESC){
                        salir();
                    }
                    break;
            }
        }

        //
        // Eventos de la ventana win
        //
        if( ev.xfocus.window == win){
            switch(ev.type){
                case Expose:
                    // Si es la ultima interuccion del evento expose
                    if( ev.xexpose.count == 0 ){
                    }
                    break;

                case KeyPress:
                    if(ev.xkey.keycode == ESC){
                        salir();
                    }
                    break;

                case ButtonPress:
                    break;
            }
        }


    }

}
