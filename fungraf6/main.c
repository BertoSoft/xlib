#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn

#include "ui.h"

void* reloj (void* q);

int main(){
    XEvent      ev;

    //
    // Iniciamos la Ui
    //
    initUi();

    //
    // Iniciamos los valores
    //
    //opt_menu = 2;

    //
    //Loop
    //
    while(1){
        setFechaHora();
        //
        // Si hay algun evento lo procesamos
        //
        while(QLength(dpy) > 0){
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
                        menuClick(ev);
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
}


