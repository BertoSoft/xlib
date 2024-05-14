#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED



#define TITULO              "Funciones Graficas V1.0"
#define MAX_OPT_LANZADERA   5
#define ANCHO_MINIMO        1000
#define ALTO_MINIMO         700
#define BTN_LANZADERA       100
#define ESC                 27


//
// Structuras
//
typedef struct{
    Window          id;
    int             x;
    int             y;
    int             ancho;
    int             alto;
    int             borde;
    unsigned long   color;
    unsigned long   back_color;
    int             is_focused;
    int             is_cheked;
    char            msg[1024];
} Datos;



//
// Variables
//
Display         *dpy;
int             scr;
Window          win_scr;
Window          win_lanzadera;
Window          win_graf;
Window          win_edit;
Window          win_abrir;
Window          win_math;
GC              gc_scr;
GC              gc_lanzadera;
GC              gc_graf;
GC              gc_edit;
GC              gc_abrir;
GC              gc_math;
unsigned long   blanco, negro;
unsigned long   azul, azure, rojo, amarillo, gris, gris_claro, gris_oscuro, purpura;
int             ancho_pantalla, alto_pantalla;
Datos           dat_scr, dat_lanzadera, dat_win_abrir, dat_win_edit, dat_win_math, dat_win_graf;
int             opt_lanzadera;
int             opt_funcion;
char            focus[1024];


//
// Funciones
//
void            initUi();
void            closeUi();
void            refreshUi();
void            refreshEdit();
void            pintaChkFunciones();
void            pintaPolinomicas();
void            pintaTrigonometricas();
void            pintaExponenciales();
void            pintaLogaritmicas();
void            resizeWin(XEvent ev);
void            salir();
unsigned long   colorPorNombre(Display *dis, char *nombre);                                 // Devuelve el valor long del color a traves del nombre del color
XImage          *loadImagen(Display *display, Window w, char *ruta);                        // Carga una imagen del disco en un *XImage
void            setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);   // Crea efecto hundido
void            setUnClick(Display *d,Window w, GC gc, int x, int y, int ancho, int alto);  // Crea efecto elevado
void            win_lanzadera_click(XEvent ev);
void            win_edit_click(XEvent ev);
void            setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, int x0, int y0, int ancho, int alto);

#endif // UI_H_INCLUDED
