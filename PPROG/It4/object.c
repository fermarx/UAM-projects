/** 
 * @brief It implements the object interface and all the function to manage it
 * 
 * @file Object.c
 * @author 
 * @version 1.0 
 * @date 13-01-2015 
 * @copyright GNU Public License
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "object.h"
#define MAX_OPTIONS 5
#define MAX_DESCRIPTION 200

/*Stores the Id of the space where the object is located and it's name*/
struct _Object{
    Id id;
    char name[WORD_SIZE + 1];
    char description[MAX_DESCRIPTION];
    char moved_description[MAX_DESCRIPTION];
    BOOL movable;
    BOOL moved;
    BOOL hidden;
    Id open;
    BOOL illuminate;
    BOOL turnedOn; 
    Id starter_loc;
};

Object *object_create(Id id){
    /*initializating variables*/
    Object *newObject = NULL;

    if(id==NO_ID) return NULL;
    /*Allocating the object memory*/
    newObject = (Object*)malloc(sizeof(Object));
    if (newObject == NULL){
        return NULL;
        }
    /*field initialization*/
    newObject->id = id;
    newObject->name[0]='\0';
    newObject->description[0]='\0';
    newObject->moved_description[0]='\0';
    newObject->hidden=FALSE;
    newObject->moved=FALSE;
    newObject->movable=TRUE;
    newObject->open=NO_ID;
    newObject->illuminate=FALSE;
    newObject->turnedOn=FALSE;
    newObject->starter_loc=NO_ID;
    return newObject;
}


void object_destroy(Object *a){
    if (!a)return;
    free(a);
    a = NULL;
    return;
}
/*sets the name*/
STATUS object_set_name(Object *a, char *name){
    if (!a || !name) return ERROR;
    if (!strcpy(a->name, name)) return ERROR;
    return OK;
}
/*sets the description*/
STATUS object_set_description(Object *o, char *desc){
    if (!o || !desc)return ERROR;
    strcpy(o->description, desc);
    return OK;
}
/*get the name*/
char *object_get_name(Object *a){
    if (!a) return NULL;
    return a->name;
}
/*get id*/
Id object_get_id(Object *a){
    if (!a) return NO_ID;
    return a->id;
}

STATUS object_print(Object *a){
    fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", a->id, a->name);   
    return OK;
}
/*gets the description of the object*/ 
char *object_get_description(Object* o){
    if(!o){
        return NULL;
    }
    return o->description;
}
/*gets if an object can illuminate or not*/
BOOL object_get_illuminate(Object* o){
    if(!o) return FALSE;
    return o->illuminate;
}
/*sets if an objecct can illuminate or not*/
STATUS object_set_illuminate(Object *o, BOOL state){
    if(!o)return ERROR;
    o->illuminate=state;
    return OK;
}
/*get if an object can be moved*/
BOOL object_get_movable(Object* o){
    if(!o) return FALSE;
    return o->movable;
}
/*sets if an object can be moved*/
STATUS object_set_movable(Object* o, BOOL state){
    if(!o) return ERROR;
    o->movable=state;
    return OK;
}
/*get if an object has been moved*/
BOOL object_get_moved(Object* o){
    if(!o) return FALSE;
    return o->moved;
}
/*sets if an object has been moved*/
STATUS object_set_moved(Object* o, BOOL state){
    if(!o) return ERROR;
    o->moved=state;
    return OK;
}
/*get if an object is hidden*/
BOOL object_get_hidden(Object* o){
    if(!o) return FALSE;
    return o->hidden;
}
/*sets if an object id hidden*/
STATUS object_set_hidden(Object* o, BOOL status){
    if(!o) return ERROR;
    o->hidden=status;
    return OK;
}
/*gets if an object can open links*/
Id object_get_open(Object* o){
    if(!o) return ERROR;
    return o->open;
}
/*sets if an object can open links*/
STATUS object_set_open(Object* o, Id id){
    if(!o) return ERROR;
    o->open=id;
    return OK;
}
/*sets if an object can illuminate*/
STATUS object_set_turnedOn(Object* o, BOOL state){
    if(!o) return ERROR;

    if(o->illuminate==TRUE){
        o->turnedOn=state;
        return OK;
    }
        return ERROR;
}
/*gets if an object can illuminate*/
BOOL object_get_turnedOn(Object* o){
    if(!o) return FALSE;
    return o->turnedOn;
}
/*sets the description of an object if it has been moved*/
STATUS object_set_moved_description(Object* o, char* description){
    if(!o || !description) return ERROR;
    if(!strcpy(o->moved_description, description)) return ERROR;
    return OK;
}
/*gets the description of an object if it has been moved*/
char* object_get_moved_description(Object* o){
    if(!o) return NULL;
    return o->moved_description;
}
/*sets the started location of a game*/
STATUS object_set_starter_loc(Object* o, Id id){
    if(!o) return ERROR;
    o->starter_loc=id;
    return OK;
}
/*gets the started location of a game*/
Id object_get_starter_loc(Object* o){
    if(!o) return NO_ID;
    return o->starter_loc;
}
