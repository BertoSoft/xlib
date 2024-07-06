#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>



#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
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
    int             id;
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
    int             id;
    Window          padre;
    int             x;
    int             y;
    int             ancho;
    int             alto;
    XFontStruct     *xfs;
    char            msg[1024];
    char            msg_visual[1024];
    int             is_enabled;
    int             is_focused;
    int             is_selected;
    unsigned long   color;
    unsigned long   back_color;
}DatosEt;

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

typedef struct{
int     num_directorios;
char    directorio[1024][1024];
int     num_archivos;
char    archivo[1024][1024];
}DatosDir;

//
// Variables
//
Display         *dpy;
int             scr;
DatosWindow     w[4], open, save;
DatosBoton      btn[MAX_MENU];
Atom            cerrar_ventana;
unsigned long   blanco, negro;
unsigned long   azul, azure, rojo, salmon, naranja;
unsigned long   amarillo, gris, gris_claro, marron;
unsigned long   gris_oscuro, purpura;
char            file_open[1024];
char            msg_barra_inferior[1024];
Time            old_time;

void            initUi();
void            closeUi();
void            resizeUi(XEvent ev);
void            pintaUi();
void            pintaGrafica();
void            pintaDatos();
void            menuClick(XEvent ev);
int             getMenuPulsado(XEvent ev);

DatosWindow     crearVentana(Window padre, int x, int y, int ancho, int alto, unsigned long color, unsigned long back_color);
DatosBoton      crearBoton(Window padre, int x, int y, int ancho, int alto, XFontStruct *xfs, char *msg, XImage *img);
DatosEt         crearEditText(Window padre, int x, int y, int ancho, int alto, XFontStruct *xfs, char *msg, unsigned long color, unsigned long back_color);
void            cerrarVentana(DatosWindow dww);
void            guardarDatos(char *p_ruta_archivo);
unsigned long   colorPorNombre( Display *dis, char *nombre );
void            setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setCuadro(Display *d, Window w, GC gc, unsigned long color, int x, int y, int ancho, int alto);
void            setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto);
void            setTextoEditText(DatosEt *et, char *pMsg);
void            setFechaHora();
void            setActiveWindow(Window w);
XImage          *loadImagen(Display *display, Window w, char *ruta);
Window          getWindowsActiva();
DatosStates     getWindowStates(Window w);
DatosDir        getDir(char *ruta);


#endif // UI_H_INCLUDED
