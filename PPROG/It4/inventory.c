#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "inventory.h"
#include "types.h"

struct _Inventory{
    Set *objects;
    int max_obj;
};


Inventory *inventory_create(int max_obj){
    /*initializating variables*/
    Inventory *newInventory=NULL;
    if(!max_obj){
      return NULL;
    }
  /*Allocating the inventory's memory*/
    newInventory=(Inventory*)malloc (sizeof(Inventory));
    if (!newInventory)return NULL;
    /*field initialization*/
    newInventory->max_obj=max_obj;
    newInventory->objects=set_ini();
    return newInventory;
}

STATUS inventory_destroy (Inventory *inventory){
    if (!inventory) return ERROR;
    /*destroys the sets of the inventoy and frees it*/
    set_destroy(inventory->objects);
    free (inventory);
    return OK;
}

STATUS inventory_set_object(Inventory* inventory, Id id_obj) {
  if (!inventory) {
    return ERROR;
  }
  /*set an object id if the inventory is not full*/
  if(inventory_is_full(inventory)==FALSE){  
    if(set_add(inventory->objects, id_obj)==OK){
      return OK;
    }
  }
  return ERROR;
}

int inventory_get_max_obj (Inventory * inventory){
    if (!inventory) return 0;
    return inventory->max_obj;
} 

Set *inventory_get_objects(Inventory* inventory) {
  if (!inventory) {
    return NULL;
  }
  return inventory->objects;
}

STATUS inventory_print(Inventory* inventory) {
  Id idaux = NO_ID;
  if (!inventory) return ERROR;

  fprintf(stdout, "--> Inventory (Maximum objects: %d)\n",inventory->max_obj);

  idaux = inventory_get_max_obj(inventory);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> Maximum objects: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No maximum objects.\n");
  }
  if (set_get_ids(inventory->objects)!=NULL) {
    fprintf(stdout, "Inventory: ");
    set_print(stdout, inventory->objects);
    } else {
    fprintf(stdout, "---> No object in the inventory.\n");
  }
  return OK;
}


BOOL inventory_contain_id(Inventory* inv, Id id){
  if(!inv||id==NO_ID)
    return FALSE;
    /*search if the id passed through the function is in the backpack of the player*/
  return set_find(inv->objects, id);
}

BOOL inventory_is_full(Inventory* inv){
  /*checks is the inventory if full by calling the numbers of ids that the set has and comparing it to the max_objects that the player has*/
  if(set_get_num_ids(inv->objects)==inv->max_obj) return TRUE;
  return FALSE;
}

STATUS inventory_delete_object(Inventory* inv, Id id){
  if(!inv || id==NO_ID || inventory_contain_id(inv, id)==FALSE) return ERROR;
  return set_del(inv->objects, id);
}

int inventory_get_number_of_objects(Inventory* inv){
  if(!inv) return -1;
  return set_get_num_ids(inv->objects);
}