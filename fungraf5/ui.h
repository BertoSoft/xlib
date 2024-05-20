#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED



#define TITULO              "Funciones Graficas V1.0"
#define ANCHO_MINIMO        1000
#define ALTO_MINIMO         700
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

//
// Structuras
//
typedef struct{
    Window          id;
    GC              gc;
    char            nombre[1024];
    int             x;
    int             y;
    int             ancho;
    int             alto;
    int             borde;
    unsigned long   color;
    unsigned long   back_color;
    int             is_focused;
    int             is_enabled;
    XFontStruct     *xfs;
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
Window          win_guardar;
Window          win_math;
Datos           dat_scr, dat_lanzadera, dat_win_abrir, dat_win_edit, dat_win_math, dat_win_graf, dat_win_guardar;
GC              gc_scr;
GC              gc_lanzadera;
GC              gc_graf;
GC              gc_edit;
GC              gc_abrir;
GC              gc_math;
unsigned long   blanco, negro;
unsigned long   azul, azure, rojo, amarillo, gris, gris_claro, gris_oscuro, purpura;
int             ancho_pantalla, alto_pantalla;
int             opt_lanzadera;
int             opt_funcion;
char            old_focus_edit[1024];
char            focus[1024];
int             polinomio[5];


//
// Funciones
//
void            initUi();
void            initWindows();
void            initControls();
void            closeUi();
void            refreshFocus();
void            refreshUi();
void            refreshEdit();
void            pintaChkFunciones();
void            pintaPolinomicas();
char            *polinomioToChar();
void            pintaTrigonometricas();
void            pintaExponenciales();
void            pintaLogaritmicas();
void            pintaEcuacion();
void            resizeWin(XEvent ev);
void            salir();
unsigned long   colorPorNombre(Display *dis, char *nombre);                                 // Devuelve el valor long del color a traves del nombre del color
XImage          *loadImagen(Display *display, Window w, char *ruta);                        // Carga una imagen del disco en un *XImage
void            winLanzaderaClick(XEvent ev);
void            winEditClick(XEvent ev);
void            winEditKeyPress(XEvent ev);
void            winLanzaderaKeyPress(XEvent ev);
void            setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto);
void            setEditText(Window w, GC gc, XFontStruct *xfs, Datos txt_texto);
void            setCursor(Datos txt);
void            setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);   // Crea efecto hundido
void            setUnClick(Display *d,Window w, GC gc, int x, int y, int ancho, int alto);  // Crea efecto elevado
void            setFocus(char *nombre);
void            setOldFocus(Window w, char *nombre);
char            *getFocus();
char            *getOldFocus(Window w);

#endif // UI_H_INCLUDED
