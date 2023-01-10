#include <stdio.h>
#include <string.h>
#include "minimiza.h"
#define INACCESIBLE 0
#define ACCESIBLE 1
#define INDISTINGUIBLE 0
#define DISTINGUIBLE 1
#define MAX_NOMBRE 20

typedef struct _estado{  
    char *nombre;
    int *estados;
    int num_estados;
    int tipo;
    int num_clases;
}Estado;

/*Inicializamos estado con tipo normal por defecto*/
Estado* init_estado(int num_estados){
    Estado *nuevo=NULL;

    nuevo=(Estado*)malloc(sizeof(Estado));
    if(!nuevo){
        return NULL;
    }

    nuevo->estados = (int*)calloc(num_estados, sizeof(int));
    if(!nuevo->estados){
        free(nuevo);
        return NULL;
    }

    /*Dejamos al reserva de memoria del nombre para más adelante*/
    nuevo->nombre = NULL;
    nuevo->num_estados = num_estados;
    nuevo->tipo = NORMAL;
    nuevo->num_clases = -1;

    return nuevo;
}

void free_estado(Estado *e){
    if(e->nombre!=NULL){
        free(e->nombre);
    }
    if(e->estados!=NULL){
        free(e->estados);
    }
    
    free(e);
   
}

void print_estado(AFND *afnd, Estado* estado){
    
    int i;
    
    for (i = 0; i < estado->num_estados; i++)
        printf("%d ", estado->estados[i]);
        
}

void crear_nombre_estado(AFND * afnd, Estado *e){
    char *nombre=NULL;
    int i, longitud=0;

    for(i=0; i<e->num_estados; i++){
        longitud += strlen(AFNDNombreEstadoEn(afnd, e->estados[i]));
    }
    longitud++;


    nombre = (char*)malloc(longitud*sizeof(char));
    if(!nombre) return;

    strcpy(nombre, AFNDNombreEstadoEn(afnd, e->estados[0]));
    for(i=1; i<e->num_estados; i++){
        strcat(nombre, AFNDNombreEstadoEn(afnd, e->estados[i]));
    }

    e->nombre = nombre;
}

void determinar_tipo_estado(AFND *afnd, Estado *e, int existe_inicial){
    int i;

    if(existe_inicial==0){/*Todavía no existe inicial por lo que podemos poner como tipo inicial*/
        for(i=0; i<e->num_estados; i++){

            if(e->tipo==FINAL){
                if(AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL || AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                    e->tipo = INICIAL_Y_FINAL;
                    return;
                }
            }else if(e->tipo==INICIAL){
                if(AFNDTipoEstadoEn(afnd, e->estados[i]) == FINAL || AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                    e->tipo = INICIAL_Y_FINAL;
                    return;
                }
            }else if(e->tipo==NORMAL){
                if(AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL){
                    e->tipo = INICIAL;
                }else if(AFNDTipoEstadoEn(afnd, e->estados[i]) == FINAL){
                    e->tipo = FINAL;
                } else if(AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                    e->tipo = INICIAL_Y_FINAL;
                    return;
                }
            }
            
        }
    } else { /*Ya existe estado inicial por lo que no puede haber otro*/
        for(i=0; i<e->num_estados; i++){
            /*Si el tipo es final no hago nada ya que solo puedo pasar a inicial_final pero no puedo tener más estados iniciales*/
            /*El tipo no puede ser inicial porque ya tengo un estado inicial.*/
            /*Solo cambia en caso de ser normal*/
            if(e->tipo==NORMAL){
                if(AFNDTipoEstadoEn(afnd, e->estados[i]) == FINAL || AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                    e->tipo = FINAL;
                    return;
                }
            }
        }
    }
    
}

void free_array_estados(Estado **array){
    int i, num_estados;

    if(array!=NULL){
        num_estados = array[0]->num_clases;
        for(i=0; i<num_estados; i++){
            if(array[i]){
                free_estado(array[i]);
            }
        }
        free(array);
    }
}

void aux_encontrar_estados_inaccesibles(AFND *afnd, int estado, int num_estados, int num_simb, int* accesible){
    int i, j;
    int existe_transicion=0;
    

    for(i=0; i<num_estados; i++){
        if(accesible[i]!=ACCESIBLE){
            for(existe_transicion=0, j=0; j<num_simb; j++){
                existe_transicion=AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, estado, j, i);
                if(existe_transicion==1){
                    break;
                }
            }
            if(existe_transicion==1){
                accesible[i]=ACCESIBLE;
                aux_encontrar_estados_inaccesibles(afnd, i, num_estados, num_simb, accesible);
            }
        }
    }
}

int* encontrar_estados_inaccesibles(AFND *afnd){
    int *accesible=NULL;
    int num_estados=-1, inicial=-1, num_simb=-1, i;

    num_estados = AFNDNumEstados(afnd);
    accesible = (int*)malloc(num_estados*sizeof(int));
    if(!accesible){
        return NULL;
    }

    for(i=0; i<num_estados; i++){
        accesible[i]=INACCESIBLE;
    }

    inicial = AFNDIndiceEstadoInicial(afnd);
    num_simb = AFNDNumSimbolos(afnd);

    accesible[inicial] = ACCESIBLE;

    aux_encontrar_estados_inaccesibles(afnd, inicial, num_estados, num_simb, accesible);

    return accesible;
}



/*
* En esta función devolvemos un nuevo afnd habiendo eliminado los
* estados inaccesibles(indicados por int *accesible) del afnd pasados como argumento.
* Además se libera memoria del array de int pasado como argumento.
* Se ha de llamar primero a encontrar_estados_inaccesibles para obtener
* el argumento de int *accesible
*/
AFND* eliminar_estados_inaccesibles(AFND *afnd, int *accesible){
    AFND* nuevo=NULL;
    int num_estados_antiguo=-1, num_estados_nuevo=-1, num_simb=-1;
    int i, j, k;

    if(!afnd || !accesible){
        return NULL;
    }

    num_estados_antiguo = AFNDNumEstados(afnd);
    num_simb = AFNDNumSimbolos(afnd);

    for(num_estados_nuevo=0, i=0; i<num_estados_antiguo; i++){
        if(accesible[i]==ACCESIBLE){
            num_estados_nuevo++;
        }
    }

    /*Caso especial*/
    if(num_estados_antiguo == num_estados_nuevo){
        return afnd;
    }

    nuevo = AFNDNuevo("solo_accesibles", num_estados_nuevo, num_simb);
    if(!nuevo){
        return NULL;
    }
    
    for (i=0; i < num_estados_antiguo; i++){
        if(accesible[i]==ACCESIBLE){
            AFNDInsertaEstado(nuevo, AFNDNombreEstadoEn(afnd, i), AFNDTipoEstadoEn(afnd, i));
        }
    }

    for(i=0; i<num_simb; i++){
        AFNDInsertaSimbolo(nuevo, AFNDSimboloEn(afnd, i));
    }
    
    for(i=0; i < num_estados_antiguo; i++){
        if(accesible[i]==ACCESIBLE){
            for(j=0; j < num_simb ; j++){
                for(k=0; k < num_estados_antiguo; k++){
                    if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k)){
                        AFNDInsertaTransicion(nuevo, AFNDNombreEstadoEn(afnd, i), AFNDSimboloEn(afnd, j), AFNDNombreEstadoEn(afnd, k));
                    }
                }
            }
        }       
    }

    return nuevo;
}


/*Reservamos memoria e inicializamos la matriz con valores de INDISTINGUIBLE*/
int **init_matriz(AFND *afnd){
    int **matriz=NULL;
    int n_estados, i, j;

    n_estados = AFNDNumEstados(afnd);

    matriz = (int**)malloc(n_estados*sizeof(int*));
    if (!matriz) return NULL;

    for(i=0; i<n_estados; i++){
        matriz[i] = (int*)malloc(n_estados*sizeof(int));
        if(matriz[i]==NULL){/*Control de errores*/
            i--;
            for(; i>=0; i--){
                free(matriz[i]);
            }
            free(matriz);
            return NULL;
        }

        /*Inicializamos todas las casillas de la matriz*/
        for(j=0; j<n_estados; j++){
            matriz[i][j] = INDISTINGUIBLE;
        }    
    }

    return matriz;
}

/*liberamos matriz*/
void free_matriz(int **matriz, int num_estados){
    int i;

    for(i=0; i<num_estados; i++){
        if(matriz[i]){
            free(matriz[i]);
        }
    }

    if(matriz){
        free(matriz);
    }

}

/* distinguimos entre estados finales y no finales */
void matriz_estados_finales(AFND *afnd, int **matriz){
    int n_estados;
    int i, j;

    n_estados = AFNDNumEstados(afnd);

    for (i = 0; i < n_estados; i++){
        if(AFNDTipoEstadoEn(afnd, i) == FINAL || AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL){
            for(j = 0; j < n_estados; j++){
                
                if (AFNDTipoEstadoEn(afnd, j) != FINAL && AFNDTipoEstadoEn(afnd, j) != INICIAL_Y_FINAL){
                    matriz[i][j] = DISTINGUIBLE;
                    matriz[j][i] = DISTINGUIBLE;
                }
            }
        }
    }
}

void matriz_algoritmo(AFND *afnd, int  **matriz){
    int n_estados, n_simb, cambio;
    int estado1, estado2;
    int i, j, k, l;

    n_estados = AFNDNumEstados(afnd);
    n_simb = AFNDNumSimbolos(afnd);

    do{
        for(cambio=0, i=0; i<n_estados; i++){
            for(j=0; j<n_estados; j++){
                if(matriz[i][j]==INDISTINGUIBLE){

                    for(k=0; k<n_simb; k++){
                        /*Miramos la transicion para el primer estado (indice i)*/
                        for(l=0; l<n_estados; l++){
                            if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, k, l)==1){
                                estado1=l;
                                break;
                            }
                        }

                        /*Miramos la transicion para el primer estado (indice j)*/
                        for(l=0; l < n_estados; l++){
                            if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, j, k, l)==1){
                                estado2=l;
                                break;
                            }
                        }

                        /*Comprobamos que a los estados a los que se llega son distinguibles entre sí.
                        En caso de serlo, nuestros estados i, j serán distinguibles entre sí.*/
                        if(matriz[estado1][estado2]==DISTINGUIBLE){
                            matriz[i][j] = DISTINGUIBLE;
                            matriz[j][i] = DISTINGUIBLE;
                            cambio = 1;
                            break;
                        }

                    }
                }
            }
        }

    }while(cambio!=0);

}

int* crear_clases(AFND *afnd, int **matriz, int *n_clases){

    int i, j, n_estados, *clases = NULL, cont_clase = 0;
    int todo1 = 0;

    n_estados = AFNDNumEstados(afnd);
    clases = (int*)malloc(n_estados*sizeof(int));
    if(!clases) return NULL;
    
    /*Inicializamos a -1 para comprobar más tarde si hemos asignado una clase ya o no*/
    for(i = 0 ; i < n_estados; i++){
        clases[i] = -1;
    }
    
    /* creas un array con los estados, guardas
    en el array i la clase a la que pertenece
    el estado i si es disitnguble  */
    for (i =0 ; i < n_estados; i++){
        for(todo1=0, j=0; j < n_estados; j++){
            if(matriz[i][j] == INDISTINGUIBLE){
                todo1=1;
                if(clases[i] == -1){
                    clases[i] = cont_clase;
                    cont_clase++;
                }
                else if(clases[j] == -1)
                    clases[j] = clases[i];
            }
        }
        /*Caso especial que todos los estados sean distinguibles con el estado i*/
        if((todo1=0)){
            clases[i]=cont_clase;
            cont_clase++;
        }
    }
    *n_clases = cont_clase;
    return clases;

}

/*3 sets de fors:
1: Saber numero de clases
2: Saber numero de estados de cada clase
3: Colocar cada estado con los de su clase y poner nombre
*/
Estado **crear_array_estados(AFND *afnd, int *clases, int n_clases){
    Estado **estados=NULL;
    int n_estados=0, num_estados_antiguo, num_estados_registrados=0, /*n_clases,*/ existe_inicial, i, j, k;

    num_estados_antiguo = AFNDNumEstados(afnd);

    /*Reservamos memoria*/
    estados = (Estado**)malloc(n_clases*sizeof(Estado*));
    if(!estados){
        return NULL;
    }

    for(i=0; i<n_clases; i++){
        for(n_estados=0, j=0; j<num_estados_antiguo; j++){
            if(clases[j]==i){
                n_estados++;
            }
        }
        /*Inicializamos el estado en el array con posicion i*/
        estados[i] = init_estado(n_estados);
        if(!estados[i]){
            i--;
            for(; i>=0; i--){
                free_estado(estados[i]);
            }
            free(estados);
        }
    }

    /*tienes el array iniciado y quieres que en el estado 0 te guarde
      los estados que hay en clases[0] etc

      clases[j] = array que determina a que clase pertenecce cada estado
      estados[i] = en el caso del ejemplo va a haver 4 estados donde luego
                    se vana  guardar en estados[i]->estados[n_estados] los estados
                    en clases

    */
    existe_inicial=0;
    for(i=0; i<n_clases; i++){
        for(n_estados=0, j=0; j<num_estados_antiguo; j++){
            if(clases[j]==i){
                estados[i]->estados[n_estados] = j;
                n_estados++;
            }
        }
        estados[i]->num_clases = n_clases;
        crear_nombre_estado(afnd, estados[i]);
        determinar_tipo_estado(afnd, estados[i], existe_inicial);
        if(estados[i]->tipo == INICIAL || estados[i]->tipo == INICIAL_Y_FINAL){
            existe_inicial=1;
        }
    }

    return estados;
}

AFND *crear_automata_minimizado(AFND *afnd, Estado **estados){
    AFND *nuevo = NULL;
    int n_estados, n_simb, n_estado_antiguo, i, j, k;
    int aux, transita=0;

    n_simb = AFNDNumSimbolos(afnd);
    n_estados = estados[0]->num_clases;
    n_estado_antiguo = AFNDNumEstados(afnd);

    nuevo = AFNDNuevo("minimizado", n_estados, n_simb);
    if(!nuevo){
        return NULL;
    }

    for(i=0; i<n_simb; i++){
        AFNDInsertaSimbolo(nuevo, AFNDSimboloEn(afnd, i));
    }

    for(i=0; i<n_estados; i++){
        AFNDInsertaEstado(nuevo, estados[i]->nombre, estados[i]->tipo);
    }

    for(i=0; i<n_estados; i++){
        /*Miramos solo el primer estado original de la clase de estados ya que son indistinguibles*/
        aux = estados[i]->estados[0];
        
        for(j=0; j<n_simb; j++){
            for(transita=0, k=0; k<n_estado_antiguo; k++){
                if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, aux, j, k)==1){
                    /*k es mi estado al que transiciona aux con simbolo j*/
                    /*ahora quiero mirar en que clase esta k*/
                    /* despues insertar una transicion entre la clase i y la clase a la que
                    pertenece k con simbolo j */
                    /*al final break para que no siga mirando estados que no hace falta*/
                    for(int l=0; l<n_estados; l++){
                        for(int m=0; m<estados[l]->num_estados; m++){
                            if(estados[l]->estados[m] == k){/*Lo hemos encontrado*/
                                /*el estado k está en la clase l*/
                                AFNDInsertaTransicion(nuevo, AFNDNombreEstadoEn(nuevo, i), AFNDSimboloEn(nuevo, j), AFNDNombreEstadoEn(nuevo, l));
                                /*Como solo puede tener una transicion hago breaks hasta que cambie de simbolo para mirar otra transicion*/
                                transita=1;
                                break;
                            }
                        }
                        if(transita!=0){
                            break;
                        }
                    }
                }
                if(transita!=0){
                    break;
                }
            }
        }
    }

    return nuevo;

}

/*FALTAN CONTROLES DE ERRORES*/
AFND *AFNDMinimiza(AFND * afnd){

    AFND *afnd_simp = NULL, *afnd_minim = NULL;
    int *accesible=NULL, **matriz = NULL, *clases = NULL, n_clases = 0;
    Estado **array=NULL;

    int i,j;

    /*Tenemos afnd sin estados inaccesibles*/
    accesible = encontrar_estados_inaccesibles(afnd);
    if(!accesible){
        printf("Error en encontrar_estados_inaccesibles\n");
        return NULL;
    }

    afnd_simp = eliminar_estados_inaccesibles(afnd, accesible);
    free(accesible);
    if(!afnd_simp){
        printf("Error en eliminar_estados_inaccesibles\n");
        return NULL;
    }
    

    /*Creamos y completamos matriz de distinguibles*/
    matriz = init_matriz(afnd_simp);
    if(!matriz){
        printf("Error en init_matriz\n");
        return NULL;
    }

    matriz_estados_finales(afnd_simp, matriz);
    matriz_algoritmo(afnd_simp, matriz);

    /*Creamos un array para decirnos qué estado pertenece a qué clase*/
    clases = crear_clases(afnd_simp, matriz, &n_clases);
    free_matriz(matriz, AFNDNumEstados(afnd_simp));
    if(!clases){
        printf("Error en crear_clases\n");
        return NULL;
    }

    /*Creamos array de estados*/
    array = crear_array_estados(afnd_simp, clases, n_clases);    
    free(clases);
    if(!array){
        printf("Error en crear_array_estados\n");
        return NULL;
    }

    /*Creamos el automata minimizado*/
    afnd_minim = crear_automata_minimizado(afnd_simp, array);
    free_array_estados(array);
    if(!afnd_minim){
        return NULL;
    }
    if(AFNDNumEstados(afnd)!=AFNDNumEstados(afnd_simp)){
        AFNDElimina(afnd_simp);
    }

    return afnd_minim;
}