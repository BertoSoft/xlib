

#include "open.h"
#include "ui.h"

//
// Funciones Graficas
//
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

    open = crearVentana(DefaultRootWindow(dpy), x, y, ancho, alto, w[2].color, gris_claro);

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
    char        ruta_actual[1024];
    int         x, y, ancho, alto;
    XFontStruct *xfs = XLoadQueryFont(dpy, FONT_N_B);

    //
    // Obtenemos la ruta inicial del programa y la copiamos en ruta, esta luego puede cambiar
    //
    if(getcwd(ruta_actual, sizeof(ruta_actual)) == NULL){
        ruta_actual[0] = '\0';
    }
    strcpy(ruta, ruta_actual);

    //
    // Fila_seleccionada = -1, Indica que no hay ningun archivo o directorio seleccionado;
    //
    fila_seleccionada = -1;

    //
    // Nombre del Archivo = nulo
    //
    archivo[0] = '\0';

    //
    // Creamos los botones Cancelar y Aceptar
    //
    x       = open.ancho - 350;
    y       = open.alto -65;
    ancho   = 150;
    alto    = 40;

    btn_open[0] = crearBoton(open.id, x, y, ancho, alto, xfs, "Cancelar", NULL);

    x       = open.ancho - 175;
    y       = open.alto -65;
    ancho   = 150;
    alto    = 40;

    btn_open[1] = crearBoton(open.id, x, y, ancho, alto, xfs, "Aceptar", NULL);
}

void showOpen(){

    initOpen();
    initControles();
    pintaOpen();
}

void pintaOpen(){

    pintaLabels();
    pintaTabla();
    pintaBotones();

}

void pintaLabels(){
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    char        msg[1024];

    xfs = XLoadQueryFont(dpy, FONT_N_B);

    //
    // Muestro en el label el nombre completo del archivo
    //
    sprintf(msg,"Archivo : ");
    strcat(msg, ruta);

    if(strlen(ruta) != 1){
        strcat(msg, "/");
    }
    strcat(msg, archivo);

    x       = 25;
    y       = 50;
    ancho   = open.ancho - 55;
    alto    = 25;

    XClearArea(dpy, open.id, x, y -20, ancho, alto, False);
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
    alto    = open.alto -175;

    //
    // Dibujamos el cuadro
    //
    XSetForeground(dpy, open.gc, blanco);
    XFillRectangle(dpy, open.id, open.gc, x, y, ancho, alto);
    setClick(dpy, open.id, open.gc, x, y, ancho, alto);

    //
    // Obtenemmos el listado del char ruta, en forma de DatosDir;
    //
    datos_dir = getDir(ruta);

    //
    // Lo ordenamos y lo recortamos si no coje
    //
    num_lineas_visibles = (int)( alto / 25) -1;

    //
    // Primero los directorios ./ y ../ , debemos distinguir si ruta != /
    //
    if(strcmp(ruta, "/") == 0){
        j = 0;
        i = 0;
    }
    else{
        sprintf(listado[0], "./");
        sprintf(listado[1], "../");
        i = 2;
        j = 0;
    }

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
    // Ahora miramos si hay seleccion, si es asi la presentamos
    //
    if(fila_seleccionada >= 0){
        x       = 27;
        y       = 67 + (int) (fila_seleccionada * 25);
        ancho   = open.ancho - 50;
        alto    = 25;

        XSetForeground(dpy, open.gc, naranja);
        XFillRectangle(dpy, open.id, open.gc, x, y, ancho, alto);
        strcpy(archivo, listado[fila_seleccionada]);
        pintaLabels();
    }

    //
    // Ahora Mostramos el listado en el cuadro
    //
    i       = 0;
    x       = 40;
    y       = 85;
    ancho   = open.ancho -75;
    alto    = 25;

    while(i < num_lineas_visibles){
        setTexto(open.id, open.gc, listado[i], xfs, negro, x, y + (int) (i * 25), ancho, alto);
        i++;
    }
}

void pintaBotones(){
    int i;

    i = 0;
    while(i < MAX_BTN_OPEN){
        setUnClick(dpy, open.id, open.gc, btn_open[i].x, btn_open[i].y, btn_open[i].ancho, btn_open[i].alto);

        setTexto(open.id,
                 open.gc,
                 btn_open[i].texto,
                 btn_open[i].xfs,
                 btn_open[i].color,
                 btn_open[i].x +40,
                 btn_open[i].y +25,
                 btn_open[i].ancho -50,
                 btn_open[i].alto -25
                 );
        i++;
    }

}

void closeOpen(){

    //
    // Colocamos el mensaje de la barra uinferior
    //
    if(strlen(file_open) > 0){
        sprintf(msg_barra_inferior, "%s", file_open);
    }
    else{
        sprintf(msg_barra_inferior, "%s", TITULO);
    }

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


//
// Gestion de eventos
//
void openButtonPress(XEvent ev){
    int x1, x2, y1, y2;
    int x,y;
    int i, fila, max_filas;
    int opt;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en la tabla obtenemos la fila seleccionada
    //
    x1          = 25;
    x2          = 25 + open.ancho -50;
    y1          = 65;
    y2          = 65 + open.alto -175;
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
    if(fila >= 0 ){
        tablaClick(fila);
    }

    //
    // Si la pulsacion es en los botones obtenemos el boton pulsado y lo clickeamos, solo clickearlo si hay seleccion
    //
    i   = 0;
    opt = -1;
    while(i < MAX_BTN_OPEN){
        x1 = btn_open[i].x;
        x2 = btn_open[i].x + btn_open[i].ancho;
        y1 = btn_open[i].y;
        y2 = btn_open[i].y + btn_open[i].alto;

        if(x1 < x && x < x2){
            if(y1 < y && y < y2){
                opt = i;
                i = MAX_BTN_OPEN;
            }
        }
        i++;
    }

    if(opt == 0){
       setClick(
                dpy,
                open.id,
                open.gc,
                btn_open[0].x,
                btn_open[0].y,
                btn_open[0].ancho,
                btn_open[0].alto
                );
    }
    if(fila_seleccionada >= 0 && opt == 1){
        setClick(
                dpy,
                open.id,
                open.gc,
                btn_open[1].x,
                btn_open[1].y,
                btn_open[1].ancho,
                btn_open[1].alto
                );
    }

}

void openButtonRelease(XEvent ev){
    int opt = -1;
    int i;
    int x1, x2, y1, y2;
    int x,y;

    //
    // Obtenemos el x e y de la pulsacion
    //
    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en los botones obtenemos el boton pulsado
    //
    i = 0;
    while(i < MAX_BTN_OPEN){
        x1 = btn_open[i].x;
        x2 = btn_open[i].x + btn_open[i].ancho;
        y1 = btn_open[i].y;
        y2 = btn_open[i].y + btn_open[i].alto;

        if(x1 < x && x < x2){
            if(y1 < y && y < y2){
                opt = i;
                i = MAX_BTN_OPEN;
            }
        }
        i++;
    }

    if(opt == 0){
        setUnClick(
                    dpy,
                    open.id,
                    open.gc,
                    btn_open[0].x,
                    btn_open[0].y,
                    btn_open[0].ancho,
                    btn_open[0].alto
                    );

        btnCancelarClick();
    }
    else if( fila_seleccionada >= 0 && opt == 1){
        setUnClick(
                    dpy,
                    open.id,
                    open.gc,
                    btn_open[1].x,
                    btn_open[1].y,
                    btn_open[1].ancho,
                    btn_open[1].alto
                    );

        btnACeptarClick();
    }


}

void openDoubleClick(XEvent ev){
    int x, y;
    int x1, x2, y1, y2;
    int i, fila, max_filas;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en la tabla obtenemos la fila seleccionada
    //
    x1          = 25;
    x2          = 25 + open.ancho -50;
    y1          = 65;
    y2          = 65 + open.alto -175;
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
    if(fila >= 0 ){
        tablaDoubleClick(fila);
    }
}



void tablaClick(int fila){

    fila_seleccionada = fila;
    pintaTabla();
}

void tablaDoubleClick(int fila){

    btnACeptarClick();
}

void btnACeptarClick(){
    char    msg[1024];
    int     i;

    //
    // Si estamos ante un archivo
    //
    if(fila_seleccionada >= 0 && archivo[strlen(archivo) -1] != '/' && strlen(ruta) != 1){
        strcpy(msg, ruta);
        strcat(msg, "/");
        strcat(msg, archivo);
        strcpy(file_open, msg);

        //
        // Colocamos la ruta del archivo en la barra inferior
        //
        sprintf(msg_barra_inferior, "%s", file_open);

        closeOpen();
    }
    //
    // Si estamos ante un directorio
    //
    else if(fila_seleccionada >= 0 && archivo[strlen(archivo) -1] == '/'){

        //
        // Si es ./, no hacemos nada, borramos la seleccion y repintamos
        //
        if(strcmp(archivo, "./") == 0){
            fila_seleccionada = -1;
            pintaTabla();
        }
        //
        // Si es ../ , bajamos un nivel
        //
        else if(strcmp(archivo, "../") == 0){
            strcpy(msg, ruta);
            i = strlen(msg);
            msg[strlen(msg) -1] = '\0';
            while(i >= 0){
                if(msg[i] == '/'){
                    msg[i] = '\0';
                    i = 0;
                }
                else{
                    msg[i] = '\0';
                }
                i--;
            }
            strcpy(ruta, msg);
            fila_seleccionada = -1;
            archivo[0] = '\0';
            pintaOpen();
        }
        //
        // cualquier otro caso cambiamos ruta y repintamos
        //
        else{
            strcpy(msg, ruta);
            if(strlen(ruta) != 1){
                strcat(msg, "/");
            }
            strcat(msg, archivo);
            msg[strlen(msg) -1] = '\0';
            strcpy(ruta, msg);
            fila_seleccionada = -1;
            archivo[0] = '\0';
            pintaOpen();
        }

    }
    //
    // Cualquier otro caso, que no haya seleccion
    //
    else{
        fila_seleccionada   = -1;
        archivo[0]          = '\0';
        ruta[0]             = '\0';
    }


}

void btnCancelarClick(){

    //
    // Colocamos seleccion = -1, y archivo = \0 y ruta =\0 y cerramos
    //
    fila_seleccionada   = -1;
    archivo[0]          = '\0';
    ruta[0]             = '\0';

    closeOpen();

}
