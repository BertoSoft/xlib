#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "ui.h"

#define MAX_MENU    6       // Maximo de botones del menu
#define BTN_MENU    100     // Ancho de los botones del menu

void initUi(){
    int             ancho_scr, alto_scr;

    //
    // iniciamos las variables
    //
    dpy             = XOpenDisplay((char *)0);
    scr             = DefaultScreen(dpy);
    blanco          = WhitePixel(dpy, scr);
    negro           = BlackPixel(dpy, scr);
    ancho_scr       = DisplayWidth(dpy, scr);
    alto_scr        = DisplayHeight(dpy, scr);

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
                                azul);

    dat_menu.id            = win_menu;
    dat_menu.x             = ancho_scr -90 -100;
    dat_menu.y             = 0;
    dat_menu.ancho         = 100;
    dat_menu.alto          = alto_scr -60 -50;
    dat_menu.borde         = 0;
    dat_menu.color         = negro;
    dat_menu.back_color    = azul;

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


    //
    // Establecemos las propiedades de win
    //
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

    //
    // Establecemos las propiedades de win_inf
    //
    dat_inf.id          = win_inf;
    dat_inf.x           = 0;
    dat_inf.y           = alto_scr -108;
    dat_inf.ancho       = ancho_scr -90 -100;
    dat_inf.alto        = 43;
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

    dat_scr.x       = ev.xconfigure.x;
    dat_scr.y       = ev.xconfigure.y;
    dat_scr.ancho   = ev.xconfigure.width;
    dat_scr.alto    = ev.xconfigure.height;

    dat_win.x       = 1;
    dat_win.y       = 1;
    dat_win.ancho   = dat_scr.ancho -107;
    dat_win.alto    = dat_scr.alto -47;

    dat_menu.x      = dat_scr.ancho -103;
    dat_menu.y      = 1;
    dat_menu.ancho  = 100;
    dat_menu.alto   = dat_scr.alto -48;

    dat_inf.x      = 0;
    dat_inf.y      = dat_scr.alto -47;
    dat_inf.ancho  = dat_scr.ancho -4;
    dat_inf.alto   = 44;

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

    XMapRaised(dpy, win_scr);
    XFlush(dpy);
}

void menuClick(XEvent ev){
    int i;

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
            break;
        case 3:
            break;
        case 4:
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
    XSetForeground( d, gc, gris_claro);
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
    XSetForeground( d, gc, gris_claro);
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

    tam_msg_pixels  = XTextWidth(xfs, msg, strlen(msg));
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

void setEditText(Window w, GC gc, XFontStruct *xfs, Datos txt_texto){
    int     x   = txt_texto.x +5;
    int     y   = txt_texto.y -(int) (txt_texto.alto / 2) - 10;
    int     i, j;
    int     ancho_msg, max_caracters, ancho_caracter, len_msg;
    char    msg_visual[1024];

    //
    // Dibujamos el cuadro blanco
    //
    XSetForeground(dpy, gc, blanco);
    XFillRectangle(dpy, w, gc, x, y, txt_texto.ancho, txt_texto.alto);

    //
    // Obtenemos los max caracteres a mostrar, y le restamos 2
    //
    len_msg     = strlen(txt_texto.msg);
    ancho_msg   = XTextWidth(xfs, txt_texto.msg, len_msg);

    if(ancho_msg > 0){
        ancho_caracter  = (int) (ancho_msg / len_msg);
        max_caracters   = (int) (txt_texto.ancho / ancho_caracter);
    }
    else{
        ancho_caracter  = 0;
        max_caracters   = 0;
    }
    max_caracters -= 2;
    if(max_caracters < 0) max_caracters = 0;
    if(max_caracters > 1022) max_caracters = 1022;

    //
    // Ahora si el texto es muy grande, mostramos solo el final
    //
    if(len_msg > 0 && len_msg >= max_caracters){
        i = max_caracters -1;
        j = 0;
        msg_visual[max_caracters +1]   = '\0';
        while(j < max_caracters){
            msg_visual[i] = txt_texto.msg[len_msg -1 -j];
            i--;
            j++;
        }
    }
    else{
        strcpy(msg_visual, txt_texto.msg);
    }

    setClick(dpy, w, gc, x, y, txt_texto.ancho, txt_texto.alto);
    setTexto(w, gc, msg_visual, txt_texto.xfs, txt_texto.color, x +8, txt_texto.y, txt_texto.ancho, txt_texto.alto);
}

void setFechaHora(){
    char        msg[1024];
    int         x, y, ancho, alto;
    XFontStruct *xfs;
    time_t      time_actual;
    struct tm   *tm_fecha;


    time_actual     = time(NULL);
    tm_fecha        = localtime(&time_actual);
    xfs             = XLoadQueryFont(dpy, FONT_N_B);

    x       = (int) (dat_inf.ancho * 0.7) + 10;
    y       = (int) (dat_inf.alto / 2) + 10;
    ancho   = (int) (dat_inf.ancho * 0.18);
    alto    = dat_inf.alto;

    strftime(msg, 1023, "%A, %d de %B de %Y", tm_fecha);
    XSetForeground(dpy, gc_inf, dat_inf.back_color);
    XFillRectangle(dpy, win_inf, gc_inf, (int)(dat_inf.ancho * 0.7) + 2, 4, ancho, 46);
    XSetForeground(dpy, gc_inf, dat_inf.color);
    setTexto(win_inf, gc_inf, msg, xfs, negro, x, y, ancho, alto);

    x       = (int) (dat_inf.ancho * 0.9) + 50;
    y       = (int) (dat_inf.alto / 2) + 10;
    ancho   = (int) (dat_inf.ancho * 0.1);
    alto    = dat_inf.alto;

    strftime(msg, 1023, "%X", tm_fecha);
    XSetForeground(dpy, gc_inf, dat_inf.back_color);
    XFillRectangle(dpy, win_inf, gc_inf, (int)(dat_inf.ancho * 0.9) + 2, 4, ancho, 46);
    XSetForeground(dpy, gc_inf, dat_inf.color);

    setTexto(win_inf, gc_inf, msg, xfs, negro, x, y, ancho, alto);


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


