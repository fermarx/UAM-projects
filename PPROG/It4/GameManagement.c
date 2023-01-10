#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameManagement.h"
#include "game.h"
#include "link.h"

STATUS game_load_spaces(Game* game, char* filename) {
  /*initialitation of the variables*/
  FILE* file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char* toks = NULL;
  char *gdesc;
  char *gdesc2;
  char *gdesc3;
  char description[MAX_DESCRIPTION];
  char shortDescription[MAX_DESCRIPTION];
  int illumination;
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID, up=NO_ID, down=NO_ID;
  Space* space = NULL;
  STATUS status = OK;
  
  if (!filename) {
    return ERROR;
  }
  /*opening the file*/
  file = fopen(filename, "r");
  if (file == NULL) {
    return ERROR;
  }
  /*reade the lines that starts with #s: and when it detects a | keeps saving the data that it reads in another field
    if it detects a & it prints the array of characters on the board
   */
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#s:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      north = atol(toks);
      toks = strtok(NULL, "|");
      east = atol(toks);
      toks = strtok(NULL, "|");
      south = atol(toks);
      toks = strtok(NULL, "|");
      west = atol(toks);
      toks = strtok(NULL, "|");
      up = atol(toks);
      toks = strtok(NULL, "|");
      down = atol(toks);
      toks=strtok(NULL, "|");
      strcpy(description,toks);
      toks=strtok(NULL,"|");
      strcpy(shortDescription,toks);
      toks=strtok(NULL, "&");
      illumination=atol(toks);
      gdesc =strtok(NULL,"&");
      gdesc2=strtok(NULL,"&");
      gdesc3=strtok(NULL,"&");


#ifdef DEBUG 
      printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
/*creates the space with the data read from the file*/
      space = space_create(id);
      if (space != NULL) {
	      space_set_name(space, name);
	      space_set_north(space, north);
	      space_set_east(space, east);
	      space_set_south(space, south);
	      space_set_west(space, west);
        space_set_up(space, up);
        space_set_down(space, down);
        space_set_illumination(space, illumination);
        space_set_description(space, description);
        space_set_short_description(space, shortDescription);
        space_set_gdesc(space,gdesc,0);
        space_set_gdesc(space,gdesc2, 1);
        space_set_gdesc(space,gdesc3, 2);
        game_add_space(game, space);
      
      }
    }
  }
  if (ferror(file)) {
    status = ERROR;
  }
  fclose(file);
  return status;
}

STATUS game_load_objects(Game* game, char* filename) {
  /*Var declaration*/
  FILE* file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char* toks = NULL;
  Object *object;
  Id id = NO_ID, spaceid = NO_ID, open=NO_ID;
  STATUS status = OK;
  char description[MAX_DESCRIPTION];
  char moved_description[MAX_DESCRIPTION];
  int illumination, movable, moved, hidden, turnedOn;
 
  if (!filename) { /*Error control*/
    return ERROR;
  }
  
  file = fopen(filename, "r"); /*Opening file*/
  if (file == NULL) {
    return ERROR;
  }
  /*reads the lines that starts with #o: and when it detects a | keeps saving the data that it reads in another field */
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#o:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      spaceid = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(description,toks);
      toks=strtok(NULL, "|");
      strcpy(moved_description, toks);
      toks = strtok(NULL,"|");
      illumination=atol(toks);
      toks=strtok(NULL, "|");
      open=atol(toks);
      toks=strtok(NULL, "|");
      movable=atol(toks);
      toks=strtok(NULL, "|");
      moved=atol(toks);
      toks=strtok(NULL, "|");
      hidden=atol(toks);
      toks=strtok(NULL, "|");
      turnedOn=atol(toks);
      /*creates the object with the data read from the file*/
      object = object_create(id);
      if (object != NULL) {
        object_set_name(object, name);
        object_set_description(object, description);
        object_set_moved_description(object, moved_description);
        object_set_illuminate(object,illumination);
        object_set_starter_loc(object, spaceid);
        object_set_open(object, open);
        object_set_movable(object, movable);
        object_set_hidden(object, hidden);
        object_set_moved(object, moved);
        object_set_turnedOn(object, turnedOn);
        game_add_object(game, object);
        if(spaceid==NO_ID){
          player_set_object(game_get_player(game), id);
          fprintf (stderr, "%ld", id);
        }else{
          game_set_object_location(game, spaceid, id);
        }
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
  }
  
  fclose(file);

  return status;
}


STATUS game_load_links(Game *game,char* filename){
  /*Var declaration*/
  FILE* file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char* toks = NULL;
  int open;
  Link* link=NULL;
  Id id = NO_ID, first = NO_ID, second= NO_ID;
  STATUS status = OK;
  if (!filename) { /*Error control*/
    return ERROR;
  }
  file = fopen(filename, "r");
  if (file == NULL) {
    return ERROR;
  }
/*reads the lines that starts with #l: and when it detects a | keeps saving the data that it reads in another field */
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#l:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      first = atol(toks);
      toks = strtok(NULL, "|");
      second = atol(toks);
      toks=strtok(NULL, "|");
      open=atol(toks);
/*creates the links with the data read from the file*/
      link = link_create(id);
      if (link != NULL) {
        link_set_name(link, name);
        link_set_connect(link, first, second);
        link_set_state(link, open);
        game_add_link(game, link);
      }
    }
  }
 if (ferror(file)) {
    status = ERROR;
  }
  
  fclose(file);
  
  return status;
 }

STATUS gamemanagement_save(Game* game, char* filename){
  /*Var declaration*/
  int i,j=0;
  Id* obj;
  Space* sp=NULL;
  Link* lk=NULL;
  FILE* f;

  f=fopen(filename,"w"); /*Creating the save file*/

  if(f==NULL){
    return ERROR;
  }
/*----------------------------------------------------------------------------*/

  
  fprintf(f, "#p:%ld|%ld", player_get_id(game_get_player(game)), game_get_player_location(game));
  fprintf(f, "\n");
  /*----------------------------------------------------------------------------*/
    /*saves the objects of the game*/
    
    for(i=0;i<MAX_SPACES && game_get_space_by_position(game, i)!=NULL;i++){ 
    
      obj=set_get_ids(space_get_objects(game_get_space_by_position(game, i)));
      for(j=0;game_get_object_from_id(game,obj[j])!=NULL;j++){
        /*Id, name, location, normal description, moved description, can iluminate, open, movable, moved, hidden, turnedOn */
        fprintf(f,"#o:%ld|%s|%ld|%s|%s|%d|%ld|%d|%d|%d|%d|\n", obj[j], object_get_name(game_get_object_from_id(game,obj[j])),space_get_id(game_get_space_by_position(game, i)),object_get_description(game_get_object_from_id(game,obj[j])), object_get_moved_description(game_get_object_from_id(game,obj[j])),object_get_illuminate(game_get_object_from_id(game,obj[j])),object_get_open(game_get_object_from_id(game,obj[j])), object_get_movable(game_get_object_from_id(game,obj[j])), object_get_moved(game_get_object_from_id(game,obj[j])), object_get_hidden(game_get_object_from_id(game,obj[j])), object_get_turnedOn(game_get_object_from_id(game,obj[j])));
      }
    }fprintf(f, "\n");


    /*Saves player's objects*/

    obj=player_get_objects_array(game_get_player(game));

  
    for(i=0; game_get_object_from_id(game, obj[i])!=NULL;i++){
    /*Id, name, normal description, moved description*/
      fprintf(f,"#o:%ld|%s|-1|%s|%s|%d|%ld|%d|%d|%d|%d|\n", obj[i], object_get_name(game_get_object_from_id(game,obj[i])),object_get_description(game_get_object_from_id(game,obj[i])), object_get_moved_description(game_get_object_from_id(game,obj[i])),object_get_illuminate(game_get_object_from_id(game,obj[i])),object_get_open(game_get_object_from_id(game,obj[i])), object_get_movable(game_get_object_from_id(game,obj[i])), object_get_moved(game_get_object_from_id(game,obj[i])), object_get_hidden(game_get_object_from_id(game,obj[i])), object_get_turnedOn(game_get_object_from_id(game,obj[i])));
    }  
  fprintf(f, "\n");
  for(i=0;i<MAX_SPACES && game_get_space_by_position(game, i)!=NULL;i++){
    sp=game_get_space_by_position(game, i);
    if(sp){
      /*id, name, north, east, sout, west, up, down, illumination, descr, short descr, gdesc1, gdesc2, gdesc3*/
      if ( strcmp( space_get_gdesc(sp, 0),"       ")==0){
        fprintf(f, "#s:%ld|%s|%ld|%ld|%ld|%ld|%ld|%ld|%s|%s|%d\n", space_get_id(sp), space_get_name(sp), space_get_north(sp),space_get_east(sp), space_get_south(sp), space_get_west(sp), space_get_up(sp),space_get_down(sp), space_get_description(sp), space_get_short_description(sp),space_get_illumination(sp));
      }else{
        fprintf(f,"#s:%ld|%s|%ld|%ld|%ld|%ld|%ld|%ld|%s|%s|%d&%s&%s&%s&\n", space_get_id(sp), space_get_name(sp), space_get_north(sp),space_get_east(sp), space_get_south(sp), space_get_west(sp), space_get_up(sp),space_get_down(sp), space_get_description(sp), space_get_short_description(sp),space_get_illumination(sp), space_get_gdesc(sp, 0), space_get_gdesc(sp, 1), space_get_gdesc(sp, 2));
      }
    }
  } fprintf(f, "\n");
  for(i=0;i<MAX_LINK;i++){
    lk=game_get_link_by_position(game, i);
    if(lk){
      /*id, name, first, secind, state*/
      fprintf(f, "#l:%ld|%s|%ld|%ld|%d\n", link_get_id(lk), link_get_name(lk), link_get_first(lk), link_get_second(lk), link_get_state(lk));
    }
  } fprintf(f, "\n");
  fclose(f);
  return OK;
 }

STATUS game_load_player(Game* game, char* filename){
  /*Var declaration*/
  FILE* file = NULL;
  char line[WORD_SIZE] = "";
  char* toks = NULL;
  Id id = NO_ID, location= NO_ID;
  STATUS status = OK;
  Player* pl=NULL;

  if (!filename) {  /*Error control*/
    return ERROR;
  }
  
  file = fopen(filename, "r"); /*File opening*/
  if (file == NULL) {
    return ERROR;
  }

  /*reads the lines that starts with #p: and when it detects a | keeps saving the data that it reads in another field */
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#p:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      location=atol(toks);

      fprintf(stderr, "%li", location);

  /*creates the player with the data read from the file*/
      pl = player_create(id);
      if (pl != NULL) {
        player_set_space(pl, location);
        game_add_player(game, pl);
      }
    }
  }
 if (ferror(file)) { /*Error control*/
    status = ERROR;
  }
  
  fclose(file);
  
  return status;
}

Game* gamemanagement_load(Game *game, char* filename){
  Id obj_id[MAX_OBJECTS];
  int i;
  if( !filename)return NULL;

  /*load the spaces, links and objects*/
  game=game_create_from_file(filename);
  if(!game){
  return ERROR;
  }
  /*loads the player (id,location and objects */
    if(game_load_player(game, filename)==ERROR){
    return ERROR;
  }
  game_set_player_location(game, player_get_space(game_get_player(game)));
  
  for(i =0; i<MAX_OBJECTS;i++){
    obj_id[i]=NO_ID;
  }
  for(i=0 ; obj_id[i]!=NO_ID ; i++){  
    game_set_player_object(game,  obj_id[i]);
  }
  return game;
}