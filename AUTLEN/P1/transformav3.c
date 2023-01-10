#include "transforma.h"
#include <string.h>
#define MAX_SIMBOLOS 1024
#define MAX_ESTADOS 1024


typedef struct _estado{   
    int id;
    int estados[MAX_ESTADOS];
    int num_estados;
    int tipo;
}Estado;

typedef struct _array{  
    Estado estados[MAX_ESTADOS];
    int num_estados;
}Array;

void initEstado(Estado *e){
    int i;
    e->id=-1;
    for(i=0; i<MAX_ESTADOS; i++){
        e->estados[i]=0;
    }
    e->num_estados=0;
    e->tipo=NORMAL;
}

void initArray(Array *a){
    int i;
    a->num_estados=0;
    for(i=0; i<MAX_ESTADOS; i++){
        initEstado(&a->estados[i]);
    }
}

void insertarEstadoEnEstado(Estado *e, int nuevo_estado){
    int i;
    if(e->num_estados == MAX_ESTADOS) return;
    for(i=0; i<e->num_estados; i++){
        if(e->estados[i]==nuevo_estado){
            return;
        }
    }
    e->estados[e->num_estados] = nuevo_estado;
    e->num_estados++;
}

char* crearNombreEstado(AFND * afnd, Estado *e){
    char* nombre=NULL;
    int i;
    for(i=0; i<e->num_estados; i++){
        strcat(nombre, AFNDNombreEstadoEn(afnd, e->estados[i]));
    }
    return nombre;
}

/*Devuelve 1 si existe y 0 si no existe*/
int existeEstado(Array *a, Estado *e){
    int i, j, k, count;

    if(!a || !e || a->num_estados == MAX_ESTADOS) return 0;

    for (i=0, count=0; i<a->num_estados; i++){
        if(a->estados[i].num_estados == e->num_estados){
            for(j=0; j<a->estados[i].num_estados; j++){
                for(k=0; k<e->num_estados; k++){
                    if(a->estados[i].estados[j] == e->estados[k]){
                        count++;
                        break;
                    }
                }
            }
        }

        /*Ha encontrado un estado igual*/
        if(count==e->num_estados){
            return 1;
        }
        /*Si no lo encuentra reseteamos count y vamos con el siguiente estado*/
        count=0;
    }
    return 0;
}

void insertaEstadoEnArray(Array *a, Estado *e){
    Estado cpy;

    if(!a || !e || a->num_estados==MAX_ESTADOS) return;

    if(existeEstado(a, e)==1) return;

    cpy = copiarEstado(e);

    a->estados[a->num_estados] = cpy;
    a->num_estados++;
}

Estado copiarEstado(Estado *e){
    Estado nuevo;
    int i;

    initEstado(&nuevo);

    for(i=0; i < e->num_estados; i++){
        nuevo.estados[i]=e->estados[i];
    }

    nuevo.id = e->id;
    nuevo.tipo = e->tipo;
    nuevo.num_estados = e->num_estados;

    return nuevo;
}

void buscarTransicionesLambda(AFND *afnd, int ***matriz, int estado, Estado *e){
    int i, j, k;
    int n_estados = AFNDNumEstados(afnd), n_simbolos = AFNDNumSimbolos(afnd);

    i = estado; /* estado del que quieres bucar las transiciones lambda */
    j = n_simbolos;
    for(k=0; k<n_estados; k++){
        if(matriz[i][j][k]==1){
            insertarEstadoEnEstado(e, k);
            buscarTransicionesLambda(afnd, matriz, k, e);
        }
    }
}

AFND * AFNDTransforma(AFND * afnd){

    int fila = 0, columna = 0, capa = 0, n_estados = AFNDNumEstados(afnd), n_simbolos = AFNDNumSimbolos(afnd), i;
    int matriz[n_estados][n_simbolos + 1][n_estados];
    char *nombre, *simbolo[n_estados];
    Array array;
    Estado estado, cpy;
    AFND *afd;

    /**
     *  matriz del afnd dado.
     */
    for(fila = 0; fila < n_estados; fila++){
        for(columna = 0; columna < n_simbolos + 1; columna++){
            for(capa = 0; capa < n_estados; capa++){
                matriz[fila][columna][capa] = AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, fila, columna, capa);
                if(columna == n_simbolos){
                    matriz[fila][columna][capa] = AFNDCierraLTransicionIJ(afnd, fila, columna);
                }
            }
        }
    }

    /**
     *  matriz con los simbolos del autmata dado
     */
    for (i = 0; i < n_estados; i++){
        simbolo[i] = AFNDSimboloEn(afnd, i);
    }

    /**
     * creacion de cada estado que va a ir en el afd
     */
    initArray(&array);
    initEstado(&estado);

    for(fila = 0; fila < n_estados; fila++){
        if (AFNDTipoEstadoEn(afnd, fila) == INICIAL)
            estado.tipo = INICIAL;
        if (AFNDTipoEstadoEn(afnd, fila) == INICIAL_Y_FINAL)
            estado.tipo = INICIAL_Y_FINAL;
        if (AFNDTipoEstadoEn(afnd, fila) == FINAL)
            estado.tipo = FINAL;

        insertarEstadoEnEstado(&estado, fila); 
        buscarTransicionesLambda(afnd, matriz, fila, &estado);
        /*determinar el tipo de estado (función auxiliar)*/
        insertaEstadoEnArray(&array, &estado);

        for(columna = 0; columna < n_simbolos + 1; columna++){
            Estado aux;
            /* para cada columna haces un nuevo estado */
            initEstado(&aux);

            for(capa = 0; capa < n_estados; capa++){
                /* si hay lambda mete lambda */
                if(columna == n_simbolos){
                    if( matriz[fila][columna][capa] == 1){ 
                        insertarEstadoEnEstado(&estado, capa); 
                    }
                }
                else {
                    /* ahora tiene que crear los estados a los que va 

                        un estado nuevo se crea cuando se llega a dos
                        diferentes estados (capas) con un mismo simbolo (columna) */
                    if( matriz[fila][columna][capa] == 1){
                        insertarEstadoEnEstado(&aux, capa);
                    }
                }
            }
            if (AFNDTipoEstadoEn(afnd, capa) == INICIAL)
                aux.tipo = INICIAL;
            if (AFNDTipoEstadoEn(afnd, capa) == INICIAL_Y_FINAL)
                aux.tipo = INICIAL_Y_FINAL;
            if (AFNDTipoEstadoEn(afnd, capa) == FINAL)
                aux.tipo = FINAL;

            /* si hay un estado con el mismo nombre no lo mete en el array, sino, sí*/
            cpy = copiarEstado(&aux);
            insertaEstadoEnArray(&array, &cpy);
        }
    }

    /**
     *  matriz afd
     */
    afd = AFNDNuevo("afd", array.num_estados, sizeof(simbolo));
    /* inserta los simbolos que se utilizan en el afnd */
    for ( i = 0; i <  sizeof(simbolo); i++) {
        AFNDInsertaSimbolo(afd, simbolo[i]);
    }
    /* inserta los nuevos estados que se han creado */
    for (i = 0; i < array.num_estados; i++) {
        nombre = crearNombreEstado(afnd, &array.estados[i]);
        AFNDInsertaEstado(afd, nombre, &array.estados[i].tipo);
    }



}