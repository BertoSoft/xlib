#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <Imlib2.h>

#define TITULO "Controles V 1.0"

#define ESC                 9
#define ENTER               36
#define RUN                 104
#define BACK                22
#define DEL                 119
#define ARRIBA              111
#define ABAJO               116
#define IZQUIERDA           113
#define DERECHA             114
#define KEY_1               87
#define KEY_2               88
#define KEY_3               89
#define KEY_4               83
#define KEY_5               84
#define KEY_6               85
#define KEY_7               79
#define KEY_8               80
#define KEY_9               81
#define KEY_0               90
#define KEY_DIV             106
#define KEY_MUL             63
#define KEY_SUM             86
#define KEY_RES             82
#define KEY_MAY             50
#define KEY_PTO             91
#define FONT_G              "rk24"
#define FONT_N_B            "9x15bold"
#define FONT_N              "9x15"

#define MAX_MENU        6       // Numero de botones del menu
#define ANCHO_BTN_MENU  100     // Ancho de los botones del menu


//
// Estructuras
//
typedef struct{
    Window          id;
    Window          padre;
    GC              gc;
    int             x;
    int             y;
    int             ancho;
    int             alto;
    int             is_enabled;
    unsigned long   color;
    unsigned long   back_color;
}DatosWindow;

typedef struct{
    Window          padre;
    int             x;
    int             y;
    int             ancho;
    int             alto;
    int             is_cheked;
    XImage          *img;
    XFontStruct     *xfs;
    char            texto[1024];
    unsigned long   color;
    unsigned long   back_color;
}DatosBoton;

typedef struct{
    int is_focused;
    int is_hidden;
    int is_maximized;
    int is_activa;
    int is_above;
    int is_below;
    int is_skip;
    int is_shaded;
}DatosStates;

//
// Variables
//
Display         *dpy;
int             scr;
DatosWindow     w[4], open;
DatosBoton      btn[MAX_MENU];
Atom            cerrar_ventana;
unsigned long   blanco, negro;
unsigned long   azul, azure, rojo;
unsigned long   amarillo, gris, gris_claro;
unsigned long   gris_oscuro, purpura;
int             contador;

void            initUi();
void            closeUi();
void            resizeUi(XEvent ev);
void            pintaUi();
void            menuClick(XEvent ev);
int             getMenuPulsado(XEvent ev);

DatosWindow     crearVentana(Window padre, int x, int y, int ancho, int alto, unsigned long color, unsigned long back_color);
DatosBoton      crearBoton(Window padre, int x, int y, int ancho, int alto, XFontStruct *xfs, char *msg, XImage *img);
void            cerrarVentana(DatosWindow dww);
unsigned long   colorPorNombre( Display *dis, char *nombre );
void            setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto);
void            setFechaHora();
void            setActiveWindow(Window w);
XImage          *loadImagen(Display *display, Window w, char *ruta);
Window          getWindowsActiva();
DatosStates     getWindowStates(Window w);

#endif // UI_H_INCLUDED
