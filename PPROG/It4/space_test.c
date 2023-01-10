#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "space.h"
#include "types.h"

int main(){
Space *newspace=NULL, *Nullspace=NULL;
Id Nid=NO_ID, id=1, getid=NO_ID;
BOOL boolean=TRUE;
Set *objs=NULL;
char *name=NULL, *description="Normal description",*shortdesc=NULL, *testgdesc=NULL, *tsd="Test short description";
/*-----------------------------------------------------------------------------------------------------*/
printf("Creating a new space\n");
newspace=space_create(id);
if (newspace==NULL){
    printf("ERROR: creating space\n");
}else{
    printf("OK, space created correctly\n");
}
printf("Creating a new space with NULL id\n");
printf("\n");
newspace=space_create(Nid);
if (newspace==NULL){
    printf("ERROR: creating space\n");
}else{
    printf("OK, space created correctly\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/

printf("Getting the space id which was 1\n");
getid=space_get_id(newspace);
if(getid==NO_ID){
    printf("ERROR");
}else{
    printf("OK");
}printf("\n");

/*-----------------------------------------------------------------------------------------------------*/

printf("Setting space name to spacename1\n");
if(space_set_name(newspace, "spacename1")==OK){
    printf("OK, name set\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Setting null space name to spacename1\n");
name=NULL;
if(!name){
    printf("ERROR\n");
}else {
    printf("OK, the space name is: %s", name);
}printf("\n");

/*-----------------------------------------------------------------------------------------------------*/

printf("Setting space north id to 1\n");
if(space_set_north(newspace, id)==ERROR){
    printf("ERROR\n");
}
else{
    printf("OK\n");
}printf("\n");
printf("Setting null space north id to 1\n");
if(space_set_north(Nullspace, id)==ERROR){
    printf("ERROR\n");
}
else{
    printf("OK\n");
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space north id\n");
id=space_get_north(newspace);
if(!id){
    printf("ERROR\n");
}
else{
    printf("OK, id is %ld\n", id);
}printf("\n");
printf("Getting nullspace north id\n");
id=space_get_north(Nullspace);
if(!id){
    printf("ERROR\n");
}
else{
    printf("OK, north id is %ld\n", id);
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space south id to 2\n");
if(space_set_south(newspace, id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Setting the null space south id to 2\n");
if(space_set_south(Nullspace, id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");

/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space south id\n");
id=space_get_south(newspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK, south id is %ld\n", id);
}printf("\n");
printf("Getting nullspace south id\n");
id=space_get_south(Nullspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK\n");
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space east id to 3\n");
if(space_set_east(newspace, id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Setting the null space east id to 3\n");
if(space_set_east(Nullspace, id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space east id\n");
id=space_get_east(newspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK, east id is %ld\n", id);
}printf("\n");
printf("Getting nullspace east id\n");
id=space_get_east(Nullspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK\n");
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space west id to 4\n");
if(space_set_west(newspace, id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
printf("Setting the null space west id to 4\n");
if(space_set_east(Nullspace, id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space west id\n");
id=space_get_west(newspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK, west id is %ld\n", id);
}
printf("\n");
printf("Getting nullspace west id\n");
id=space_get_west(Nullspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space up id to 5\n");
if(space_set_up(newspace, id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
printf("Setting the null space up id to 5\n");
if(space_set_up(Nullspace, id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space up id\n");
id=space_get_up(newspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK, up id is %ld\n", id);
}
printf("\n");
printf("Getting nullspace up id\n");
id=space_get_up(Nullspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space down id to 6\n");
if(space_set_down(newspace, id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
printf("Setting the null space down id to 6\n");
if(space_set_down(Nullspace, id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space down id\n");
id=space_get_down(newspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK, up id is %ld\n", id);
}
printf("\n");
printf("Getting nullspace down id\n");
id=space_get_down(Nullspace);
if(id==NO_ID){
    printf("ERROR\n");
}else{
    printf("OK\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space ilumination to TRUE\n");
if(space_set_illumination(newspace,TRUE)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
printf("Setting the null space ilumination to TRUE\n");
if(space_set_illumination(Nullspace, TRUE)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting space illumination state\n");
boolean=space_get_illumination(newspace);
if(boolean==FALSE){
    printf("Ilumination: false\n");
}else{
    printf("Ilumination: true\n");
}
printf("\n");
printf("Getting nullspace illumination state\n");
boolean=space_get_illumination(Nullspace);
if(boolean==FALSE){
    printf("Ilumination: false\n");
}else{
    printf("Ilumination: true\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
id=1;
printf("Setting an object to the space with id 1\n");
if(space_set_object(newspace,id)==OK){
    printf("OK\n");
}
else{
    printf("ERROR\n");
}
printf("\n");
printf("Setting an object to the null space with id 1\n");
if(space_set_illumination(Nullspace, id)==OK){
    printf("OK\n");
}
else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting the objects in the space\n");
objs=space_get_objects(newspace);
if(objs==NULL){
    printf("ERROR\n");

}else{
    printf("OK\n");
}printf("\n");
printf("Getting the objects in the null space\n");
objs=space_get_objects(Nullspace);
if(objs==NULL){
    printf("ERROR\n");

}else{
    printf("OK\n");
}
/*-----------------------------------------------------------------------------------------------------*/
printf("Printing the objects of the space\n");
if(space_print(newspace)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");

printf("Printing the objects of the null space\n");
if(space_print(Nullspace)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space description\n");
if(space_set_description(newspace, "test description")==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Setting the null space description\n");
if(space_set_description(Nullspace, "test description")==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting the space description\n");
description=space_get_description(newspace);
if(description==NULL){
    printf("ERROR\n");
}else{
    printf("OK, the description is %s",description);
}
printf("\n");
printf("Getting the null space description\n");
description=space_get_description(Nullspace);
if(description==NULL){
    printf("ERROR\n");
}else{
    printf("OK, the description is %s",description);
}
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting the boolean if space contains objects or not\n");
if(space_contain_object(newspace,id)==TRUE){
    printf("OK\n");

}else{
    printf("ERROR\n");
}printf("\n");
printf("Getting the boolean if null space contains objects or not\n");
if(space_contain_object(Nullspace,id)==TRUE){
    printf("OK\n");

}else{
    printf("ERROR\n");
}
/*-----------------------------------------------------------------------------------------------------*/
printf("Setting the space short description\n");
if(space_set_description(newspace, tsd)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Setting the null space short description\n");
if(space_set_description(Nullspace, tsd)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Getting the space short description\n");
shortdesc=space_get_short_description(newspace);
if(shortdesc==NULL){
    printf("ERROR\n");
}else{
    printf("OK, the description is %s",shortdesc);
}
printf("\n");
printf("Getting the null space short description\n");
shortdesc=space_get_short_description(Nullspace);
if(shortdesc==NULL){
    printf("ERROR\n");
}else{
    printf("OK, the description is %s",shortdesc);
}
/*-----------------------------------------------------------------------------------------------------*/
BOOL space_contain_link(Space* space, Id id);
printf("Checking if the space has a link with south, which was '2' \n");
if(space_contain_link(newspace, id++)==TRUE){
    printf("OK, the space is linked with south\n");
}else{
    printf("ERROR, the space isnt linked with south\n");
}
printf("\n");
printf("Checking if the null space has a link with south, which was '2' \n");
if(space_contain_link(Nullspace,id)==TRUE){
    printf("OK, the space is linked with south\n");
}else{
    printf("ERROR, the space isnt linked with south\n");
}
/*-----------------------------------------------------------------------------------------------------*/
printf("Set the array of chars that later are going to be printed in the board (gdesc)\n");
if(space_set_gdesc(newspace, "-----", id++)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}printf("\n");
printf("Now in the null space\n");
if(space_set_gdesc(Nullspace, "-----", id)==OK){
    printf("OK\n");
}else{
    printf("ERROR\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Get the array of chars that later are going to be printed in the board (gdesc)\n");
testgdesc=space_get_gdesc(newspace,id);
if(testgdesc==NULL){
    printf("ERROR\n");
}else{
    printf("OK\n");
}
printf("\n");
printf("Now in the null space\n");
testgdesc=space_get_gdesc(Nullspace,id);
if(testgdesc==NULL){
    printf("ERROR\n");
}else{
    printf("OK\n");
}
printf("\n");
/*-----------------------------------------------------------------------------------------------------*/
printf("Destroying the space\n");
if(space_destroy(newspace)==ERROR){
    printf("ERROR: when destroying the space\n");
}
printf("Destroying the null space\n");
if(space_destroy(Nullspace)==ERROR){
    printf("ERROR: when destroying the space\n");
}
return (EXIT_SUCCESS);
}
/*-----------------------------------------------------------------------------------------------------*/