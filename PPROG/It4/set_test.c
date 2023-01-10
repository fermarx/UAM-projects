#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#define MAX_IDS 4


int main(){
    Set *test=NULL;
    int i=2;

    printf("Initialising set...");
    test=set_ini();
    if(!test){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }

    printf("Printing empty set: ");
    set_print(stdout, test);
    printf("\n");

    printf("Adding invalid id...");
    if(set_add(test, NO_ID)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Adding id 2...");
    if(set_add(test, 1)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }

    printf("Invalidly checking if set is full...");
    if(set_is_full(NULL)==TRUE){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Checking if set is full...");
    if(set_is_full(test)==TRUE){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }

    printf("Filling set...\n");
    while(set_add(test, i)==OK){
        i++;
    }
    printf("Checking if set is full...");
    if(set_is_full(test)==FALSE){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }

    printf("The set has %d ids.\n", set_get_num_ids(test));

    printf("Invalidly deleting id 2...");
    if(set_del(NULL, 2)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
    printf("Deleting id 2...");
    if(set_del(test, 2)==ERROR){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }

    printf("Looking for id 2...");
    if(set_find(test, 2)==FALSE){
        printf("ERROR\n");
    }else{
        printf("OK\n");
    }
   printf("Looking for id 1...");
    if(set_find(test, 1)==FALSE){
        printf("NOT IN SET\n");
    }else{
        printf("OK\n");
    }

    printf("Id in position 3 is...%ld", set_obtain_Id_by_position(test, 3));


    set_destroy(test);
    printf("\n");
    return (EXIT_SUCCESS);
}
