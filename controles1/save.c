
#include "save.h"
#include "ui.h"

void initSave(){
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

    save = crearVentana(DefaultRootWindow(dpy), x, y, ancho, alto, w[2].color, gris_claro);

    //
    // Establecemos las propiedades de la ventana
    //
    XSetStandardProperties(dpy, save.id, TITULO, TITULO, None, NULL, 0, NULL);

    //
    // Establecemos el tamaño fijo de open
    //
    XSizeHints *tam = XAllocSizeHints();

    tam->flags        = PMinSize | PMaxSize;
    tam->min_width    = tam->max_height = ancho;
    tam->min_height   = tam->max_height = alto;

    XSetWMNormalHints(dpy, save.id, tam);
    XFree(tam);


    //
    // La mepeamos
    //
    XMapRaised(dpy, save.id);

    //
    // La centramos
    //
    XMoveResizeWindow(dpy, save.id, x, y, ancho, alto);

    //
    // Vaciamos el buffer grafico
    //
    XFlush(dpy);
}

void initControlesSave(){
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    int         x0;
    char        ruta_actual[1024];

    xfs = XLoadQueryFont(dpy, FONT_N_B);
    x0 = (int) (save.ancho / 2);

    //
    // Creamos el editText etArchivo
    //
    x       = x0 -x0 +235;
    y       = 40;
    ancho   = 350;
    alto    = 25;

    et_archivo  = crearEditText(save.id, x, y, ancho, alto, xfs, "", negro, blanco);

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
    x       = save.ancho - 350;
    y       = save.alto -65;
    ancho   = 150;
    alto    = 40;

    btn_save[0] = crearBoton(save.id, x, y, ancho, alto, xfs, "Cancelar", NULL);

    x       = save.ancho - 175;
    y       = save.alto -65;
    ancho   = 150;
    alto    = 40;

    btn_save[1] = crearBoton(save.id, x, y, ancho, alto, xfs, "Guardar", NULL);

}

void showSave(){

    initSave();
    initControlesSave();
    setTextoEditText(&et_archivo, "grafica.cfg");
    et_archivo.is_selected = True;
    sprintf(archivo, "grafica.cfg");
    pintaSave();
}

void closeSave(){

    //
    // Cerramos Save
    //
    cerrarVentana(save);
    save.is_enabled     = False;

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
    btn[1].is_cheked    = False;
    pintaUi();
}

void pintaSave(){

    pintaLabelsSave();
    pintaEtsSave();
    pintaTablaSave();
    pintaBotonesSave();
}

void pintaLabelsSave(){
    XFontStruct *xfs;
    int         x, y, ancho, alto;
    int         x0;
    char        msg[1024];

    xfs = XLoadQueryFont(dpy, FONT_N_B);
    x0 = (int) (save.ancho / 2);

    x       =  x0 -x0 +150;
    y       = 40;
    ancho   = 75;
    alto    = 25;
    sprintf(msg, "Nombre.");
    setTexto(save.id, save.gc, msg, xfs, et_archivo.color, x, y, ancho, alto);

}

void pintaEtsSave(){
    int x, y, ancho, alto;
    int ancho_caracter;

    x               = et_archivo.x;
    y               = et_archivo.y -16;
    ancho           = et_archivo.ancho;
    alto            = et_archivo.alto;
    ancho_caracter  = XTextWidth(et_archivo.xfs, "1", 1);

    //
    // Pintamos el et_Archivo
    //
    XSetForeground(dpy, save.gc, blanco);
    XFillRectangle(dpy, save.id, save.gc, x, y, ancho, alto);
    setClick(dpy, save.id, save.gc, x, y, ancho, alto);

    //
    // Ahora si hay seleccion la pintamos
    //
    if(et_archivo.is_selected){
        XSetForeground(dpy, save.gc, salmon);
        XFillRectangle(
                       dpy,
                       save.id,
                       save.gc,
                       x +5,
                       y +5,
                       (int)(ancho_caracter * strlen(et_archivo.msg_visual)),
                       alto -7
                       );
    }
    else{
        XSetForeground(dpy, save.gc, et_archivo.back_color);
        XFillRectangle(
                       dpy,
                       save.id,
                       save.gc,
                       x +5,
                       y +5,
                       (int)(ancho_caracter * strlen(et_archivo.msg_visual)),
                       alto -7
                       );
    }

    //
    // Queda por presentar el msg, si lo tiene, nosotros presentamos el msg_visual
    //
    if(et_archivo.msg_visual[0] != '\0'){
        setTexto(save.id, save.gc, et_archivo.msg_visual, et_archivo.xfs, et_archivo.color, x +7, y +18, ancho, alto);
    }

}

void pintaTablaSave(){
    int             i, j;
    int             x, y, ancho, alto;
    int             num_lineas_visibles, num_lineas_totales;
    DatosDir        datos_dir;
    char            listado[1024][1024];
    char            msg[1024];
    XFontStruct     *xfs = XLoadQueryFont(dpy, FONT_N_B);


    //
    // Creamos el dir
    //
    x       = 25;
    y       = 100;
    ancho   = save.ancho - 50;
    alto    = save.alto -200;

    XSetForeground(dpy, save.gc, blanco);
    XFillRectangle(dpy, save.id, save.gc, x, y, ancho, alto);
    setClick(dpy, save.id, save.gc, x, y, ancho, alto);

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
        y       = 102 + (int) (fila_seleccionada * 25);
        ancho   = save.ancho - 50;
        alto    = 25;

        XSetForeground(dpy, save.gc, naranja);
        XFillRectangle(dpy, save.id, save.gc, x, y, ancho, alto);

        //
        // Si es archivo los copiamos en archivo, y en el msg del et_archivo, si es directorio solo en archivo
        //
        if(listado[fila_seleccionada][strlen(listado[fila_seleccionada]) -1] != '/'){
            strcpy(archivo, listado[fila_seleccionada]);
            setTextoEditText(&et_archivo, archivo);

            et_archivo.is_focused   = False;
            et_archivo.is_selected  = True;
        }
        else{
            strcpy(archivo, listado[fila_seleccionada]);
        }
        pintaEtsSave();
    }

    //
    // Ahora Mostramos el listado en el cuadro
    //
    i       = 0;
    x       = 40;
    y       = 120;
    ancho   = save.ancho -75;
    alto    = 25;

    while(i < num_lineas_visibles){
        setTexto(save.id, save.gc, listado[i], xfs, negro, x, y + (int) (i * 25), ancho, alto);
        i++;
    }
}

void pintaBotonesSave(){
    int i;

    i = 0;
    while(i < MAX_BTN_SAVE){
        setUnClick(dpy, save.id, save.gc, btn_save[i].x, btn_save[i].y, btn_save[i].ancho, btn_save[i].alto);

        setTexto(save.id,
                 save.gc,
                 btn_save[i].texto,
                 btn_save[i].xfs,
                 btn_save[i].color,
                 btn_save[i].x +40,
                 btn_save[i].y +25,
                 btn_save[i].ancho -50,
                 btn_save[i].alto -25
                 );
        i++;
    }
}


//
// Funciones de eventos
//
void saveButtonPress(XEvent ev){
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
    x2          = 25 + save.ancho -50;
    y1          = 100;
    y2          = 100 + save.alto -200;
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
        saveTablaClick(fila);
    }

    //
    // Si la pulsacion es en el et_archivo, activamos la seleccion
    //
    x1  = et_archivo.x;
    x2  = et_archivo.x + et_archivo.ancho;
    y1  = et_archivo.y -16;
    y2  = et_archivo.y -16 + et_archivo.alto;
    if(x1 < x && x < x2){
        if(y1 < y && y < y2){
            saveEtClick();
        }
    }


    //
    // Si la pulsacion es en los botones obtenemos el boton pulsado y lo clickeamos, solo clickearlo si hay seleccion
    //
    i   = 0;
    opt = -1;
    while(i < MAX_BTN_SAVE){
        x1 = btn_save[i].x;
        x2 = btn_save[i].x + btn_save[i].ancho;
        y1 = btn_save[i].y;
        y2 = btn_save[i].y + btn_save[i].alto;

        if(x1 < x && x < x2){
            if(y1 < y && y < y2){
                opt = i;
                i = MAX_BTN_SAVE;
            }
        }
        i++;
    }

    if(opt == 0){
       setClick(
                dpy,
                save.id,
                save.gc,
                btn_save[0].x,
                btn_save[0].y,
                btn_save[0].ancho,
                btn_save[0].alto
                );
    }
    if(fila_seleccionada >= 0 && opt == 1){
        setClick(
                dpy,
                save.id,
                save.gc,
                btn_save[1].x,
                btn_save[1].y,
                btn_save[1].ancho,
                btn_save[1].alto
                );
    }


}

void saveButtonRelease(XEvent ev){
    int x1, x2, y1, y2;
    int x,y;
    int i;
    int opt;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en los botones obtenemos el boton pulsado y lo clickeamos, solo clickearlo si hay seleccion
    //
    i   = 0;
    opt = -1;
    while(i < MAX_BTN_SAVE){
        x1 = btn_save[i].x;
        x2 = btn_save[i].x + btn_save[i].ancho;
        y1 = btn_save[i].y;
        y2 = btn_save[i].y + btn_save[i].alto;

        if(x1 < x && x < x2){
            if(y1 < y && y < y2){
                opt = i;
                i = MAX_BTN_SAVE;
            }
        }
        i++;
    }

    if(opt == 0){
       setUnClick(
                dpy,
                save.id,
                save.gc,
                btn_save[0].x,
                btn_save[0].y,
                btn_save[0].ancho,
                btn_save[0].alto
                );

        saveBtnCancelarClick();
    }

    if(fila_seleccionada >= 0 && opt == 1){
        setUnClick(
                dpy,
                save.id,
                save.gc,
                btn_save[1].x,
                btn_save[1].y,
                btn_save[1].ancho,
                btn_save[1].alto
                );

        saveBtnGuardarClick();
    }


}

void saveDoubleClick(XEvent ev){
    int x1, x2, y1, y2;
    int x,y;
    int i, fila, max_filas;

    x = ev.xbutton.x;
    y = ev.xbutton.y;

    //
    // Si la pulsacion es en la tabla obtenemos la fila seleccionada
    //
    x1          = 25;
    x2          = 25 + save.ancho -50;
    y1          = 100;
    y2          = 100 + save.alto -200;
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
        saveTablaDoubleClick(fila);
    }

}

void saveKeyPress(XEvent ev){
    char msg[1024];

    //
    // Si la pulsacion es REturn
    //
    if(ev.xkey.keycode == ENTER || ev.xkey.keycode == RUN){
        saveEtReturnPressed();
    }
    //
    // Si la pulsacion es delete
    //
    else if(ev.xkey.keycode == BACK){

        //
        // Si esta selected
        //
        if(et_archivo.is_selected){
            et_archivo.is_selected  = False;
            et_archivo.is_focused   = True;
            fila_seleccionada       = -1;
            msg[0]                  = '\0';

            setTextoEditText(&et_archivo, msg);
            pintaSave();
        }
        //
        // Si no esta selected
        //
        else{
            strcpy(msg, et_archivo.msg);
            msg[strlen(msg) -1] = '\0';
            setTextoEditText(&et_archivo, msg);
            pintaEtsSave();
        }
    }
    //
    // Culaquiera otra pulsacion
    //
    else{
        saveEtKeyPress(ev.xkey.keycode);
    }
}

//
// Metodos del formulario
//
void saveTablaClick(int fila){

    fila_seleccionada = fila;
    pintaTablaSave();

}

void saveTablaDoubleClick(int fila){

    saveBtnGuardarClick();
}

void saveEtReturnPressed(){

    saveBtnGuardarClick();
}

void saveEtKeyPress(unsigned int key_code){
    char msg[1024];
    char msg1[1024];
    char ch;

    //
    // Obtenemos la tecla
    //
    ch = XkbKeycodeToKeysym(dpy, key_code, 0, 0);

    //
    // Si se encuentra selected pero no focused, le damos el foco y editamos
    //
    if(!et_archivo.is_focused && et_archivo.is_selected){

        et_archivo.is_focused   = True;
        et_archivo.is_selected  = False;
        fila_seleccionada       = -1;

        sprintf(msg, "%c", ch);
        setTextoEditText(&et_archivo, msg);
        pintaSave();
    }

    //
    // Si se encuentra focused y no esta seleccionado, sumamos lo tecleado
    //
    else if(et_archivo.is_focused && !et_archivo.is_selected){
        sprintf(msg1, "%c", ch);
        strcpy(msg, et_archivo.msg);
        strcat(msg, msg1);

        setTextoEditText(&et_archivo, msg);
        pintaEtsSave();
    }

    //
    // Si se encuentra focused y selected, le dejamos el foco y cambiamos el msg
    //
    else if(et_archivo.is_selected && et_archivo.is_focused){

        et_archivo.is_focused   = True;
        et_archivo.is_selected  = False;

        sprintf(msg, "%c", ch);
        setTextoEditText(&et_archivo, msg);
        pintaEtsSave();
    }

}

void saveEtClick(){

    //
    // Si no esta seleccionado y esta focused lo seleccionamos
    //
    if(!et_archivo.is_selected && et_archivo.is_focused){
        et_archivo.is_selected = True;
        pintaEtsSave();
    }
}

void saveBtnGuardarClick(){
    char ruta_archivo[1024];
    char msg[1024];

    //
    // Antes de nada fusionamos ruta y archivo
    //
    if(fila_seleccionada >= 0 && archivo[strlen(archivo) -1] != '/' && strlen(ruta) != 1){
        strcpy(msg, ruta);
        strcat(msg, "/");
        strcat(msg, archivo);
        strcpy(file_open, msg);
    }

    //
    // Si es un directorio, establecemos la nueva ruta
    //
    if(archivo[strlen(archivo) -1] == '/'){
        strcpy(msg, ruta);
        if(strlen(ruta) != 1){
            strcat(msg, "/");
        }
        strcat(msg, archivo);
        msg[strlen(msg) -1] = '\0';
        strcpy(ruta, msg);
        fila_seleccionada = -1;
        archivo[0] = '\0';
        setTextoEditText(&et_archivo, archivo);
        pintaSave();
    }
    //
    // Si es un archivo guardamos los datos
    //
    else{

        //
        // Si esta vacio, avisamos
        //
        if(archivo[0] == '\0'){

        }
        //
        // Si existe el archivo guardamos toda la ruta
        //
        else{



            guardarDatos(ruta_archivo);








        }
    }


}

void saveBtnCancelarClick(){

    //
    // Colocamos seleccion = -1, y archivo = \0 y ruta =\0 y cerramos
    //
    fila_seleccionada   = -1;
    archivo[0]          = '\0';
    ruta[0]             = '\0';

    closeSave();
}
