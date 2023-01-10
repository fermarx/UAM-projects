#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "inventory.h"
#define MAX_OBJ 4

int main(int argc, char **argv) {

    Inventory *inventory=NULL;
    Inventory *NullInv=NULL;
    int i, NullnumObj=0;

/*--------------------------------------------------------------------------------------*/
    printf("Initializating null inventory\n");
    NullInv=inventory_create(NullnumObj);
    if (!inventory){
        printf ("ERROR: initializating the inventory.\n");
        }
    printf("\n");
    printf("Initializating null inventory\n");
    inventory = inventory_create (MAX_OBJ);
    if (!inventory){
        printf ("Error while initializating the inventory.\n");
    }
    printf("OK\n");
    printf("\n");
    printf("\n");
/*--------------------------------------------------------------------------------------*/
    printf ("Printing the empty inventory\n");
    if (inventory_print(inventory)==ERROR){ 
        printf("Null inventory sended\n");
        }

    for (i=0;i<MAX_OBJ;i++){
        if (inventory_set_object(inventory, i)==ERROR){
            printf("ERROR while setting the id %i\n", i);
        } 
    }printf("ERROR\n");
    printf("\n");
    printf ("Printing the empty null inventory\n");
    if (inventory_print(NullInv)==ERROR){ 
        printf("Null inventory sended\n");
        }

    for (i=0;i<MAX_OBJ;i++){
        if (inventory_set_object(NullInv, i)==ERROR){
            printf("ERROR while setting the id %i\n", i);
        } 
    }
    printf ("Setting ids in the inventory was UNsuccesfull\n");
    printf("\n");
/*--------------------------------------------------------------------------------------*/
    printf ("Printing the full inventory\n");
    if (inventory_print(inventory)==ERROR) printf("ERROR: Null inventory\n");

    printf ("Searching for id 666\n");
    if(inventory_contain_id (inventory, 666)==FALSE){
        printf ("The inventory does not contain id 666\n");
    }else{
        printf("Inventory does contain id 666\n");
    }
    printf("\n");
    printf ("Printing the full NULL inventory\n");
    if (inventory_print(NullInv)==ERROR) printf("ERROR: Null inventory\n");

    printf ("Searching for id 666\n");
    if(inventory_contain_id (inventory, 666)==FALSE){
        printf("The inventory is not initialized or it does not contain id 666\n");
    }
    printf("\n");
/*--------------------------------------------------------------------------------------*/
    printf ("Deleting all the data of inventory\n");
    if (inventory_destroy(inventory)==ERROR){
        printf("Error while destorying the inventory.\n");
    }else{
        printf("Inventory destroyed successfully\n");
    }
    printf("\n");
        printf ("Deleting all the data of null inventory\n");
    if (inventory_destroy(NullInv)==ERROR){
        printf("ERROR while destorying the inventory.\n");
    }
    printf("\n");
/*--------------------------------------------------------------------------------------*/

    printf("TEST FINNISHED, EXITING\n");
    return EXIT_SUCCESS;
}
