#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"
#include "types.h"
#include "space.h"
#include "set.h"
#include "game.h"
#define MAX_HORI 7
#define MAX_VERT 3
#define MAX_DESCRIPTION 100

/*It stores the connexions of the space*/
struct _Space{
  Id id;
  char name[WORD_SIZE + 1];
  Id north;
  Id south;
  Id east;
  Id west;
  Id up;
  Id down;
  Set *objects;
  BOOL illuminated;
  char description[MAX_DESCRIPTION];
  char shortDescription[MAX_DESCRIPTION];
  char gdesc[MAX_VERT][MAX_HORI+1];
};

Space* space_create(Id id) {
  /*initializating variables*/
  Space *newSpace = NULL;
  if (id == NO_ID)return NULL;
  /*Allocating the sapace memory*/
  newSpace = (Space*) malloc(sizeof (Space));
  if (newSpace == NULL) return NULL;
  /*field initialization*/
  newSpace->id = id;
  newSpace->east=NO_ID;
  newSpace->west=NO_ID;
  newSpace->north=NO_ID;
  newSpace->south=NO_ID;
  newSpace->up=NO_ID;
  newSpace->down=NO_ID;
  newSpace->illuminated=FALSE;
  newSpace->name[0] = '\0';
  newSpace->description[0] = '\0';
  newSpace->shortDescription[0] = '\0';
  newSpace->objects = set_ini();
  strcpy(newSpace->gdesc[0],"       ");
  strcpy(newSpace->gdesc[1],"       ");
  strcpy(newSpace->gdesc[2],"       ");
  return newSpace;
}
/*destroys the sets of the space and frees it*/
STATUS space_destroy(Space* space) {
  if (!space) return ERROR;
  set_destroy(space->objects);
  free(space);
  return OK;
}
/*sets the name of the space*/
STATUS space_set_name(Space* space, char* name) {
  if (!space || !name) return ERROR;
  if (!strcpy(space->name, name))return ERROR;
  return OK;
}
/*sets the id of the north*/
STATUS space_set_north(Space* space, Id id) {
  if (!space) return ERROR;
  space->north=id;
  return OK;
}
/*sets the id of the south*/
STATUS space_set_south(Space* space, Id id) {
  if (!space) return ERROR;
  space->south=id;
  return OK;
}
/*sets the id of the east*/
STATUS space_set_east(Space* space, Id id) {
  if (!space) return ERROR;
  space->east=id;
  return OK;
}
/*sets the id of the west*/
STATUS space_set_west(Space* space, Id id) {
  if (!space) return ERROR;
  space->west=id;
  return OK;
}
/*sets the id of up*/
STATUS space_set_up(Space* space, Id id) {
  if (!space) return ERROR;
  space->up=id;
  return OK;
}
/*sets the id of down*/
STATUS space_set_down(Space* space, Id id) {
  if (!space) return ERROR;
  space->down=id;
  return OK;
}
/*sets if an object can illuminate or not*/
STATUS space_set_illumination(Space* space, BOOL state){
  if(!space) return ERROR;
  space->illuminated=state;
  return OK;
}
/*adds an object in to a space*/
STATUS space_set_object(Space* space, Id id_object) {
  if (!space) return ERROR;
  if(!set_add(space->objects, id_object))return ERROR;
  return OK;
}
/*gets the name of an space*/
const char *space_get_name(Space* space) {
  if (!space) return NULL;
  return space->name;  
}
/*returns the id of the space*/
Id space_get_id(Space* space){
  if (!space) return NO_ID;
  return space->id;
}
/*returns the id of the north*/
Id space_get_north(Space* space){  
  if (!space) return NO_ID;
  return space->north;
}
/*returns the id of the south*/
Id space_get_south(Space* space) {
  if (!space) return NO_ID;
  return space->south;
}
/*returns the id of the east*/
Id space_get_east(Space* space) {
  if (!space) return NO_ID;
  return space->east;
}
/*returns the id of the west*/
Id space_get_west(Space* space) {
  if (!space) return NO_ID;
  return space->west;
}
/*returns the id of up*/
Id space_get_up(Space* space) {
  if (!space) return NO_ID;
  return space->up;
}
/*returns the id of down*/
Id space_get_down(Space* space) {
  if (!space) return NO_ID;
  return space->down;
}
/*returns if an object can illuminate or not*/
BOOL space_get_illumination(Space* space){
  if(!space) return FALSE;
  return space->illuminated;
}
/*returns an array of objects*/
Set* space_get_objects(Space* space){
  if (!space) return NULL;
  return space->objects;
}

STATUS space_print(Space* space) {
  Id idaux = NO_ID;
  BOOL state;

  if (!space) {
    return ERROR;
  }

  fprintf(stdout, "--> Space (Id: %ld; Name: %s)\n", space->id, space->name);

  idaux = space_get_north(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> North link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No north link.\n");
  }

  idaux = space_get_south(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> South link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No south link.\n");
  }

  idaux = space_get_east(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> East link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No east link.\n");
  }

  idaux = space_get_west(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> West link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No west link.\n");
  }

  idaux = space_get_up(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> Up link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No up link.\n");
  }

  idaux = space_get_down(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> Down link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No down link.\n");
  }

  state=space_get_illumination(space);
  if(state==TRUE){
    fprintf(stdout, "---> Space illuminated.\n");
  } else{
    fprintf(stdout, "--> Space not illuminated.\n");
  }

  if (space_get_objects(space)!=NULL) {
    fprintf(stdout, "Objects: ");
    set_print(stdout, space_get_objects(space));
    } else {
    fprintf(stdout, "---> No object in the space.\n");
  }
  printf("%s", space->gdesc[0]);
  printf("%s", space->gdesc[1]);
  printf("%s", space->gdesc[2]);
  return OK;
}
/*returns the array of chars that later are going to be printed in the board*/
char *space_get_gdesc(Space *space, int line){
  if(!space){return NULL;}
  if(!(space->gdesc)){return NULL;}
  return space->gdesc[line];  
}
/*returns the array of chars that later are going to be printed in the board*/
STATUS space_set_gdesc(Space *space, char *gdesc, int line){
  if(!space||!gdesc){return ERROR;}
  strcpy(space->gdesc[line],gdesc);
  return OK;
}
/*sets the description of an object*/
STATUS space_set_description(Space* s, char* desc){
  if(!s)return ERROR;
  strcpy(s->description, desc);
  return OK;
}
/*returns the description of an object*/
char* space_get_description(Space *space){
  if (!space)return NULL;
  return space->description;
}
/*returns true if the space contains the id of the object you have passed*/
BOOL space_contain_object(Space* space, Id id){
  if (!space || id==NO_ID)return FALSE;
  return set_find (space->objects, id);
}
/*gets the shoert description of an object*/
char* space_get_short_description(Space* space){
  if(!space) return NULL;
  return space->shortDescription;
}
/*sets the short description of an object*/
STATUS space_set_short_description(Space* space, char* description){
  if(!space) return ERROR;
  strcpy(space->shortDescription, description);
  return OK;
}
/*returns true if the space contains the id of the link that you have passed*/
BOOL space_contain_link(Space* space, Id id){
  if(space->north==id||space->south==id||space->east==id||space->west==id||space->up==id||space->down==id){
    return TRUE;
  }
  return FALSE;
}