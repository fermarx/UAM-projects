/** 
 * @brief It defines the game loop 
 * 
 * @file game_loop.c
 * @author Profesores PPROG
 * @version 1.0 
 * @date 13-01-2015 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_engine.h"
#include "command.h"
#include "game.h"

int main(int argc, char *argv[]){
  	FILE *pf;
  	char str[MAX_TXT]="", command_status[MAX_TXT]="";
  	extern char *cmd_to_str[];
	Game *game=NULL;
	T_Command command = NO_CMD; 
	Graphic_engine *gengine=NULL;
if (argc < 2){fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]); return 1;}

if(argv[2] && strcmp(argv[2], "-l") == 0){
	pf = fopen("log.txt", "w");
	if(!pf){return 1;}
}
	game=game_create_from_file( argv[1]);
	if (game == NULL){
		fprintf(stderr, "Error while initializing game.\n"); 
		return 1;
	}
	if ((gengine = graphic_engine_create()) == NULL){
		fprintf(stderr, "Error while initializing graphic engine.\n"); 
		game_destroy(game); 
		return 1;
	}
	while ( (command != EXIT) && !game_is_over(game) ){
	  graphic_engine_paint_game(gengine, game);
		command = get_user_input();
		game_update(game, command);
		if(argv[2] && strcmp(argv[2], "-l") == 0){
		if(game_get_last_command(game)==UNKNOWN){
  					sprintf(command_status, " ");
		}else{ 
			if(game_get_status(game)){
		 		sprintf(command_status, ": OK");
 				}else {
    				sprintf(command_status, ": ERROR");
  				}}
			  	sprintf(str, "%s%s\n",cmd_to_str[game_get_last_command(game)-NO_CMD], command_status);
				fprintf(pf,"%s",str);
		}
		} game_destroy(game);
		graphic_engine_destroy(gengine);

if(argv[2] && strcmp(argv[2], "-l") == 0){
	fclose(pf);
}

return 0;
}
