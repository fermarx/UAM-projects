#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

int main(){
    Player *player, *Nullplayer=NULL;
    Id NullplayerId=NO_ID;
    const char *name=NULL;
    int numObjs, maxObjs;
    Id Pid=NO_ID, id=NO_ID;
    /*----------------------------------------------------------------------------*/
    printf("Creating a null player\n");
    player=player_create(NullplayerId);
    if(player==NULL){
        printf("ERROR\n");
    }else {printf("Player created sucessfuly");}
    printf("\n");
    printf("\n");
    printf("-Creating a player with id=1\n");
    player=player_create(1);
    if(player==NULL){
        printf("ERROR\n");
    }else {printf("OK\n");}
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Setting player to position to '9'\n");
    if(player_set_space(player, 9)==ERROR){
         printf("ERROR\n");
    }
        else{
        printf("OK, position '9' set correctly\n");

    }
    printf("\n");
        printf("-Setting player to position to null position\n");
    if(player_set_space(player, NO_ID)==ERROR){
         printf("ERROR\n");
    }
        else{
        printf("OK, position '9' set correctly\n");

    }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Setting player name to 'Player1' \n");
    if(player_set_name(player, "Player1")==ERROR){
        printf("ERROR\n");
    }
        else{
        printf("OK, name set correctly\n");

    }
    printf("\n");

    printf("-Setting player name to a wrong name (NULL) \n");
    if(player_set_name(player, NULL)==ERROR){
        printf("ERROR\n");
    }
        else{
        printf("OK, name set correctly\n");

    }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Setting object 'Key1' to the player inventory which id is '1' \n");
    if(player_set_object(player, 1)==ERROR){
        printf("Error setting the object 'Key1\n");
    }
        else{
            printf("Key1 correctly attached to the player's inventory\n");
        }
    printf("\n");
    printf("-Setting object 'Key100' to the player inventory which id is 'NO_ID' \n");
    if(player_set_object(player, NO_ID)==ERROR){
        printf("Error setting the object 'Key100'\n");
    }
        else{
            printf("Key1 correctly attached to the player's inventory\n");
        }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Getting the player's name:\n");
    if(player_get_name(player)==ERROR){
        printf("Error getting the player's name\n");
    }else{
        name=player_get_name(player);
        printf("Name of the player is: %s\n", name);
    }
    printf("\n");
    printf("-Getting the NULL player's name:\n");
    if(player_get_name(Nullplayer)==ERROR){
        printf("Error getting the player's name\n");
    }else{
        name=player_get_name(player);
        printf("Name of the player is: %s\n", name);
    }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Getting the player's id:\n");
    Pid=player_get_id(player);
    if(player_get_id(player)==ERROR){
        printf("ERROR\n");

        }else{
            printf("The player's id is: %ld\n", Pid);
        }
    printf("\n");
    printf("-Getting the Null player's id:\n");
    Pid=player_get_id(Nullplayer);
    if(player_get_id(Nullplayer)==ERROR){
        printf("ERROR\n");

        }else{
            printf("The player's id is: %ld\n", Pid);
        }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Getting the player's number of objects:\n");
    numObjs=player_get_number_of_objects(player);
    if(player_get_number_of_objects(player)==ERROR){
        printf("ERROR\n");
        }else{
            printf("Player num of objects: '%d'",numObjs);
        }
    printf("\n");
    printf("-Getting the Null player's number of objects:\n");
    numObjs=player_get_number_of_objects(Nullplayer);
    if(player_get_number_of_objects(Nullplayer)==ERROR){
        printf("ERROR\n");
        }else{
            printf("Player num of objects: '%d'",numObjs);
        }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Getting the player's max number of objects:\n");
    maxObjs=player_get_max_number_of_objects(player);
        if (!maxObjs){
            printf("ERROR\n");

        }else {
            printf("The max number of objects for player is: %i\n", maxObjs);
        }
    printf("\n");
        printf("-Getting the Null player's max number of objects:\n");
    maxObjs=player_get_max_number_of_objects(Nullplayer);
        if (!maxObjs){
            printf("ERROR\n");

        }else {
            printf("The max number of objects for player is: %i\n", maxObjs);
        }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Getting the player's space:\n");
    id=player_get_space(player);
    if(!id){
        printf("ERROR\n");
    }
    else{
        printf("Player's space is: %ld\n", id);
    }

    if(player_print(player)==ERROR){
        printf("ERROR\n");
    }
    printf("\n");
        printf("-Getting the null player's space:\n");
    id=player_get_space(Nullplayer);
    if(!id){
        printf("ERROR\n");
    }
    else{
        printf("Player's space is: %ld\n", id);
    }

    if(player_print(Nullplayer)==ERROR){
        printf("ERROR\n");
    }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Checks if 'Key1' is in he player's inventory\n");
    if(player_contain_object_id(player, 1)==TRUE){
        printf("OK\n");
    }else{
        printf("ERROR\n");
    }
    printf("-Checks if 'Key2' is in he player's inventory (which shouldn't be\n");   
        if(player_contain_object_id(player, 2)==TRUE){
        printf("OK\n");
    }else{
        printf("ERROR\n");
    }
    printf("\n");
    /*----------------------------------------------------------------------------*/
    printf("Destroying the player's struct\n");
    if(player_destroy(player)==OK){
        printf("Player destroyed correctly\n");
    }else{
        printf("Player wasnt destroyed correcly");
    }printf("-Destroying  a null player's struct\n");
    if(player_destroy(Nullplayer)==OK){
        printf("ERROR\n");
    }else{
        printf("ERROR");
    }
    printf("\n");
    printf("Test finnished, exiting\n");
    return EXIT_SUCCESS;
}