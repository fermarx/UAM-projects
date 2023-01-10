/** 
 * @brief It implements the game interface and all the associated callbacks
 * for each command
 * 
 * @file game.c
 * @author Profesores PPROG
 * @version 1.0 
 * @date 13-01-2015 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "GameManagement.h"
#include "object.h"
#include "player.h"
#include "set.h"
#include "space.h"
#define N_CALLBACK 11
#define MAX_NAME 20

/*Main game struct where everything is stored to play the game*/
struct _Game{
  Object *object[MAX_OBJECTS + 1];
  Player *player;
  Space *spaces[MAX_SPACES + 1];
  T_Command last_cmd;
  STATUS ok;
  Link* links[MAX_LINK];
  char description[MAX_DESCRIPTION];
};
/**
   Define the function type for the callbacks
*/
typedef STATUS (*callback_fn)(Game* game);

/**
   List of callbacks for each command in the game 
*/
STATUS game_callback_unknown(Game* game);
STATUS game_callback_exit(Game* game);
STATUS game_callback_pick(Game *game);
STATUS game_callback_drop(Game *game);
STATUS game_callback_move(Game *game);
STATUS game_callback_inspect (Game *game);
STATUS game_callback_turnOn (Game *game);
STATUS game_callback_turnOff (Game *game);
STATUS game_callback_open(Game* game);
STATUS game_callback_save(Game *game);
STATUS game_callback_load(Game *game);

static callback_fn game_callback_fn_list[N_CALLBACK]={
  game_callback_unknown,
  game_callback_exit,
  game_callback_pick,
  game_callback_drop,
  game_callback_move,
  game_callback_inspect,
  game_callback_turnOn,
  game_callback_turnOff,
  game_callback_open,
  game_callback_save,
  game_callback_load
  };

/**
   Private functions
*/

/*It obtains the space ID at an index in the array*/
Id     game_get_space_id_at(Game* game, int position);



/**
   Game interface implementation
*/

Game* game_create() {
  /*declaration of variables*/
  int i;
  Game* game;
/*memory allocation*/
  game = malloc(sizeof(Game));
  if(game==NULL){
    return NULL;
  }
  /*fields iniziation*/
  for (i = 0; i <= MAX_SPACES; i++) {
    game->spaces[i] = NULL;
  }
  for (i = 0; i <= MAX_OBJECTS; i++){
    game->object[i] = NULL;      
  }
  for (i = 0; i < MAX_LINK; i++){
    game->links[i] = NULL;      
  }
  game->player=player_create(1);
  game->last_cmd = NO_CMD;
  game->ok=OK;
  game->description[0]='\0';
  
  return game;
}

Game* game_create_from_file(char* filename) {
  Game* game;
/*create the game*/
  if ((game=game_create()) == NULL){
    return NULL;
  }
/*loading the information needed to play the game*/
  if (game_load_spaces(game, filename) == ERROR){ 
    return NULL;
  }
  if (game_load_objects(game, filename) == ERROR){ 
    return NULL;
  }
  if (game_load_links(game, filename) == ERROR){  
    return NULL;
  }

  game_set_player_location(game, game_get_space_id_at(game, 4));


  return game;
}

STATUS game_destroy(Game* game) {
  int i;
 /*destroying all the information to exit the game*/
  for (i = 0; (i < MAX_SPACES) && (game->spaces[i] != NULL); i++) {
    space_destroy(game->spaces[i]);
  }
  for (i = 0; (i < MAX_OBJECTS) && (game->object[i] != NULL); i++) {
    object_destroy(game->object[i]);
  }
  for (i = 0; (i < MAX_LINK) && (game->links[i] != NULL); i++) {
    link_destroy(game->links[i]);
  }

  player_destroy(game->player);
  free(game);
  return OK;
}

STATUS game_add_space(Game* game, Space* space) {
  int i = 0;

  if (!space || !game) {
    return ERROR;
  }
/*looking for a free array position free to add a new space*/
  while ( (i < MAX_SPACES) && (game->spaces[i] != NULL)){
    i++;
  }

  if (i >= MAX_SPACES) {
    return ERROR;
  }
/*adding the space in to the free array position*/
  game->spaces[i] = space;

  return OK;
}

STATUS game_add_player(Game *game, Player *player){
  if (!player || !game) {
    return ERROR;
  }
  game->player = player;
  return OK;
}

STATUS game_add_object(Game *game, Object *object){
  int i = 0;
  if (!object || !game) {
    return ERROR;
  }
  /*looking for a free array position free to add a new object*/
  while ( (i < MAX_OBJECTS) && (game->object[i] != NULL)){
    i++;
  }
  if (i >= MAX_OBJECTS) {
    return ERROR;
  }
  /*adding the object to the free array position*/
  game->object[i] = object;
  return OK;
}

STATUS game_add_link(Game* game, Link* link){
  int i = 0;

  if (!link || !game) {
    return ERROR;
  }
/*looking for a free array position free to add a new space*/
  while ( (i < MAX_LINK) && (game->links[i] != NULL) ){
    i++;
  }
  if (i >= MAX_LINK) {
    return ERROR;
  }
/*adding the space in to the free array position*/
  game->links[i] = link;
  return OK;
}



Id game_get_space_id_at(Game* game, int position) {

  if (position < 0 || position >= MAX_SPACES) {
    return NO_ID;
  }
/*returns the space in the n position*/
  return space_get_id(game->spaces[position]);
}

Space* game_get_space(Game* game, Id id){
  int i = 0;
  if(!game){return NULL;}
  if (id == NO_ID) {
    return NULL;
  }
    /*returns the array of spaces*/
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++) {
    if (id == space_get_id(game->spaces[i])){
      return game->spaces[i];
    }
  }
    
  return NULL;
}

Id game_get_player_location(Game *game) {
  if(!game){
    return NO_ID;
  }
  /*returns the player location*/
  return player_get_space(game->player);
}

int game_get_max_objects(Game *game){
  int i=0;
  if(!game){return -1;}
  /*loop till the array is null and returning the number of objects of the object array*/
  for(i=0;game->object[i]!=NULL; i++);
  return i;
}

Id game_get_object_location(Game *game, Id id_object) {
  int i;
  
  if(!game || id_object == NO_ID){
    return NO_ID;
  }
  for(i=0; i<MAX_SPACES; i++){
    /*find if the object id (id_object) is in the space or not*/
    if( set_find(space_get_objects(game->spaces[i]), id_object) ){
      /*if the object is in the space it returns the space id where the object is located*/
      return space_get_id(game->spaces[i]);
    }
  }
  return NO_ID;
}

STATUS game_get_status(Game *game){
  if (!game) return ERROR;
  return game->ok;
}

Object *game_get_object(Game *game, int var){
  if (!game) return NULL;
  return game->object[var];
}

Player *game_get_player(Game *game){
  if (!game) return NULL;
  return game->player;
}

Link* game_get_link_from_id(Game* game, Id id){
  int i=0;
  for(i=0;i<MAX_LINK-19;i++){
    if(link_get_id(game->links[i])==id){
      return game->links[i];
    }
  }
  return NULL;
}

char* game_get_description(Game* game){
  if(!game) 
    return NULL;
  return game->description;
}

Object *game_get_object_from_id(Game *game, Id object_id){
  int i;
  if(object_id == NO_ID){
    return NULL;
  }
  /*searching if there is an object in with the link passed through the function*/
  for(i=0; i<MAX_OBJECTS; i++){
    if(object_id == object_get_id(game->object[i])){
      return game->object[i];
    }
  }
  return NULL;
}

Link* game_get_link_from_position(Game* game, int i) {
  return game->links[i];
}

STATUS game_set_player_location(Game* game, Id id) {
    if(!game || id == NO_ID) {
      return ERROR;
    }
  
  player_set_space(game->player,id);
  return OK;
}

STATUS game_set_player_object(Game *game, Id id){
if(!game || id == NO_ID) {
      return ERROR;
    }
  
  player_set_object(game->player,id);
  return OK;
}

Link * game_get_link_i(Game* game, int i){

    if (!game)
        return NULL;
    return game->links[i];
}

STATUS game_set_object_location(Game* game, Id id_space, Id id_object) {
  /*initialitation of the variables*/
  int i;
  Set *objects;
  Space *newObjectName;
  if(!game){
    return ERROR;
  }
  /*sets the object in the space*/
  for(i=0; i<MAX_SPACES; i++){
    /*if the passed id in in the space, it deletes it*/
    objects=space_get_objects(game->spaces[i]);
    if(set_find(objects, id_object)){
      set_del(objects, id_object);
    }
  }
/*it sets the objects id in to the space passed through the variables */
  newObjectName=game_get_space(game, id_space);
  if(!space_set_object(newObjectName, id_object)){
    return ERROR;
  }
  return OK;
}

STATUS game_update(Game* game, T_Command cmd) {
  if(!game){return ERROR;}
  game->last_cmd = cmd;
  game->ok=(*game_callback_fn_list[cmd])(game);
  return OK;
}

T_Command game_get_last_command(Game* game){
  return game->last_cmd;
}

/*Esta es la funcion en la que ponemos la informacion del dado los objetos etc (NO MODIFICADA)*/
void game_print_data(Game* game) {
  int i;
  int x;
  printf("\n\n-------------\n\n");
  
  /*prints the spaces on the screen*/
  printf("=> Spaces: \n");
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++) {
    space_print(game->spaces[i]);
  }
  /*prints the object location on the screen*/
  printf("=> Object location: \n");
  for(x=0; x<MAX_SPACES; x++){
    if(!space_get_objects(game->spaces[x])){
      printf("%ld", space_get_id(game->spaces[x]));
    }
  }
  /*prints the player location on the screen*/
  printf("=> Player location: %ld\n", player_get_space(game->player));
  printf("prompt:> ");
}


BOOL game_is_over(Game* game) {
  return FALSE;
}

Space* game_get_space_by_position(Game* game, int position){
  if(!game||position<0||position>MAX_SPACES)
    return NULL;
  return game->spaces[position];
}

Link* game_get_link_by_position(Game* game, int position){
  if(!game||position<0||position>MAX_SPACES)
    return NULL;
  return game->links[position];
}
/**
   Callbacks implementation for each command 
*/

STATUS game_callback_unknown(Game* game) {
  return OK;
}

STATUS game_callback_exit(Game* game) {
  return OK;
}

STATUS game_callback_pick(Game *game){
  /*declaration of variables*/
  int i=0;
  char *buff=NULL;
  Id player_space = NO_ID;
  Id object_space = NO_ID;
  Set *find=NULL;
  Id idobject=NO_ID;
  char name[MAX_NAME];
  if(!game){return ERROR;}
/*si el numero de objetos que tiene el player es mayor que el numero de objetos  maximos que puede tener devuelve error*/
  scanf("%s", name);
  if(inventory_is_full(player_get_inventory(game_get_player(game)))){return ERROR;}
  player_space = player_get_space(game_get_player(game));
  find=space_get_objects(game_get_space(game,player_space));
/*viendo si los objetos de ese espacio tienen el nombre que busca. si sí, guarda su id en idobject*/
  for(i=0; i<set_get_num_ids(space_get_objects(game_get_space(game,player_space))); i++){
    buff=object_get_name(game_get_object_from_id(game,set_obtain_Id_by_position(find,i)));
    if(!strcmp(buff,name)){
      idobject=set_obtain_Id_by_position(find,i);
      break;
    }
  }
  if(object_get_movable(game_get_object_from_id(game, idobject))==TRUE){
    object_space = game_get_object_location(game, idobject);
    /*id donde se encuentra el player es igual al id donde se encuentra el object*/
    if(player_space==object_space){
      player_set_object(game_get_player(game), idobject);
    /*  NOPE =>  set_add(inventory_get_objects(player_get_inventory( game->player)), idobject);*/
      game_set_object_location(game, NO_ID, idobject);
    }  
    if(idobject==NO_ID){return ERROR;}
    return OK;
  }
  return ERROR;
}

STATUS game_callback_drop(Game *game){
  /*declaraiton of variables*/
  Id playerlocation;
  char name[MAX_NAME];
  int i;
  Id obj_req, *obj_array;
  Object* obj=NULL;
  scanf ("%s", name);
  for(i=0;game->object[i]!=NULL;i++){
    if(strcmp(name, object_get_name(game->object[i]))==0){
      obj=game->object[i];
      break;
    }
  }
  obj_array=player_get_objects_array(game->player);
  if(player_contain_object_id(game->player, object_get_id(obj))==TRUE){
    playerlocation = game_get_player_location(game);/*loaclizacion del jugador*/
    if(playerlocation == NO_ID) return ERROR;/*el id del player no es null*/
    if(set_is_full(space_get_objects(game_get_space(game,playerlocation)))==TRUE)return ERROR;/*el espacio no está lleno*/
    for (i=0; i<inventory_get_max_obj(player_get_inventory(game->player)); i++){
      /*id del objeto que quieres*/
      obj_req = obj_array[i];
      if (strcmp(name, object_get_name(game_get_object_from_id(game, obj_req)))==0){
        /*compara el nombre que has metido con el nombre del objeto que el player tiene*/
        break;
      }
    }
    game_set_object_location(game,playerlocation,obj_req);
    inventory_delete_object(player_get_inventory(game->player),obj_req);
    if(playerlocation!=object_get_starter_loc(obj)){
      object_set_moved(obj, TRUE);
    }
    return OK;
  }
  return ERROR;
}

STATUS game_callback_move(Game *game){
  /*decaration of variables*/
  char name[MAX_NAME];
  int i = 0;
  Id space_id = NO_ID, next_space;
  Space* space_act=NULL;
  Link* lk=NULL;
  if (!game)
    return ERROR;
  /*scans the wanted direction*/
  scanf("%s", name);
  space_id=game_get_player_location(game);
  if (NO_ID == space_id ){
    return ERROR;
  }
  space_act=game_get_space(game, space_id);
  /*saves the link and next location based on the input*/
  if (strcmp(name, "north") == 0 || strcmp(name, "n") == 0 || strcmp(name, "N") == 0 || strcmp(name, "North") == 0){
    /*gets the link from the game and link that space has stored in the in the space->north*/
    lk=game_get_link_from_id(game, space_get_north(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else if (strcmp(name, "south") == 0 || strcmp(name, "s") == 0 || strcmp(name, "S") == 0 || strcmp(name, "South") == 0){
    /*gets the link from the game and link that space has stored in the in the space->south*/
    lk=game_get_link_from_id(game, space_get_south(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else if (strcmp(name, "east") == 0 || strcmp(name, "e") == 0 || strcmp(name, "E") == 0 || strcmp(name, "East") == 0){
    /*gets the link from the game and link that space has stored in the in the space->east*/
    lk=game_get_link_from_id(game, space_get_east(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else if (strcmp(name, "west") == 0 || strcmp(name, "w") == 0 || strcmp(name, "W") == 0 || strcmp(name, "West") == 0){
    /*gets the link from the game and link that space has stored in the in the space->west*/
    lk=game_get_link_from_id(game, space_get_west(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else if (strcmp(name, "up") == 0 || strcmp(name, "u") == 0 || strcmp(name, "U") == 0 || strcmp(name, "Up") == 0){
    /*gets the link from the game and link that space has stored in the in the space->up*/
    lk=game_get_link_from_id(game, space_get_up(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else if (strcmp(name, "down") == 0 || strcmp(name, "d") == 0 || strcmp(name, "D") == 0 || strcmp(name, "Down") == 0){
    /*gets the link from the game and link that space has stored in the in the space->down*/
    lk=game_get_link_from_id(game, space_get_down(space_act));
    /*now next space has the id of the next location*/
    next_space=link_get_connection(space_id, lk);
  }
  else{
    return ERROR;
  }
  /*sets the location*/
  if (next_space != NO_ID && link_get_state(lk)==TRUE) {
    game_set_player_location(game, next_space);
    for(i=0; game->object[i]!=NULL;i++){
      if((next_space!=object_get_starter_loc(game->object[i]))&&(player_contain_object_id(game->player, object_get_id(game->object[i])==TRUE))){
        object_set_moved(game->object[i], TRUE);
      }
    }  
    /*if an object thet the player is carring is turend on it illuminates the space*/
    for(i=0; game->object[i]!=NULL;i++){
      if(object_get_turnedOn(game->object[i])==TRUE){
        if(player_contain_object_id(game->player, object_get_id(game->object[i]))==TRUE){
          space_set_illumination(game_get_space(game, next_space), TRUE);
        }
        else if(set_find(space_get_objects(space_act),object_get_id(game->object[i]))==TRUE){
          return OK;
        }
      }
    }  
    space_set_illumination(space_act, FALSE);
    return OK;
  }
  return ERROR;
} 
  STATUS game_callback_inspect(Game* game){
/*declaration of varables*/
  char name [MAX_NAME];
  int i;
  Object* obj=NULL;
  Space* space=NULL;
/*error control*/
  if (!game) return ERROR;
  space=game_get_space(game, game_get_player_location(game));
  scanf("%s", name);
  /*if the space is illumiated and the introduced word is space, it will get the description of the space*/
  if(space_get_illumination(space)==TRUE){
    if(!strcmp(name, "space") || !strcmp(name, "Space") || !strcmp(name, "s") || !strcmp(name, "S")){
      strcpy(game->description, space_get_description(space));
      return OK;
    }
    /*checks if the word you enteres is on the backpack of the player*/
    for(i=0;game->object[i]!=NULL;i++){
      if(strcmp(name, object_get_name(game->object[i]))==0){
        obj=game->object[i];
        break;
      }
    }
    /*if the space is illuminated it will check if the word that you enterd is on the space you are in*/
    if(space_contain_object(space, object_get_id(obj))==TRUE||player_contain_object_id(game_get_player(game), object_get_id(obj))==TRUE){
      if(obj!=NULL){
        if(object_get_moved(obj)==FALSE){
          strcpy(game->description, object_get_description(obj));
        }else {
          strcpy(game->description, object_get_moved_description(obj));
        }
        return OK;
      }
    }
  }
  return ERROR;

}

STATUS game_callback_turnOn(Game *game){
  /*declaration of variables*/
  char name[MAX_NAME];
  Object* obj=NULL;
  int i=0;

  if(!game)
    return ERROR;
  scanf("%s", name);
  /*checks if the naem you entered is in the object array*/
  for(i=0;game->object[i]!=NULL;i++){
    if(strcmp(name, object_get_name(game->object[i]))==0){
      obj=game->object[i];
      break;
    }
  }
  /*if the player has the object in the backpack and the object can be turned on, it will turn on the object*/
  if(player_contain_object_id(game_get_player(game), object_get_id(obj))==TRUE){
    if(object_get_illuminate(obj)==TRUE && object_get_turnedOn(obj)==FALSE){
      object_set_turnedOn(obj, TRUE);
      space_set_illumination(game_get_space(game, game_get_player_location(game)), TRUE);
      return OK;
    }
  }
  return ERROR;
}

STATUS game_callback_turnOff(Game *game){
  char name[MAX_NAME];
  Object* obj=NULL;
  int i=0;

  if(!game)
    return ERROR;

  scanf("%s", name);

  /*looks for the object with scanned name*/
  for(i=0;game->object[i]!=NULL;i++){
    if(strcmp(name, object_get_name(game->object[i]))==0){
      obj=game->object[i];
      break;
    }
  }

  /*if the player contains the object and it can be turned off, it is turned off*/
  if(player_contain_object_id(game_get_player(game), object_get_id(obj))==TRUE){
    if(object_get_illuminate(obj)==TRUE && object_get_turnedOn(obj)==TRUE){
      object_set_turnedOn(obj, FALSE);
      
      /*looks for another turned on object in the space, if there is, it returns OK*/
      for(i=0; game->object[i]!=NULL;i++){
        if(object_get_turnedOn(game->object[i])==TRUE){
          if(set_find(space_get_objects(game_get_space(game, game_get_player_location(game))),object_get_id(game->object[i]))==TRUE || player_contain_object_id(game->player, object_get_id(game->object[i]))==TRUE){
            return OK;
          }
        }
      }  

      /*if there wasnt another turned on object, sets the illumination of the space to FALSE*/
      space_set_illumination(game_get_space(game, game_get_player_location(game)), FALSE);
      return OK;
    }
  }
  return ERROR;
}

STATUS game_callback_open(Game* game){
  char link_name[MAX_NAME];
  char obj_name[MAX_NAME];
  Link* lk=NULL;
  int i, j;
  Space* space_act=NULL;
  Object* obj=NULL;

  if(!game) return ERROR;
  scanf("%s with %s", link_name, obj_name);

  space_act=game_get_space(game, game_get_player_location(game));
  /*looks for the link with scanned name*/
  for(i=0;game->links[i]!=NULL;i++){
    lk=game->links[i];
    if(!strcmp(link_get_name(lk), link_name) && (space_contain_link(space_act, link_get_id(lk))==TRUE)){
      
      /*looks for the object with scanned name*/    
      for(j=0;game->object[j]!=NULL;j++){
        obj=game->object[j];
        if((!strcmp(object_get_name(obj), obj_name))&&(player_contain_object_id(game->player, object_get_id(obj)))&&(object_get_open(obj)==link_get_id(lk))){
          
          /*sets the state of the link to open*/
          link_set_state(lk, TRUE);
          return OK;
        }
      }
    }
  }
  return ERROR;
}

STATUS game_callback_save(Game *game){
  if(gamemanagement_save(game, "save.txt")==ERROR)return ERROR;
  return OK;
}

STATUS game_callback_load(Game *game){
  game_destroy(game);
  game = gamemanagement_load(game, "save.txt");
  if (!game)return ERROR;
  return OK;
}


