#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "types.h"
#define MAX_IDS 4

struct _Set{
    Id array_ids[MAX_IDS];    
    int num_ids; 
};

Id set_obtain_Id_by_position(Set *s, int index){
    if(!s || (index >= s->num_ids) || (index < 0)){return NO_ID;}
    return s->array_ids[index];
}

Set *set_ini(){
    /*initializating variables*/
    Set *s=NULL;
    int i;
    /*Allocating the set memory*/
    s=(Set*)malloc(sizeof(Set));
    if(!s) return NULL;
    /*field initialization*/
    s->num_ids=0;
    for(i=0; i<MAX_IDS; i++){
        s->array_ids[i] = NO_ID;
    }
    return s;
}

void set_destroy(Set *s){
    if(s){
        free(s);
    }
}
/*adds an id*/
STATUS set_add(Set *s, Id id){
    if(s->num_ids==MAX_IDS || !s || id==NO_ID) return ERROR;
    s->array_ids[s->num_ids]=id;
    s->num_ids++;
    return OK;
}
/*returns true if the set if full*/
BOOL set_is_full(Set *s){
    if(!s || s->num_ids==MAX_IDS){return TRUE;}
    return FALSE;
}
/*deletes the id that you pass througth the function*/
STATUS set_del(Set *s, Id id){
    int i;
    if(!s) return ERROR;
    if(s->num_ids==0|| id==NO_ID) return ERROR;
    for(i=0; i<(s->num_ids); i++){
        if(s->array_ids[i]==id){            
            for(; i < s->num_ids; i++){
                s->array_ids[i] = s->array_ids[i+1];
            }
            
            s->num_ids--;
            return OK;
        }
    }
    return ERROR;
}
/*returns the number of ids the set has*/
int set_get_num_ids(Set *s){
    if (!s)return -1;
    return s->num_ids;
}
/*returns true if the id you have passed throught the fnction is in the set array*/
BOOL set_find(Set *set, Id id){
    int i;
    if(!set || id ==NO_ID) return FALSE;
    for( i = 0; i < set->num_ids; i++){
        if(set->array_ids[i] == id){
            return TRUE;
        }
    }
    return FALSE;
}

STATUS set_print(FILE *pf, Set *s){
    int i;
    if(!s || !pf) {return ERROR;}
    for(i=0; i<(s->num_ids); i++){
        fprintf(pf, "%ld ", s->array_ids[i]);
    }
    return OK;
}
/*returns a set of arrays*/
Id *set_get_ids(Set *set){
    if(!set){
        return ERROR;
    }
    return set->array_ids;
}