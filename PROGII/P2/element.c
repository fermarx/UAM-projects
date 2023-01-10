#include "element.h"

struct _Element {
    char *a;
};

Element * element_init(){

    Element *newEle;

    newEle = (Element*) malloc(sizeof(Element));
    if (!newEle) return NULL;

    newEle->a = NULL;

    return newEle;
}

void element_free(Element *ele){

    if (ele){
        free(ele->a);
        free(ele);
        ele = NULL;
    }
    return;
}

Element *element_setInfo(Element *ele, void* info){

    if (!ele || !info) return NULL;

    if(ele->a) free(ele->a);


    ele->a = (char*)calloc(1,sizeof(char*));
    if (!ele->a) return NULL;

    *ele->a = *(char*)info;
    if(!ele->a){
        free(ele->a);
        return NULL;
    }

    return ele;
}

void * element_getInfo(Element *ele){

    if (!ele) return NULL;

    return (char*)ele->a;
}

Element * element_copy(const Element *ele){

    Element *cpy = NULL;

    if (!ele) return NULL;

    cpy = element_init();
    if(!cpy) return NULL;

    if (!element_setInfo(cpy, ele->a)) {
        element_free(cpy);
        return NULL;
    }

    return cpy;
}

Bool element_equals(const Element *ele1, const Element *ele2){

    if (!ele1 || !ele2) return FALSE;

    if (strcmp(ele1->a, ele2->a) == 0) return TRUE;

    return FALSE;
}

int element_print(FILE *pf, const Element *ele){

    int nchar = 0;

    if (!pf || !ele) return -1;

    nchar += fprintf(pf, "%s ", ele->a);

    return nchar;
}