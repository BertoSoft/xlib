#include <dirent.h>
#include <unistd.h>

#include "open.h"
#include "ui.h"

void initOpen(){
    int x, y, ancho, alto;

    //
    // Colocamos la ventana principal como inactiva
    //
    w[0].is_enabled = False;

    //
    // Creamos la ventana
    //
    ancho   = 800;
    alto    = 600;
    x       = (int)(DisplayWidth(dpy, scr) / 2) - (ancho / 2);
    y       = (int)(DisplayHeight(dpy, scr) / 2) -(alto / 2);

    open = crearVentana(DefaultRootWindow(dpy), x, y, ancho, alto, negro, gris_claro);

    //
    // Establecemos las propiedades de la ventana
    //
    XSetStandardProperties(dpy, open.id, TITULO, TITULO, None, NULL, 0, NULL);

    //
    // Establecemos el tamaño fijo de open
    //
    XSizeHints *tam = XAllocSizeHints();

    tam->flags        = PMinSize | PMaxSize;
    tam->min_width    = tam->max_height = ancho;
    tam->min_height   = tam->max_height = alto;

    XSetWMNormalHints(dpy, open.id, tam);
    XFree(tam);

    //
    // La mepeamos
    //
    XMapRaised(dpy, open.id);

    //
    // La centramos
    //
    XMoveResizeWindow(dpy, open.id, x, y, ancho, alto);

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);
}

void initControles(){
    char    ruta_actual[1024];

    //
    // Obtenemos la ruta inicial del programa y la copiamos en ruta, esta luego puede cambiar
    //
    if(getcwd(ruta_actual, sizeof(ruta_actual)) == NULL){
        ruta_actual[0] = '\0';
    }
    strcpy(ruta, ruta_actual);

    //
    // Fila_seleccionada = -1;
    //
    fila_seleccionada = -1;
}

void showOpen(){

    initOpen();
    initControles();
    pintaOpen();
}

void pintaOpen(){

    pintaLabels();
    pintaTabla();
    //pintaBotonoes();

}

void pintaLabels(){
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    char        msg[1024];

    xfs = XLoadQueryFont(dpy, FONT_N_B);
    sprintf(msg,"Nombre del archivo ");
    x       = 25;
    y       = 50;
    ancho   = 195;
    alto    = 25;

    setTexto(open.id, open.gc, msg, xfs, open.color, x, y, ancho, alto);

}

void pintaTabla(){
    int             i, j;
    int             x, y, ancho, alto;
    int             num_lineas_visibles, num_lineas_totales;
    DatosDir        datos_dir;
    char            listado[1024][1024];
    char            msg[1024];
    XFontStruct     *xfs = XLoadQueryFont(dpy, FONT_N_B);

    x       = 25;
    y       = 65;
    ancho   = open.ancho - 50;
    alto    = open.alto -200;

    //
    // Dibujamos el cuadro
    //
    XSetForeground(dpy, open.gc, blanco);
    XFillRectangle(dpy, open.id, open.gc, x, y, ancho, alto);
    setClick(dpy, open.id, open.gc, x, y, ancho, alto);

    //
    // Obtenemmos el listado del char ruta, en forma de DatosDir;
    //
    datos_dir = getListado(ruta);

    //
    // Lo ordenamos y lo recortamos si no coje
    //
    num_lineas_visibles = (int)( alto / 25) -1;

    //
    // Primero los directorios
    //
    sprintf(listado[0], "./");
    sprintf(listado[1], "../");
    i = 2;
    j = 0;
    while(j < datos_dir.num_directorios){
        sprintf(msg,"%s", datos_dir.directorio[j]);
        strcat(msg, "/");

        if(strcmp(msg, "./") != 0 && strcmp(msg, "../") != 0){
            strcpy(listado[i], msg);
            i++;
        }
        j++;
    }

    //
    // Ahora los archivos
    //
    j = 0;
    while(j < datos_dir.num_archivos){
        strcpy(listado[i], datos_dir.archivo[j]);
        i++;
        j++;
    }

    //
    // Ahora obtenemos el maximo de lineas a mostrar
    //
    num_lineas_totales = datos_dir.num_directorios + datos_dir.num_archivos;
    if(num_lineas_totales < num_lineas_visibles){
        num_lineas_visibles = num_lineas_totales;
    }

    //
    // Ahorra miramos si hay seleccion
    //
    if(fila_seleccionada >= 0){
        x       = 27;
        y       = 67 + (int) (fila_seleccionada * 25);
        ancho   = open.ancho - 50;
        alto    = 25;

        XSetForeground(dpy, open.gc, amarillo);
        XFillRectangle(dpy, open.id, open.gc, x, y, ancho, alto);
    }

    //
    // Ahora pintamos en el cuadro
    //
    i       = 0;
    x       = 40;
    y       = 85;
    ancho   = open.ancho -75;
    alto    = 25;

    while(i <= num_lineas_visibles){
        setTexto(open.id, open.gc, listado[i], xfs, open.color, x, y + (int) (i * 25), ancho, alto);
        i++;
    }
}

void pintaBotones(){

}

void closeOpen(){

    //
    // Cerramos open
    //
    cerrarVentana(open);
    open.is_enabled     = False;

    //
    // Si w[0] esta minimizad la traemos al frente
    //
    DatosStates states_w0 = getWindowStates(w[0].id);
    if(states_w0.is_hidden){
        setActiveWindow(w[0].id);
    }

    //
    // Activamos w[0], y repintamos el menu
    //
    w[0].is_enabled     = True;
    btn[0].is_cheked    = False;
    pintaUi();
}

void openClick(XEvent ev){
    int x1, x2, y1, y2;
    int x,y;
    int i, fila, max_filas;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en la tabla obtenemos la fila seleccionada
    //
    x1          = 25;
    x2          = 25 + open.ancho -50;
    y1          = 65;
    y2          = 65 + open.alto -200;
    fila        = -1;
    max_filas   = (int)((y2 - y1)/ 25);
    i           = 0;

    //
    // Miramos si esta en la tabla
    //
    if(x1 < x && x < x2){
        if(y1 < y && y < y2){

            //
            //Estamos dentro de la tabla buscamos la fila
            //
            y2 = y1 + 25;
            while(i < max_filas){
                if(y1 < y && y < y2){

                    //
                    // Tenemos la fila terminamos
                    //
                    fila = i;
                    i = max_filas;
                }
                y1 = y2;
                y2 = y1 + 25;
                i++;
            }
        }
    }

    //
    // Si fila > 0 , llamamos a tablaClick();
    //
    if(fila >= 0){
        tablaClick(fila);
    }

    //
    // Si la pulsacion es en el boton
    //


}

void tablaClick(int fila){

    fila_seleccionada = fila;
    pintaTabla();
}

void tablaDoubleClick(XEvent ev){

}

DatosDir getListado(char *ruta){
    DatosDir        listado;
    DIR             *pDir = opendir(ruta);
    struct dirent   *dp;

    //
    // No se puede leer el directorio, devolvemos nulo
    //
    if(!pDir){
        listado.num_archivos    = -1;
        listado.num_directorios = -1;
    }
    //
    // se abrio el directorio, procedemos a leerlo
    //
    else{
        listado.num_archivos    = 0;
        listado.num_directorios = 0;
        while((dp = readdir(pDir)) != NULL){
            if(dp->d_type == 4){
                strcpy(listado.directorio[listado.num_directorios], dp->d_name);
                listado.num_directorios++;
            }
            if(dp->d_type == 8){
                strcpy(listado.archivo[listado.num_archivos], dp->d_name);
                listado.num_archivos++;
            }
        }
    }

    return listado;
}

