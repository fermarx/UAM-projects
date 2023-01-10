#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "player.h"
#include "object.h"
#include "space.h"
#include "set.h"
#include "inventory.h"
#define MAX_OBJ 4

struct _Player{
    
    Id id;
    char name[WORD_SIZE + 1];
    Inventory *inventory;
    Id space;
};

Player *player_create(Id id){
    /*initializating variables*/
    Player *newplayer = NULL;
    if(id==NO_ID)    return NULL;
    /*Allocating the object memory*/
    newplayer = (Player *)calloc(1, sizeof(Player));
    if (newplayer == NULL)
        return NULL;
    /*field initialization*/
    newplayer->id = id;
    newplayer->name[0] = '\0';
    newplayer->inventory=inventory_create(MAX_OBJ);
    newplayer->space=NO_ID;
    return newplayer;
}

STATUS player_destroy(Player *player){
    if (!player)return ERROR;
    inventory_destroy(player->inventory);
    player->inventory=NULL;
    free(player);
    player = NULL;
    return OK;
}
/*sets in the inentory the id of the object you have passed trought the function*/
STATUS player_set_object(Player *player, Id id){
    if (!player || inventory_get_max_obj(player->inventory) > MAX_OBJ) return ERROR;
    if (inventory_set_object(player->inventory, id)==ERROR)return ERROR;
    return OK;
}
/*sets the id where the player is located*/
STATUS player_set_space(Player *player, Id id){
    if (!player || id==NO_ID)
        return ERROR;
    player->space = id;

    return OK;
}
/*sets the name of the player*/
STATUS player_set_name(Player *player, char *name){
    if (!player || !name)
        return ERROR;

    if (!strcpy(player->name, name))
    {
        return ERROR;
    }

    return OK;
}
/*gets the name of the player*/
const char *player_get_name(Player *player){
    if (!player)
    {
        return NULL;
    }
    return player->name;
}
/*returns an array of the inventory that the palyer has*/
Inventory *player_get_inventory (Player* player){
    if (!player)return NULL;
    return player->inventory;
}
/*returns the id of the player*/
Id player_get_id(Player *player){
    if (!player)return NO_ID;
    return player->id;
}
/*numero de objects uqe tiene el Player*/
int player_get_number_of_objects(Player *player){
    if (!player)return 0;
    return inventory_get_number_of_objects(player->inventory);
}

/*maximo numero de objects que puede tener el Player*/
int player_get_max_number_of_objects(Player*player){
    if (!player) return 0;
    return inventory_get_max_obj(player->inventory);
}
/*gets the space where the player is located*/
Id player_get_space(Player *player){
    if (!player) return NO_ID;
    return player->space;
}

STATUS player_print(Player *player){
    if (!player) return ERROR;
    fprintf(stdout, "--> player (Id: %ld; Name: %s, Space: %ld)\n", player->id, player->name, player->id);
    return OK;
}
/*returns true if the player has the object that is passed trought the function*/
BOOL player_contain_object_id(Player* p, Id id){
    if(!p||id==NO_ID)
        return FALSE;
    return inventory_contain_id(p->inventory, id);
}
/*returns an array of objects that the player has */
Id* player_get_objects_array(Player* p){
    if(!p) return NULL;
    return set_get_ids(inventory_get_objects(p->inventory));
}