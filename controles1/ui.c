#include <stdio.h>
#include <string.h>
#include <time.h>


#include "ui.h"
#include "open.h"



void initUi(){
    int         i;
    XFontStruct *xfs;
    char        *icono[] = {
                            "./imagenes/abrir96.jpeg",
                            "./imagenes/guardar96.jpeg",
                            "./imagenes/edit96.png",
                            "./imagenes/math96.png",
                            "./imagenes/grafica96.png",
                            "./imagenes/salir96.jpeg"
                            };


    //
    //  Iniciamos los valores
    //
    dpy     = XOpenDisplay((char *)0);
    scr     = DefaultScreen(dpy);
    blanco  = WhitePixel(dpy, scr);
    negro   = BlackPixel(dpy, scr);

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
    // Definimos los Atoms
    //
    cerrar_ventana      = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

    //
    // Creamos las ventanas scr, menu, principal, y inferior
    //
    w[0] = crearVentana(DefaultRootWindow(dpy), 0, 0, DisplayWidth(dpy, scr), DisplayHeight(dpy, scr), negro, gris_claro);
    w[1] = crearVentana(w[0].id, w[0].ancho -103, 1, 100, w[0].alto -37, negro, gris);
    w[2] = crearVentana(w[0].id, 1, 1, w[0].ancho -107, w[0].alto -37, negro, gris_claro);
    w[3] = crearVentana(w[0].id, 0, w[0].alto -37, w[0].ancho -4, 44, negro, gris);

    //
    // Establecemos las propiedades de la ventana scr
    //
    XSetStandardProperties(dpy, w[0].id, TITULO, TITULO, None, NULL, 0, NULL);

    //
    // Mapeamos las ventanas
    //
    XMapRaised(dpy, w[0].id);
    XMapRaised(dpy, w[1].id);
    XMapRaised(dpy, w[2].id);
    XMapRaised(dpy, w[3].id);

    //
    // Creamos los botones del menu
    //
    i   = 0;
    xfs = XLoadQueryFont(dpy, FONT_N_B);
    while(i < MAX_MENU){
        btn[i] = crearBoton(w[1].id, 4, 4 +(i * ANCHO_BTN_MENU), 92, 92, xfs, "", loadImagen(dpy, w[1].id, icono[i]));
        i++;
    }

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);

}

void resizeUi(XEvent ev){

    //
    // Cambiamos las medidas
    //
    w[0].x       = ev.xconfigure.x;
    w[0].y       = ev.xconfigure.y;
    w[0].ancho   = ev.xconfigure.width;
    w[0].alto    = ev.xconfigure.height;

    //
    // Establecemos el tamaño minimo de w[0]
    //
    XSizeHints *tam_minimo = XAllocSizeHints();

    tam_minimo->flags        = PMinSize;
    tam_minimo->min_width    = ev.xconfigure.width;
    tam_minimo->min_height   = ev.xconfigure.height;
    XSetWMNormalHints(dpy, w[0].id, tam_minimo);

    XFree(tam_minimo);

    w[2].x       = 1;
    w[2].y       = 1;
    w[2].ancho   = w[0].ancho -107;
    w[2].alto    = w[0].alto -37;

    w[1].x      = w[0].ancho -103;
    w[1].y      = 1;
    w[1].ancho  = 100;
    w[1].alto   = w[0].alto -37;

    w[3].x      = 0;
    w[3].y      = w[0].alto -37;
    w[3].ancho  = w[0].ancho -4;
    w[3].alto   = 44;

}

void pintaUi(){
    int i;

    //
    // Recolocamos todas las ventanas
    //
    XMoveResizeWindow(dpy, w[2].id, w[2].x, w[2].y, w[2].ancho, w[2].alto);
    XMoveResizeWindow(dpy, w[1].id, w[1].x, w[1].y, w[1].ancho, w[1].alto);
    XMoveResizeWindow(dpy, w[3].id, w[3].x, w[3].y, w[3].ancho, w[3].alto);

    //
    // Borramos la pantalla
    //
    XClearWindow(dpy, w[2].id);

    //
    // pintamos menu
    //
    i = 0;
    while(i < MAX_MENU){
        //
        // Cargamos las imagenes
        //
        if(btn[i].img){
            XPutImage(dpy, w[1].id, w[1].gc, btn[i].img, 0, 0, 4, 4 + (i * 100), 92, 92);
        }

        //
        // Dibujamos el efecto boton
        //
        if(btn[i].is_cheked){
            setClick(dpy, w[1].id, w[1].gc, btn[i].x, btn[i].y, btn[i].ancho, btn[i].alto);
        }
        else{
            setUnClick(dpy, w[1].id, w[1].gc, btn[i].x, btn[i].y, btn[i].ancho, btn[i].alto);
        }

        i++;
    }

    //
    // pintamos la barra inf
    //
    setClick(dpy, w[3].id, w[3].gc, 2, 2, (int) (w[3].ancho * 0.7), w[3].alto - 4);                             //  70%
    setClick(dpy, w[3].id, w[3].gc, (int) (w[3].ancho * 0.7), 2, (int) (w[3].ancho * 0.2), w[3].alto - 4);      //  20%
    setClick(dpy, w[3].id, w[3].gc, (int) (w[3].ancho * 0.9), 2, (int) (w[3].ancho * 0.1), w[3].alto -4);       //  10%
    setFechaHora();

    XFlush(dpy);
}

void closeUi(){

    //
    //  Cerramos la ventana principal
    //
    cerrarVentana(w[0]);

    //
    // Cerramos la conexion
    //
    XCloseDisplay(dpy);
}

void menuClick(XEvent ev){
    int opt;

    //
    // Si se ha pulsado el menu, obtenemos el boton pulsado
    //
    opt = getMenuPulsado(ev);

    switch(opt){
        case 0:
            if(!open.is_enabled){
                showOpen();
            }
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
            closeUi();
            exit(0);
            break;
        default:
            break;
    }

    //
    // Pintamos la UI
    //
    pintaUi();
}

int getMenuPulsado(XEvent ev){
    int i;
    int x0, x1, y1, y0, x, y;
    int opt;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    i   = 0;
    opt = -1;
    while(i < MAX_MENU){

        x0 = btn[i].x;
        x1 = btn[i].x + btn[i].ancho;
        y0 = btn[i].y;
        y1 = btn[i].y + btn[i].alto;

        btn[i].is_cheked = False;
        if(x0 < x && x < x1){
            if(y0 < y && y < y1){
                btn[i].is_cheked = True;
                opt = i;
            }
        }
        i++;
    }
    return opt;
}



DatosWindow crearVentana(Window padre, int x, int y, int ancho, int alto, unsigned long color, unsigned long back_color){
    DatosWindow dww;

    dww.id = XCreateSimpleWindow(dpy,
                            padre,
                            x,
                            y,
                            ancho,
                            alto,
                            0,
                            color,
                            back_color
                            );

    dww.padre        = padre;
    dww.x            = x;
    dww.y            = y;
    dww.ancho        = ancho;
    dww.alto         = alto;
    dww.is_enabled   = True;
    dww.color        = color;
    dww.back_color   = back_color;

    XSelectInput(dpy, dww.id, ExposureMask |
                                KeyPressMask |
                                ButtonPressMask |
                                StructureNotifyMask |
                                FocusChangeMask |
                                PropertyChangeMask
                                );

    dww.gc = XCreateGC(dpy, dww.id, 0, 0);

    //
    // Definimos el Atom WM_DELETE
    //
    XSetWMProtocols(dpy, dww.id, &cerrar_ventana, 1);


    return dww;
}

DatosBoton crearBoton(Window padre, int x, int y, int ancho, int alto, XFontStruct *xfs, char *texto, XImage *img){
    DatosBoton btn;

    btn.padre   = padre;
    btn.x       = x;
    btn.y       = y;
    btn.ancho   = ancho;
    btn.alto    = alto;
    btn.img     = img;
    btn.xfs     = xfs;
    strcpy(btn.texto, texto);

    return btn;
}

void cerrarVentana(DatosWindow dww){

    XFreeGC(dpy, dww.gc);
    XUnmapWindow(dpy, dww.id);
    XDestroyWindow(dpy, dww.id);
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

void setActiveWindow(Window w){
    XClientMessageEvent ev;
    Atom                ventana_activa = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True);


    memset (&ev, 0, sizeof ev);
    ev.type = ClientMessage;
    ev.window = w;
    ev.message_type = ventana_activa;
    ev.format = 32;
    ev.data.l[0] = 1;
    ev.data.l[1] = CurrentTime;
    ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;

    XSendEvent (dpy, RootWindow(dpy, XDefaultScreen(dpy)), False, SubstructureRedirectMask |SubstructureNotifyMask, (XEvent*)&ev);

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

    x       = (int) (w[3].ancho * 0.7) + 10;
    y       = (int) (w[3].alto / 2) + 5;
    ancho   = (int) (w[3].ancho * 0.18);
    alto    = w[3].alto;

    strftime(msg, 1023, "%A, %d de %B de %Y", tm_actual);
    XSetForeground(dpy, w[3].gc, w[3].back_color);
    XFillRectangle(dpy, w[3].id, w[3].gc, (int)(w[3].ancho * 0.7) + 2, 4, ancho, 46);
    setTexto(w[3].id, w[3].gc, msg, xfs, azul, x, y, ancho, alto);

    x       = (int) (w[3].ancho * 0.9) + 50;
    y       = (int) (w[3].alto / 2) + 5;
    ancho   = (int) (w[3].ancho * 0.1);
    alto    = w[3].alto;

    strftime(msg, 1023, "%X", tm_actual);
    XSetForeground(dpy, w[3].gc, w[3].back_color);
    XFillRectangle(dpy, w[3].id, w[3].gc, (int)(w[3].ancho * 0.9) + 2, 4, ancho, 46);
    setTexto(w[3].id, w[3].gc, msg, xfs, azul, x, y, ancho, alto);

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

Window getWindowsActiva(){
    Window  ww;

    ww  = -1;

    if(w[0].is_enabled == True){ ww = w[0].id;}
    if(open.is_enabled == True){ ww = open.id;}

    return ww;
}

DatosStates getWindowStates(Window w){
    DatosStates     dStates;
    int             max_horz, max_vert;
    int             i;
    long            max_lenght  = 1024;
    Atom            atom_actual;
    int             formato_actual;
    unsigned long   bytes_despues, numero_stados = 0;
    Atom            *states = NULL;

    //
    // Este el atom de deteccion de wm_states
    //
    Atom wm_state   = XInternAtom(dpy, "_NET_WM_STATE", True);

    //
    // Atoms de los distintos estados
    //
    Atom wm_hiden   = XInternAtom(dpy, "_NET_WM_STATE_HIDDEN", True);
    Atom wm_focused = XInternAtom(dpy, "_NET_WM_STATE_FOCUSED", True);
    Atom wm_maxhorz = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
    Atom wm_maxvert = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_VERT", True);
    Atom wm_above   = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", True);
    Atom wm_below   = XInternAtom(dpy, "_NET_WM_STATE_BELOW", True);
    Atom wm_skip    = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", True);
    Atom wm_shaded  = XInternAtom(dpy, "_NET_WM_STATE_SHADED", True);


    //
    // Iniciamos todas las dStados en -1
    //
    max_horz = -1;
    max_vert = -1;

    dStates.is_hidden       = -1;
    dStates.is_focused      = -1;
    dStates.is_maximized    = -1;
    dStates.is_activa       = -1;
    dStates.is_above        = -1;
    dStates.is_below        = -1;
    dStates.is_skip         = -1;
    dStates.is_shaded       = -1;

    //
    // Obtenemos el windowsproperty para los estados _NET_WM_STATES
    //
    if(XGetWindowProperty(
        dpy,
        w,
        wm_state,
        0l,          //no offset
        max_lenght,
        False,       // no delete
        XA_ATOM,
        &atom_actual,
        &formato_actual,
        &numero_stados,
        &bytes_despues,
        (unsigned char **)&states
    ) == Success){

        i = 0;
        while(i < numero_stados){

            if(states[i] == wm_hiden){
                dStates.is_hidden = True;
            }
            else if(states[i] == wm_focused){
                dStates.is_focused = True;
            }
            else if(states[i] == wm_maxhorz ){
                max_horz = True;
            }
            else if(states[i] == wm_maxvert ){
                max_vert = True;
            }
            else if(states[i] == wm_above){
                dStates.is_above = True;
            }
            else if(states[i] == wm_below){
                dStates.is_below = True;
            }
            else if(states[i] == wm_skip){
                dStates.is_skip = True;
            }
            else if(states[i] == wm_shaded){
                dStates.is_shaded = True;
            }
            i++;
        }
        if(max_horz == True && max_vert == True){
            dStates.is_maximized = True;
        }
    }

    return dStates;
}



