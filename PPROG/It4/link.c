#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"
#include "types.h"
#define MAX_NAME 25
    /*Link struct for making the move command and conections possible*/
struct _Link
{
    Id id;
    char name[MAX_NAME];
    Id first;
    Id second;
    BOOL state; /*link->state == TRUE if you can pass through it*/
};

Link* link_create(Id id){
    /*initializating variables*/
    Link *newLink=NULL;
    if(id==NO_ID) return NULL;
    /*Allocating the link memory*/
    newLink=(Link*)malloc(1*sizeof(Link));
    if(!newLink)
        return NULL;
    /*field initialization*/
    newLink->id=id;
    newLink->name[0]='\0';
    newLink->first=NO_ID;
    newLink->second=NO_ID;
    newLink->state=FALSE;
    return newLink;
}
/*destroys a link*/
void link_destroy(Link* lk){
    if(!lk)
        return;
    free(lk);
    lk=NULL;
    return;
}
/*sets the name*/
STATUS link_set_name(Link* lk, char* name){
    if(!lk||!name)
        return ERROR;
    if (!strcpy(lk->name, name))
        return ERROR;
    return OK;
}
/*sets the first link*/
STATUS link_set_first(Link* lk, Id first){
    if(!lk || first==NO_ID)
        return ERROR;
    lk->first=first;
    return OK;
}
/*sets the second link*/
STATUS link_set_second(Link* lk, Id second){
    if(!lk || second==NO_ID)
        return ERROR;
    lk->second=second;
    return OK;
}
/*sets the first and second link*/
STATUS link_set_connect(Link* lk, Id first, Id second){
    if(!lk||first==NO_ID||second==NO_ID)
        return ERROR;
    lk->first=first;
    lk->second=second;
    return OK;
}
/*sets if a link is open or not*/
STATUS link_set_state(Link* lk, BOOL state){
    if(!lk)
        return ERROR;
    lk->state=state;
    return OK;
}
/*gets the id*/
Id link_get_id(Link* lk){
    if(!lk)
        return NO_ID;
    return lk->id;
}
/*gets the name*/
char* link_get_name(Link* lk){
    if (!lk)
        return NULL;
    return lk->name;
}
/*gets the first link*/
Id link_get_first(Link* lk){
    if(!lk)
        return NO_ID;
    return lk->first;
}
/*gets the second link*/
Id link_get_second(Link* lk){
    if(!lk)
        return NO_ID;
    return lk->second;
}
/*gets if a link is open or not*/
BOOL link_get_state(Link* lk){
    if(!lk)
        return FALSE;
    return lk->state;
}

STATUS link_print(FILE* pf, Link* lk){
    if (!lk)
        return ERROR;
    fprintf(stdout, "--> Link (Id: %ld; Name: %s; First space:%ld; Second space:%ld)\n", lk->id, lk->name, lk->first, lk->second);
    return OK;
}
/*copies a link*/
Link* link_copy(Link* lk){
    Link* link=NULL;
    if(!lk) return NULL;
    link=(Link*)malloc(1*sizeof(Link));
    link->id=lk->id;
    strcpy(link->name, lk->name);
    link->first=lk->first;
    link->second=lk->second;
    link->state=lk->state;
    return link;
}
/*returns the id of the link (first or second) that is equal to the one that you send througth the function*/
Id link_get_connection(Id s, Link* l){
  if(s==NO_ID||!l){
    return NO_ID;
  }
  if(link_get_second(l)==s)
    return link_get_first(l);
  return link_get_second(l);
}
