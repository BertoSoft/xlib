#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <time.h>


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

#define MAX_ET      9       // Maximo numero de editText
#define MAX_CHK     4       // maximo numero de chks


//
// Structuras
//
typedef struct{
    Window          id;
    Window          padre;
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
    int             is_cheked;
    XFontStruct     *xfs;
    char            msg[1024];
    char            msg_visual[1024];
} Datos;

//
// Declaramos los datos de los controles
//
Datos   chk[4];
Datos   et[9];

//
// Variables
//
Display         *dpy;
int             scr;
GC              gc_scr, gc_menu, gc_win, gc_inf;
Window          win_scr, win_menu, win, win_inf;
Window          win_msg_box;
GC              gc_msg_box;
Datos           dat_scr, dat_menu, dat_win, dat_inf, dat_msg_box;
unsigned long   blanco, negro;
unsigned long   azul, azure, rojo;
unsigned long   amarillo, gris, gris_claro;
unsigned long   gris_oscuro, purpura;
int             color_cursor;
int             opt_menu;

//
// Funciones
//
void            initUi();
void            resizeWin(XEvent ev);
void            closeUi();
void            hideMsgBox();
void            salir();
void            pintaUi();
void            pintaFuncion();
void            pintaMsgBox();
void            menuClick(XEvent ev);
void            msgBoxClick(XEvent ev);
void            setClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setUnClick(Display *d, Window w, GC gc, int x, int y, int ancho, int alto);
void            setTexto(Window w, GC gc, char *msg, XFontStruct *xfs, unsigned long color, int x0, int y0, int ancho, int alto);
void            setEditText(Datos txt_texto);
void            setMsgVisualEt(Window id, char *p_msg);
void            setFechaHora();
int             isNumerico(char *pMsg);
void            showMsgBox(Datos dat_padre, Datos dat_control_foco, char *pMsg);
void            pintaCursor();
XImage          *loadImagen(Display *display, Window w, char *ruta);
unsigned long   colorPorNombre( Display *dis, char *nombre );



#endif // UI_H_INCLUDED
