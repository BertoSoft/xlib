#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "ui.h"
#include "grafica.h"
#include "edit.h"

#define MAX_MENU    6       // Maximo de botones del menu
#define BTN_MENU    100     // Ancho de los botones del menu

void initUi(){
    int ancho_scr, alto_scr;
    int i;

    //
    // Iniciamos los valores de los controles
    //
    i = 0;
    while(i < MAX_ET){
        et[0].is_enabled = False;
        i++;
    }

    //
    // iniciamos las variables
    //
    dpy             = XOpenDisplay((char *)0);
    scr             = DefaultScreen(dpy);
    blanco          = WhitePixel(dpy, scr);
    negro           = BlackPixel(dpy, scr);
    ancho_scr       = DisplayWidth(dpy, scr);
    alto_scr        = DisplayHeight(dpy, scr);
    win_msg_box     = -1;


    //
    //Iniciamos los colores
    //
    azul        = colorPorNombre(dpy, "blue");
    purpura     = colorPorNombre(dpy, "medium purple");
    azure       = colorPorNombre(dpy, "azure");
    rojo        = colorPorNombre(dpy, "red");
    amarillo    = colorPorNombre(dpy, "yellow");
    gris        = colorPorNombre(dpy, "gray");
    gris_oscuro = colorPorNombre(dpy, "DarkSlateGray");
    gris_claro  = colorPorNombre(dpy, "LightGray");

    //
    // Establecemos color de cursor en True
    //
    color_cursor = True;

    //
    // creamos la ventana scr
    //
    win_scr = XCreateSimpleWindow(dpy,
                                DefaultRootWindow(dpy),
                                0,
                                0,
                                ancho_scr,
                                alto_scr,
                                0,
                                negro,
                                blanco);

    dat_scr.id          = win_scr;
    dat_scr.x           = 0;
    dat_scr.y           = 0;
    dat_scr.ancho       = ancho_scr;
    dat_scr.alto        = alto_scr;
    dat_scr.borde       = 0;
    dat_scr.color       = negro;
    dat_scr.back_color  = blanco;

    //
    // creamos la ventana menu
    //
    win_menu = XCreateSimpleWindow(dpy,
                                win_scr,
                                ancho_scr - 90 -100,
                                0,
                                100,
                                alto_scr -60,
                                0,
                                negro,
                                gris);

    dat_menu.id            = win_menu;
    dat_menu.x             = ancho_scr -90 -100;
    dat_menu.y             = 0;
    dat_menu.ancho         = 100;
    dat_menu.alto          = alto_scr -60 -50;
    dat_menu.borde         = 0;
    dat_menu.color         = negro;
    dat_menu.back_color    = gris;

    //
    // creamos la ventana principal
    //
    win = XCreateSimpleWindow(dpy,
                            win_scr,
                            0,
                            0,
                            ancho_scr -90 -100,
                            alto_scr - 60 -50,
                            0,
                            negro,
                            gris_claro);

    dat_win.id          = win;
    dat_win.x           = 0;
    dat_win.y           = 0;
    dat_win.ancho       = ancho_scr -90 -100;
    dat_win.alto        = alto_scr -107;
    dat_win.borde       = 0;
    dat_win.color       = negro;
    dat_win.back_color  = gris_claro;


    //
    // creamos la barra inferior
    //
    win_inf = XCreateSimpleWindow(dpy,
                            win_scr,
                            0,
                            alto_scr -108,
                            ancho_scr -90 -100,
                            43,
                            0,
                            negro,
                            gris);

    dat_inf.id          = win_inf;
    dat_inf.x           = 0;
    dat_inf.y           = alto_scr -108;
    dat_inf.ancho       = ancho_scr -90 -100;
    dat_inf.alto        = 33;
    dat_inf.borde       = 0;
    dat_inf.color       = negro;
    dat_inf.back_color  = gris;


    //
    // Establecemos las propiedades de la ventana scr
    //
    XSetStandardProperties(dpy, win_scr, TITULO, TITULO, None, NULL, 0, NULL);

    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, win_scr, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask );
    XSelectInput(dpy, win_menu, ExposureMask | ButtonPressMask | KeyPressMask);
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos los gcs
    //
    gc_scr      = XCreateGC(dpy, win_scr, 0, 0);
    gc_menu     = XCreateGC(dpy, win_menu, 0, 0);
    gc_win      = XCreateGC(dpy, win, 0, 0);
    gc_inf      = XCreateGC(dpy, win_inf, 0, 0);


    //
    // Establecemos el valor minimo de win_scr
    //
    XSizeHints *tam_minimo = XAllocSizeHints();

    tam_minimo->flags        = PMinSize;
    tam_minimo->min_width    = ANCHO_MINIMO;
    tam_minimo->min_height   = ALTO_MINIMO;
    XSetWMNormalHints(dpy, win_scr, tam_minimo);

    XFree(tam_minimo);

    //
    // Borramos la pantalla
    //
    XClearWindow(dpy, win_scr);

    //
    // Mapeamos las pantallas
    //
    XMapRaised(dpy, win_scr);
    XMapRaised(dpy, win_menu);
    XMapRaised(dpy, win);
    XMapRaised(dpy, win_inf);


}

void resizeWin(XEvent ev){

    //
    // Cambiamos las medidas
    //
    dat_scr.x       = ev.xconfigure.x;
    dat_scr.y       = ev.xconfigure.y;
    dat_scr.ancho   = ev.xconfigure.width;
    dat_scr.alto    = ev.xconfigure.height;

    dat_win.x       = 1;
    dat_win.y       = 1;
    dat_win.ancho   = dat_scr.ancho -107;
    dat_win.alto    = dat_scr.alto -37;

    dat_menu.x      = dat_scr.ancho -103;
    dat_menu.y      = 1;
    dat_menu.ancho  = 100;
    dat_menu.alto   = dat_scr.alto -37;

    dat_inf.x      = 0;
    dat_inf.y      = dat_scr.alto -37;
    dat_inf.ancho  = dat_scr.ancho -4;
    dat_inf.alto   = 44;

    if(opt_menu == 2){pintaEdit();}
    if(opt_menu == 4){pintaGrafica();}

}

void closeUi(){

    XFreeGC(dpy, gc_menu);
    XFreeGC(dpy, gc_win);
    XFreeGC(dpy, gc_inf);
    XFreeGC(dpy, gc_scr);


    XUnmapWindow(dpy, win_menu);
    XUnmapWindow(dpy, win);
    XUnmapWindow(dpy, win_inf);
    XUnmapWindow(dpy, win_scr);

    XDestroyWindow(dpy,win_menu);
    XDestroyWindow(dpy,win);
    XDestroyWindow(dpy,win_inf);
    XDestroyWindow(dpy,win_scr);

    XCloseDisplay(dpy);
}

void salir(){

    closeUi();
    exit(0);
}

void pintaUi(){
    int     i = 0;
    XImage  *img[MAX_MENU];
    char    *iconos[] = {
                        "./imagenes/abrir96.jpeg",
                        "./imagenes/guardar96.jpeg",
                        "./imagenes/edit96.png",
                        "./imagenes/math96.png",
                        "./imagenes/grafica96.png",
                        "./imagenes/salir96.jpeg"
                        };

    //
    // Recolocamos todas las ventanas
    //
    XMoveResizeWindow(dpy, win, dat_win.x, dat_win.y, dat_win.ancho, dat_win.alto);
    XMoveResizeWindow(dpy, win_menu, dat_menu.x, dat_menu.y, dat_menu.ancho, dat_menu.alto);
    XMoveResizeWindow(dpy, win_inf, dat_inf.x, dat_inf.y, dat_inf.ancho, dat_inf.alto);

    //
    // Borramos la pantalla
    //
    XClearWindow(dpy, win);

    //
    // pintamos menu
    //
    i = 0;
    while(i < MAX_MENU){
        //
        // Cargamos las imagenes
        //
        img[i] = loadImagen(dpy, win_menu, iconos[i]);
        if(img[i]){
            XPutImage(dpy, win_menu, gc_menu, img[i], 0, 0, 4, 4 + (i * 100), 92, 92);
        }

        //
        // creamos los efectos boton presionado
        //
        if(i == opt_menu){
            setClick(dpy, win_menu, gc_menu, 3, 3 + (i*BTN_MENU), dat_menu.ancho -4, BTN_MENU -4);
        }
        else{
            setUnClick(dpy, win_menu, gc_menu, 3, 3 + (i*BTN_MENU), dat_menu.ancho -4, BTN_MENU -4);
        }
        i++;
    }

    //
    // pintamos la barra inf
    //
    setClick(dpy, win_inf, gc_inf, 2, 2, (int) (dat_inf.ancho * 0.7), dat_inf.alto - 4);                                //  70%
    setClick(dpy, win_inf, gc_inf, (int) (dat_inf.ancho * 0.7), 2, (int) (dat_inf.ancho * 0.2), dat_inf.alto - 4);      //  20%
    setClick(dpy, win_inf, gc_inf, (int) (dat_inf.ancho * 0.9), 2, (int) (dat_inf.ancho * 0.1), dat_inf.alto -4);       //  10%
    setFechaHora();

    XMapRaised(dpy, win_scr);
    XFlush(dpy);
}

void menuClick(XEvent ev){
    int i;

    //
    // Cerramos la ventana actual
    //
    switch(opt_menu){
        case 0:
            break;
        case 1:
            break;
        case 2:
            closeEdit();
            break;
        case 3:
            break;
        case 4:
            closeGrafica();
            break;
        case 5:
            salir();
            break;
    }

    //
    // Cambiamos la opt_menu segun el boton pulsado
    //
    if(ev.xbutton.x > 0 && ev.xbutton.x < dat_menu.ancho){
        i =0;
        while(i < MAX_MENU){
            if(ev.xbutton.y > (i * BTN_MENU) && ev.xbutton.y < (i * BTN_MENU) + BTN_MENU){
                opt_menu = i;
                i = MAX_MENU;
            }
            i++;
        }
    }

    //
    // Segun boton pulsado actuamos
    //
    switch(opt_menu){
        case 0:
            break;
        case 1:
            break;
        case 2:
            showEdit();
            break;
        case 3:
            break;
        case 4:
            showGrafica();
            break;
        case 5:
            salir();
            break;
    }

    //
    // Pintamos la UI
    //
    pintaUi();
}

void msgBoxClick(XEvent ev){
    int x, y, x0, y0, y1, x1;

    x0  = dat_msg_box.ancho - 175;
    y0  = dat_msg_box.alto - 75;
    x1  = x0 + 125;
    y1  = y0 + 40;
    x   = ev.xbutton.x;
    y   = ev.xbutton.y;

    if(x0 < x && x < x1 && y0 < y && y < y1){
        hideMsgBox();
    }
}

XImage *loadImagen(Display *display, Window w, char *ruta){
    Pixmap      pix;
    Imlib_Image imlib_img;
    int         ancho, alto;
    Screen      *scr;
    XImage      *x_img;

    //
    // Leemos el archivo
    //
    imlib_img = imlib_load_image(ruta);

    //
    // Si el archivo no existe, devolvemos nulo
    //
    if(imlib_img == 0x0){
        x_img = NULL;
    }
    //
    // Si el archivo existe devolvemos *XImage
    //
    else{

    //
    // Establecemos los contextos
    //
    imlib_context_set_image(imlib_img);
    ancho   = imlib_image_get_width();
    alto    = imlib_image_get_height();

    //
    // Obtenemos la pantalla por defecto y creamos el pixmap vacío
    //
    scr = DefaultScreenOfDisplay(display);
    pix = XCreatePixmap(display, w, ancho, alto, DefaultDepthOfScreen(scr));

    //
    // Establecemos valores de imlib
    //
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisualOfScreen(scr));
    imlib_context_set_colormap(DefaultColormapOfScreen(scr));
    imlib_context_set_drawable(pix);

    //
    // Copiamos la imagen imlib en pixmap
    //
    imlib_render_image_on_drawable(0, 0);

    //
    // Pasamos el pixmap a XImage
    //
    x_img = XGetImage(display, pix, 0, 0, ancho, alto, 0xFFFFFFFF, ZPixmap);

    XFreePixmap(display, pix);
    }

    return x_img;
}

void setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h2 y v2 claras
    //
    XSetForeground( d, gc, blanco);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

    //
    // La linea h1 y v1 oscuras
    //
    XSetForeground( d, gc, gris_oscuro);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1
}

void setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto){

    XSetLineAttributes( d, gc, 3, LineSolid, CapRound, JoinMiter);

    //
    // La linea h1 y v1 claras
    //
    XSetForeground( d, gc, blanco);
    XDrawLine( d, w, gc, x, y, x + ancho, y);                                   // H1
    XDrawLine( d, w, gc, x, y, x, y + alto);                                    // V1

    //
    // La linea h2 y v2 oscuras
    //
    XSetForeground( d, gc, gris_oscuro);
    XDrawLine( d, w, gc, x, y + alto, x + ancho, y + alto);                     // H2
    XDrawLine( d, w, gc, x + ancho, y, x + ancho, y + alto);                    // V2

}

void setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto){
    int tam_msg_pixels, tam_caracter, n_max_carcters;
    int tam;

    if(strlen(msg) == 0){
        tam_msg_pixels = 0;
    }
    else{
        tam_msg_pixels  = XTextWidth(xfs, msg, strlen(msg));
    }
    if(tam_msg_pixels > 0){
        tam_caracter    = (int) (tam_msg_pixels / strlen(msg));
        n_max_carcters  = (int) (ancho / tam_caracter);
    }
    else{
        tam_caracter    = 0;
        n_max_carcters  = 0;
    }


    if(strlen(msg) <= n_max_carcters){
        tam = strlen(msg);
    }
    else{
        tam = n_max_carcters;
    }

    XSetFont(dpy, gc, xfs->fid);
    XSetForeground(dpy, gc, color);
    XDrawString(dpy, w, gc, x0, y0, msg, tam);
}

void setMsgVisualEt(Window id, char *p_msg){
    int i;

    i = 0;
    while(i< MAX_ET){
        if(et[i].id == id){
            strcpy( et[i].msg_visual, p_msg);
            i =MAX_ET;
        }
        i++;
    }
}

void setEditText(Datos et){
    int x, y;
    int     i, j;
    int     ancho_msg, max_caracters, ancho_caracter, len_msg;
    char    msg_visual[1024];

    //
    // Obtenemos los max caracteres a mostrar, y le restamos 1
    //
    len_msg     = strlen(et.msg);
    ancho_msg   = XTextWidth(et.xfs, et.msg, len_msg);

    if(ancho_msg > 0){
        ancho_caracter  = (int) (ancho_msg / len_msg);
        max_caracters   = (int) (et.ancho / ancho_caracter);
    }
    else{
        ancho_caracter  = 9;
        max_caracters   = 0;
    }
    max_caracters --;
    if(max_caracters < 0) max_caracters = 0;
    if(max_caracters > 1022) max_caracters = 1022;

    //
    // Ahora si el texto es muy grande, mostramos solo el final, msg_visual
    //
    if(len_msg > 0 && len_msg >= max_caracters){
        i = max_caracters -1;
        j = 0;
        msg_visual[max_caracters +1]   = '\0';
        while(j < max_caracters){
            msg_visual[i] = et.msg[len_msg -1 -j];
            i--;
            j++;
        }
    }
    else{
        strcpy(msg_visual, et.msg);
    }

    //
    // actualizamos el campo msg_visual de et
    //
    setMsgVisualEt(et.id, msg_visual);

    //
    // Borramos todo lo anterior
    //
    XSetForeground(dpy, et.gc, et.back_color);
    XFillRectangle(dpy, et.padre, et.gc, et.x, et.y, et.ancho, et.alto);

    //
    // Colocamos el efecto caja
    //
    setClick(dpy, et.padre, et.gc, et.x + 2, et.y + 2, et.ancho - 4, et.alto - 4);

    //
    // Mostramos msg_visual
    //
    x = et.x + 9;
    y = et.y + (int)(et.alto / 2) + 7;

    setTexto(et.padre, et.gc, msg_visual, et.xfs, et.color, x, y, et.ancho, et.alto);

}

void setFechaHora(){
    char            msg[1024];
    int             x, y, ancho, alto;
    XFontStruct     *xfs;
    time_t          time_actual;
    struct tm       *tm_actual;

    time_actual = time(NULL);
    tm_actual   = localtime(&time_actual);
    xfs         = XLoadQueryFont(dpy, FONT_N_B);

    x       = (int) (dat_inf.ancho * 0.7) + 10;
    y       = (int) (dat_inf.alto / 2) + 5;
    ancho   = (int) (dat_inf.ancho * 0.18);
    alto    = dat_inf.alto;

    strftime(msg, 1023, "%A, %d de %B de %Y", tm_actual);
    XSetForeground(dpy, gc_inf, dat_inf.back_color);
    XFillRectangle(dpy, win_inf, gc_inf, (int)(dat_inf.ancho * 0.7) + 2, 4, ancho, 46);
    setTexto(win_inf, gc_inf, msg, xfs, azul, x, y, ancho, alto);

    x       = (int) (dat_inf.ancho * 0.9) + 50;
    y       = (int) (dat_inf.alto / 2) + 5;
    ancho   = (int) (dat_inf.ancho * 0.1);
    alto    = dat_inf.alto;

    strftime(msg, 1023, "%X", tm_actual);
    XSetForeground(dpy, gc_inf, dat_inf.back_color);
    XFillRectangle(dpy, win_inf, gc_inf, (int)(dat_inf.ancho * 0.9) + 2, 4, ancho, 46);
    setTexto(win_inf, gc_inf, msg, xfs, azul, x, y, ancho, alto);

}

int isNumerico(char *pMsg){
    int     isNumerico = False;
    int     i;
    char    msg[1024];

    strcpy(msg, pMsg);

    i = 0;
    while(i < strlen(msg)){
        if(isdigit(msg[i])){
            isNumerico = True;
        }
        else{
            if(i == 0 && msg[i] == '-'){
                isNumerico = True;
            }
            else if(msg[i] == '.'){
                isNumerico = True;
            }
            else{
                isNumerico = False;
                i = strlen(msg);
            }
        }
        i++;
    }

    return isNumerico;
}

void showMsgBox(Datos dat_padre, Datos dat_control_foco, char *pMsg){
    int         x, y, ancho, alto;
    int         x0, y0;
    int         len_msg;
    XFontStruct *xfs;

    xfs = XLoadQueryFont(dpy, FONT_N_B);
    x0  = (int)(dat_padre.ancho / 2);
    y0  = (int)(dat_padre.alto / 2);

    //
    // Calculamos los valores
    //
    len_msg = strlen(pMsg);
    ancho   = XTextWidth(xfs, pMsg, len_msg) * 2;
    alto    = 200;
    x       = (x0 - (ancho / 2));
    y       = y0;

    //
    // Creamos la ventana
    //
    dat_msg_box.id = XCreateSimpleWindow(dpy,
                                    dat_padre.id,
                                    x,
                                    y,
                                    ancho,
                                    alto,
                                    0,
                                    negro,
                                    gris_claro);

    //
    // Establecemos los tipos de eventos que queremos en la ventana principal
    //
    XSelectInput(dpy, dat_msg_box.id, ExposureMask | ButtonPressMask | KeyPressMask);

    //
    // Creamos el gc
    //
    dat_msg_box.gc = XCreateGC(dpy, dat_msg_box.id, 0, 0);

    //
    // Guardamos los valores en dat_msg_box
    //
    dat_msg_box.padre       = dat_control_foco.id;
    dat_msg_box.xfs         = xfs;

    dat_msg_box.x           = x;
    dat_msg_box.y           = y;
    dat_msg_box.ancho       = ancho;
    dat_msg_box.alto        = alto;

    strcpy(dat_msg_box.msg, pMsg);
    dat_msg_box.is_enabled  = True;

    XMapRaised(dpy, dat_msg_box.id);

    //
    // Mostramos el mensaje
    //
    pintaMsgBox();
}

void hideMsgBox(){

    dat_msg_box.is_enabled = False;

    XFreeGC(dpy, dat_msg_box.gc);
    XUnmapWindow(dpy, dat_msg_box.id);
    XDestroyWindow(dpy, dat_msg_box.id);

    setFocusEt(dat_msg_box.padre);

    if(opt_menu == 2){
        pintaEdit();
    }
}

void pintaMsgBox(){
    int x, y, ancho, alto;

    ancho   = (int)(dat_msg_box.ancho / 2);
    alto    = (int)(dat_msg_box.alto / 3);
    x       = 100;
    y       = 75;

    setUnClick(dpy, dat_msg_box.id, dat_msg_box.gc, 2, 2, dat_msg_box.ancho - 4, dat_msg_box.alto -4);
    setTexto(dat_msg_box.id, dat_msg_box.gc, dat_msg_box.msg, dat_msg_box.xfs, rojo, x, y, ancho, alto);
    setUnClick(dpy, dat_msg_box.id, dat_msg_box.gc, dat_msg_box.ancho - 175, dat_msg_box.alto - 75, 125, 40);
    setTexto(dat_msg_box.id, dat_msg_box.gc, "Cerrar", dat_msg_box.xfs, rojo, dat_msg_box.ancho -140, dat_msg_box.alto -50, 100, 30);

}

void pintaCursor(){
    int             i;
    int             x, y, h;
    int             hay_ets;
    int             et_focused  = -1;
    int             ancho_msg;
    int             len_msg;
    int             ancho_caracter;
    unsigned long   color;

    //
    // Comprobamos si hay et activos
    //
    i           = 0;
    hay_ets     = False;
    while(i < MAX_ET){
        if(et[i].is_enabled == True){
            hay_ets = True;
            i = MAX_ET;
        }
        i++;
    }

    //
    // Si hay ets activos miramos si hay foco
    //
    et_focused = -1;
    if(hay_ets == True){
        i = 0;
        while(i <  MAX_ET){
            if(et[i].is_focused == True){
                et_focused = i;
                i = MAX_ET;
            }
            i++;
        }
    }

    //
    // Si hay et activos y ademas hay foco, pintamos la linea
    //
    if(hay_ets == True && et_focused >= 0){

        //
        // Primero el color
        //
        if(color_cursor == True){
            color = et[et_focused].color;
        }
        else{
            color = et[et_focused].back_color;
        }

        //
        // Ahora el ancho de msg
        //
        len_msg         = strlen(et[et_focused].msg_visual);
        ancho_msg       = XTextWidth(et[et_focused].xfs, et[et_focused].msg_visual, len_msg);
        if(len_msg > 0){
            ancho_caracter  = (int)(ancho_msg / len_msg);
        }
        else{
            ancho_caracter = 9;
        }

        XSetLineAttributes( dpy, et[et_focused].gc, 3, LineSolid, CapRound, JoinMiter);
        XSetForeground(dpy, et[et_focused].gc, color);

        x   = et[et_focused].x + (ancho_msg) + (int) (ancho_caracter);
        y   = et[et_focused].y + (int) (et[et_focused].alto / 2);
        h   = (int) (et[et_focused].alto / 2 ) - 5;

        XDrawLine(dpy, et[et_focused].padre, et[et_focused].gc, x, y + h, x, y - h + 3);
    }
}

unsigned long colorPorNombre( Display *dis, char *nombre ){
  XColor color, temp;

  XAllocNamedColor( dis,
                    DefaultColormap(dis,DefaultScreen(dis)),
                    nombre,
                    &color,
                    &temp );

  return( color.pixel );
}


