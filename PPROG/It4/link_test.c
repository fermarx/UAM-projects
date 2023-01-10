#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"
#define MAX_OBJ 4

int main(){
    Link* lk=NULL, *copy=NULL;

    printf("Invalidly initialising link...");
    lk=link_create(NO_ID);
    if(!lk){
        printf("ERROR\n");
    }
    else{
        printf("OK\n");
    }
    printf("Initialising link...");
    lk=link_create(1);
    if(!lk){
        printf("ERROR\n");
    }
    else{
        printf("OK\n");
    }


    printf("Invalidly setting name...");
    if(link_set_name(lk, NULL)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Setting name...");
    if(link_set_name(lk, "link")==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }


    printf("Invalidly setting first...");
    if(link_set_first(lk, NO_ID)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Setting first...");
    if(link_set_first(lk, 1)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }


    printf("Invalidly setting second...");
    if(link_set_second(lk, NO_ID)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Setting second...");
    if(link_set_second(lk, 2)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }


    printf("Invalidly setting state...");
    if(link_set_state(NULL, TRUE)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Setting state...");
    if(link_set_state(lk, TRUE)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }


    printf("Invalidly copying link...");
    if((copy=link_copy(NULL))==NULL){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Copying link...");
    if((copy=link_copy(lk))==NULL){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }



    printf("Printing link...");
    link_print(stdout, lk);
    printf("Printing copy...");
    link_print(stdout, copy);
    printf("\n");
    printf("Getting id...%ld\n", link_get_id(lk));
    printf("Getting name...%s\n", link_get_name(lk));
    printf("Getting first...%ld\n", link_get_first(lk));
    printf("Getting second...%ld\n", link_get_second(lk));
    printf("Getting state...%d\n", link_get_state(lk));
    printf("Getting connection to 1st space...%ld\n", link_get_connection(1, lk));
    
    link_destroy(lk);
    link_destroy(copy);

    return 0;
}
