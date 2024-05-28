#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>     // Para usar xkbKeycodeToKeysyn

#include "ui.h"
#include "edit.h"

void gesExpose(XEvent ev);
void gesConfigureNotify(XEvent ev);
void gesButtonPress(XEvent ev);
void gesKeyPress(XEvent ev);
void gesVisibilityNotify(XEvent ev);

int main(){
    XEvent      ev;

    //
    // Iniciamos la Ui
    //
    initUi();

    //
    // Iniciamos los valores
    //
    opt_menu    = 2;
    showEdit();

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

            switch(ev.type){
                case Expose:
                    if( ev.xexpose.count == 0 ){
                        gesExpose(ev);
                    }
                    break;
                case ConfigureNotify:
                    gesConfigureNotify(ev);
                    break;
                case ButtonPress:
                    gesButtonPress(ev);
                    break;
                case KeyPress:
                    gesKeyPress(ev);
                    break;
            }
        }
    }
}

void gesExpose(XEvent ev){

    if(ev.xfocus.window == win_scr){
        pintaUi();
    }
    if(ev.xfocus.window == w_edit){
        pintaEdit();
    }
}

void gesConfigureNotify(XEvent ev){

    if(ev.xfocus.window == win_scr){
        resizeWin(ev);
    }
}

void gesButtonPress(XEvent ev){
    int i;

    if(ev.xfocus.window == win_menu){
        menuClick(ev);
    }

    i = 0;
    while(i < 4){
        if(ev.xfocus.window == chk[i].id){
            editClick(ev);
        }
        i++;
    }

}

void gesKeyPress(XEvent ev){

    if(ev.xfocus.window == win_menu && ev.xkey.keycode == ESC){
        salir();
    }

    if(ev.xfocus.window == win && ev.xkey.keycode == ESC){
        salir();
    }

    if(ev.xfocus.window == w_edit && ev.xkey.keycode == ESC){
        salir();
    }
}
