#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "graphic_engine.h"
#include "game.h"
#include "set.h"
#include "link.h"
#include "space.h"
#define MAX_OBJ 9
#define MAX_NAME_OBJ 5
#define MAX_ADD 7

struct _Graphic_engine{
  Area *map, *descript, *banner, *help, *feedback;
};

Graphic_engine *graphic_engine_create(){
  static Graphic_engine *ge = NULL;

  if (ge)
    return ge;
  
  screen_init();
  ge = (Graphic_engine *) malloc(sizeof(Graphic_engine));
  
  /*(right, down, width, length)*/
  ge->map      = screen_area_init( 2, 1, 70, 16+3);
  ge->descript = screen_area_init(75, 1, 52, 16+3);
  ge->banner   = screen_area_init(40,18+3, 46, 1);
  ge->help     = screen_area_init(2,19+3, 127, 2);
  ge->feedback = screen_area_init(2,22+4, 110, 3);

  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge){
  if (!ge)
    return;
  
  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);
  
  screen_destroy();
  free(ge);
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  STATUS ok; /*used to store STATUS error ok of the last command*/
  int j=0;
  char STATUS[MAX_TXT];
  char *cmd;
  char *top;
  char *middle;
  char *bottom;
  char *a;
  char add[MAX_ADD];
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, obj_loc1=NO_ID;
  Space* space_act = NULL, *space_next=NULL;
  char obj1[MAX_NAME_OBJ], obj2[MAX_NAME_OBJ], obj3[MAX_NAME_OBJ], obj4[MAX_NAME_OBJ];
  char str[255];
  T_Command last_cmd = UNKNOWN;
  extern char *cmd_to_str[];
  int nids=0; 
  Set *setback=NULL;
  ok=game_get_status(game);
  /* Paint the in the map area */
  screen_area_clear(ge->map);
  if ((id_act = game_get_player_location(game)) != NO_ID){
    
    space_act = game_get_space(game, id_act);
    id_back = link_get_connection(space_get_id(space_act),game_get_link_from_id(game, space_get_north(space_act)));
    id_next = link_get_connection(space_get_id(space_act), game_get_link_from_id(game, space_get_south(space_act)));
    space_next = game_get_space(game, id_next);

/*prints the space if ther is not an object in the sapce*/
    strcpy(obj1, "    ");
    strcpy(obj2, "    ");
    strcpy(obj3, "    ");
    strcpy(obj4, "    ");

    nids = set_get_num_ids(space_get_objects(game_get_space(game, id_back)));
    setback= space_get_objects(game_get_space(game,id_back));

    
    if(nids == 0){
    } else if(nids == 1){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      /*prints the space if ther is just an object in the sapce*/
      strcpy(obj2, "    ");
      strcpy(obj3, "    ");
      strcpy(obj4, "    ");
    } else if(nids == 2){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      /*prints the space if there aretwo objects in the sapce*/
      strcpy(obj3, "   ");
      strcpy(obj4, "  ");
    } else if(nids == 3){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      /*prints the space if there are 3 objects in the sapce*/
      strcpy(obj4, "    ");
    } else if(nids == 4){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 3)));
      strcpy(obj4, a);
      
    } 


                    
    if (id_back != NO_ID) {
      sprintf(str, "                      |                  %2d|",(int) id_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      |%s %s %s %s |",obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      +--------------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "                                ^ %s", link_get_name(game_get_link_from_id(game,space_get_north(space_act))));
      screen_area_puts(ge->map, str);
    }
    
    setback=NULL;
    /*prints the space if ther is not an object in the sapce*/
    strcpy(obj1, "    ");
    strcpy(obj2, "    ");
    strcpy(obj3, "    ");
    strcpy(obj4, "    ");

    nids = set_get_num_ids(space_get_objects(game_get_space(game, id_act)));
    setback= space_get_objects(game_get_space(game,id_act));

    
    if(nids == 0){
    } else if(nids == 1){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      /*prints the space if ther is just an object in the sapce*/
      strcpy(obj2, "    ");
      strcpy(obj3, "    ");
      strcpy(obj4, "    ");
    } else if(nids == 2){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      /*prints the space if there are two objects in the sapce*/
      strcpy(obj3, "    ");
      strcpy(obj4, "    ");
    } else if(nids == 3){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      /*prints the space if there are 3 objects in the sapce*/
      strcpy(obj4, "    ");
    } else if(nids == 4){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 3)));
      strcpy(obj4, a);
      
    } 

    top=space_get_gdesc(game_get_space(game, id_act), 0);
    middle=space_get_gdesc(game_get_space(game,id_act), 1);
    bottom=space_get_gdesc(game_get_space(game,id_act), 2);

    
    if (id_act != NO_ID) {

    if (space_get_east(space_act)!=NO_ID && space_get_west(space_act)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_west(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d| <--  %ld",(int) id_act, link_get_connection(id_act, game_get_link_from_id(game, space_get_west(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |%s",top, link_get_name(game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        | --> %ld",middle,link_get_connection(id_act, game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
      }else if (space_get_east(space_act)!=NO_ID && space_get_up(space_act)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_up(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d| (up)<--  %ld",(int) id_act, link_get_connection(id_act, game_get_link_from_id(game, space_get_up(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |%s",top, link_get_name(game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        | --> %ld",middle,link_get_connection(id_act, game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
      }else if(space_get_west(space_act)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_west(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d| <--  %ld",(int) id_act, link_get_connection(id_act, game_get_link_from_id(game, space_get_west(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);

      }else if (space_get_east(space_act)!=NO_ID){
      
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d| -->  %ld",(int) id_act, link_get_connection(id_act, game_get_link_from_id(game, space_get_east(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);

      }else if(space_get_down(space_act)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_down(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d| (down)-->  %ld",(int) id_act, link_get_connection(id_act, game_get_link_from_id(game, space_get_down(space_act))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);

      }
      else{

        sprintf(str, "                      +--------------------+");
        screen_area_puts(ge->map, str);
        sprintf(str, "                      | (*O*)            %2d|", (int) id_act );
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);
      
      }

      sprintf(str, "                      |     %s        |",bottom);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      |%s %s %s %s |",obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      +--------------------+");
      screen_area_puts(ge->map, str);
      
    }

    
    setback=NULL;
    /*prints the space if ther is not an object in the sapce*/
    strcpy(obj1, "    ");
    strcpy(obj2, "    ");
    strcpy(obj3, "    ");
    strcpy(obj4, "    ");

    nids = set_get_num_ids(space_get_objects(game_get_space(game, id_next)));
    setback= space_get_objects(game_get_space(game,id_next));

    
    if(nids == 0){
    } else if(nids == 1){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      /*prints the space if ther is just an object in the sapce*/
      strcpy(obj2, "    ");
      strcpy(obj3, "    ");
      strcpy(obj4, "    ");
    } else if(nids == 2){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      /*prints the space if there are two objects in the sapce*/
      strcpy(obj3, "    ");
      strcpy(obj4, "    ");
    } else if(nids == 3){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      
      strcpy(obj4, "    ");
    } else if(nids == 4){
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 0)));
      strcpy(obj1, a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 1)));
      strcpy(obj2, a);
      
      a= object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 2)));
      strcpy(obj3,a);
      
      a=object_get_name(game_get_object_from_id(game, set_obtain_Id_by_position(setback, 3)));
      strcpy(obj4, a);
      
    }

    top=space_get_gdesc(game_get_space(game, id_next), 0);
    middle=space_get_gdesc(game_get_space(game,id_next), 1);   
    bottom=space_get_gdesc(game_get_space(game,id_next), 2);
    if (id_next != NO_ID) {

      sprintf(str, "                                 v %s", link_get_name(game_get_link_from_id(game,space_get_south(space_act))));
      screen_area_puts(ge->map, str);

      
      if (space_get_east(space_next)!=NO_ID){
      
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_east(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |                  %2d| -->  %ld",(int)id_next, link_get_connection(id_next, game_get_link_from_id(game, space_get_east(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);
      
      }
      else if(space_get_west(space_next)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_west(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |                  %2d| <--  %ld",(int)id_next,link_get_connection(id_next, game_get_link_from_id(game, space_get_west(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);

      }else if (space_get_east(space_next)!=NO_ID && space_get_west(space_next)!=NO_ID){
        
        sprintf(str, "                      +--------------------+ %s", link_get_name(game_get_link_from_id(game, space_get_west(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |                  %2d| <--  %ld",(int)id_next, link_get_connection(id_next, game_get_link_from_id(game, space_get_west(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |%s",top, link_get_name(game_get_link_from_id(game, space_get_east(space_next))));
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        | --> %ld",middle,link_get_connection(id_next, game_get_link_from_id(game, space_get_east(space_next))));
        screen_area_puts(ge->map, str);

      }
      else{

        sprintf(str, "                      +--------------------+");
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |                  %2d|",(int)id_next);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",top);
        screen_area_puts(ge->map, str);
        sprintf(str, "                      |     %s        |",middle);
        screen_area_puts(ge->map, str);
      
      }
      sprintf(str, "                      |     %s        |",bottom);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      |%s %s %s %s |",obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
      sprintf(str, "                      +--------------------+");
      screen_area_puts(ge->map, str);

    }
  }


  /* Paint the in the description area */
  screen_area_clear(ge->descript);
  sprintf(str, "Objects Location: ");
  screen_area_puts(ge->descript, str);

  strcpy(str, "\0");

  for(j=0; game_get_object(game, j)!=NULL; j++){
    obj_loc1=game_get_object_location(game, object_get_id(game_get_object(game, j)));
    a=object_get_name(game_get_object(game, j));
    if(obj_loc1!=NO_ID){
      sprintf(add, "%s: %ld ",a, obj_loc1);
      strcat(str,add);
    }
    if(strlen(str)>=20){  
      screen_area_puts(ge->descript, str);
      strcpy(str, "\0");
    }
  }
  screen_area_puts(ge->descript, str);
  sprintf(str, "Player Object(s): ");
  screen_area_puts(ge->descript, str);

  strcpy(str, "\0");

  for (j=0;game_get_object(game, j)!=NULL;j++){
    a=object_get_name(game_get_object(game, j));
    if(game_get_object_location(game, object_get_id(game_get_object(game, j)))==NO_ID){
      sprintf(str, "%s ",a);
      screen_area_puts(ge->descript, str);
    }
  }

  last_cmd = game_get_last_command(game);
  if(last_cmd==INSPECT){
    sprintf(str, "Description: %s", game_get_description(game));
    screen_area_puts(ge->descript, str);
  }
  else{
    sprintf(str, "You can move in this direction: %s", space_get_short_description(game_get_space(game, game_get_player_location(game))));
    screen_area_puts(ge->descript, str);
  }
  if(space_get_illumination(space_act)==TRUE){
    sprintf(str, "Space illuminated.");
    screen_area_puts(ge->descript, str);
  }
  else{
    sprintf(str, "Space not illuminated.");
    screen_area_puts(ge->descript, str);
  }
  

  /* Paint the in the banner area */
  screen_area_puts(ge->banner, "          ---THE GREATEST MAZE--- ");

  /* Paint the in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, " next(n), back(b), exit(e), pick(p), drop(d), move (m), inspect (i), turnOn(ton), turnOff(toff), open(o), save(s), load(ld)");
  screen_area_puts(ge->help, str);
  
  /* Paint in the feedback area */
  if(ok){
    strcpy(STATUS, ": OK\0");
  }
  else {
    strcpy(STATUS, ": ERROR\0");
  }

  cmd = cmd_to_str[last_cmd-NO_CMD];
  if(!strcmp(cmd, "Unknown")||!strcmp(cmd, "No command")){strcpy(STATUS,"\0");}
  sprintf(str, " %s%s", cmd, STATUS);
  screen_area_puts(ge->feedback, str);
  

  /* Dump to the terminal */
  screen_paint();
  printf("prompt:> ");
  screen_area_clear(ge->map);

  
}
