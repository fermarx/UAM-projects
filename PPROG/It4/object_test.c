#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

int main(){
    Object* obj=NULL;


    fprintf(stdout,"Initialising object...");
    obj=object_create(NO_ID);
    if(!obj){
        fprintf(stdout, "Error initialising object.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout,"Initialising object...");
    obj=object_create(111);
    if(!obj){
        fprintf(stdout, "Error initialising object.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object name...");
    if(object_set_name(obj, NULL)==ERROR){
        fprintf(stdout, "ERROR setting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object name...");
    if(object_set_name(obj, "object")==ERROR){
        fprintf(stdout, "ERROR setting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object description...");
    if(object_set_description(obj, NULL)==ERROR){
        fprintf(stdout, "ERROR setting object description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object description...");
    if(object_set_description(obj, "Test description")==ERROR){
        fprintf(stdout, "ERROR setting object description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object moved_description...");
    if(object_set_moved_description(obj, NULL)==ERROR){
        fprintf(stdout, "ERROR setting object moved_description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object moved_description...");
    if(object_set_moved_description(obj, "Test moved_description")==ERROR){
        fprintf(stdout, "ERROR setting object moved_description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }

    
    fprintf(stdout, "Setting object hidden...");
    if(object_set_hidden(NULL, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object hidden.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object hidden...");
    if(object_set_hidden(obj, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object hidden.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object moved...");
    if(object_set_moved(NULL, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object moved.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object moved...");
    if(object_set_moved(obj, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object moved.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object movable...");
    if(object_set_movable(NULL, FALSE)==ERROR){
        fprintf(stdout, "ERROR setting object movable.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object movable...");
    if(object_set_movable(obj, FALSE)==ERROR){
        fprintf(stdout, "ERROR setting object movable.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object open...");
    if(object_set_open(NULL, 222)==ERROR){
        fprintf(stdout, "ERROR setting object open.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object open...");
    if(object_set_open(obj, 222)==ERROR){
        fprintf(stdout, "ERROR setting object open.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object illuminate...");
    if(object_set_illuminate(NULL, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object illuminate.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object illuminate...");
    if(object_set_illuminate(obj, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object illuminate.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object turnedOn...");
    if(object_set_turnedOn(NULL, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object turnedOn.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object turnedOn...");
    if(object_set_turnedOn(obj, TRUE)==ERROR){
        fprintf(stdout, "ERROR setting object turnedOn.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Setting object starter_loc...");
    if(object_set_starter_loc(NULL, 1)==ERROR){
        fprintf(stdout, "ERROR setting object starter_loc.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Setting object starter_loc...");
    if(object_set_starter_loc(obj, 1)==ERROR){
        fprintf(stdout, "ERROR setting object starter_loc.\n");
    }else{
        fprintf(stdout, "OK\n");
    }



    fprintf(stdout, "Getting object id...");
    if(object_get_id(NULL)!=111){
        fprintf(stdout, "ERROR getting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object id...");
    if(object_get_id(obj)!=111){
        fprintf(stdout, "ERROR getting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object name...");
    if(!object_get_name(NULL)){
        fprintf(stdout, "ERROR getting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object name...");
    if(!object_get_name(obj)){
        fprintf(stdout, "ERROR getting object name.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object description...");
    if(!object_get_description(NULL)){
        fprintf(stdout, "ERROR getting object description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object description...");
    if(!object_get_description(obj)){
        fprintf(stdout, "ERROR getting object description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object moved_description...");
    if(!object_get_moved_description(NULL)){
        fprintf(stdout, "ERROR getting object moved_description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object moved_description...");
    if(!object_get_moved_description(obj)){
        fprintf(stdout, "ERROR getting object moved_description.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object hidden...");
    if(object_get_hidden(obj)!=FALSE){
        fprintf(stdout, "ERROR getting object hidden.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object hidden...");
    if(object_get_hidden(obj)!=TRUE){
        fprintf(stdout, "ERROR getting object hidden.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object moved...");
    if(object_get_moved(obj)!=FALSE){
        fprintf(stdout, "ERROR getting object moved.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object moved...");
    if(object_get_moved(obj)!=TRUE){
        fprintf(stdout, "ERROR getting object moved.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object movable...");
    if(object_get_movable(obj)!=TRUE){
        fprintf(stdout, "ERROR getting object movable.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object movable...");
    if(object_get_movable(obj)!=FALSE){
        fprintf(stdout, "ERROR getting object movable.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object open...");
    if(object_get_open(NULL)!=222){
        fprintf(stdout, "ERROR getting object open.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object open...");
    if(object_get_open(obj)!=222){
        fprintf(stdout, "ERROR getting object open.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object illuminate...");
    if(object_get_illuminate(NULL)!=TRUE){
        fprintf(stdout, "ERROR getting object illuminate.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object illuminate...");
    if(object_get_illuminate(obj)!=TRUE){
        fprintf(stdout, "ERROR getting object illuminate.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object turnedOn...");
    if(object_get_turnedOn(NULL)!=TRUE){
        fprintf(stdout, "ERROR getting object turnedOn.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object turnedOn...");
    if(object_get_turnedOn(obj)!=TRUE){
        fprintf(stdout, "ERROR getting object turnedOn.\n");
    }else{
        fprintf(stdout, "OK\n");
    }


    fprintf(stdout, "Getting object starter_loc...");
    if(object_get_starter_loc(NULL)!=1){
        fprintf(stdout, "ERROR getting object starter_loc.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    fprintf(stdout, "Getting object starter_loc...");
    if(object_get_starter_loc(obj)!=1){
        fprintf(stdout, "ERROR getting object starter_loc.\n");
    }else{
        fprintf(stdout, "OK\n");
    }
    object_destroy(obj);
    return (EXIT_SUCCESS);
}