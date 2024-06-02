#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
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
void colorCursor(int s);

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
    // Iniciamos la alarma de 0,5 seg
    //
    ualarm(500000, 1);
    signal(SIGALRM, colorCursor);

    //
    //Loop
    //
    while(1){
        setFechaHora();
        pintaCursor();

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

    if(ev.xfocus.window == win_msg_box){
        pintaMsgBox();
    }
}

void gesConfigureNotify(XEvent ev){

    if(ev.xfocus.window == win_scr){
        resizeWin(ev);
    }
}

void gesButtonPress(XEvent ev){

    if(ev.xfocus.window == win_menu && dat_msg_box.is_enabled == False){
        menuClick(ev);
    }

    if(ev.xfocus.window == w_edit && dat_msg_box.is_enabled == False){
        editClick(ev);
    }

    if(ev.xfocus.window == win_msg_box){
        msgBoxClick(ev);
    }
}

void gesKeyPress(XEvent ev){

    //
    // win_mwnu KeyPress y no hay msgBox
    //
    if(ev.xfocus.window == win_menu && ev.xkey.keycode == ESC && dat_msg_box.is_enabled == False){
        salir();
    }

    //
    // Win_menu keyPress y no hay msg y no es ESC, editKeyPress
    //
    if(ev.xfocus.window == win_menu && ev.xkey.keycode != ESC && dat_msg_box.is_enabled == False){
        editKeyPress(ev);
    }

    //
    // win_mwnu KeyPress y hay MsgBox, debemos cerrar este
    //
    if(ev.xfocus.window == win_menu && ev.xkey.keycode == ESC && dat_msg_box.is_enabled == True){
        hideMsgBox();
    }

    //
    // Win KeyPress
    //
    if(ev.xfocus.window == win && ev.xkey.keycode == ESC && dat_msg_box.is_enabled == False){
        salir();
    }

    //
    // Win MsgBox
    //
    if(ev.xfocus.window == win_msg_box && ev.xkey.keycode == ESC){
        hideMsgBox();
    }

    //
    // w_edit KeyPress
    //
    if(ev.xfocus.window == w_edit){
        editKeyPress(ev);
    }

}

void colorCursor(int s){

    if(color_cursor == True){
        color_cursor = False;
    }
    else{
        color_cursor = True;
    }
    signal(SIGALRM, SIG_IGN); /* ignore this signal */
    ualarm(500000, 1);
    signal(SIGALRM, colorCursor);
}
