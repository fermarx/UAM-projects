/** 
 * @brief It defines the game's functions, commands implementation and game's structure
 * @file game.h
 * @author Profesores PPROG
 * @version 1.0 
 * @date 18-01-2019
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "player.h"
#include "space.h"
#include "link.h"

#define MAX_OBJECTS 40
#define MAX_DESCRIPTION 100
#define MAX_ILLUMINATION 10


/*@brief Is the basic game's struct, where the object and player(s) are stored and where the board
*with the information is also stored, it also stores the command input
*@author modified by Rodrigo Juez
*Date: 13-03-2019
*/
typedef struct _Game Game;

/*@Brief It initializes the memory for the data structure of game
*@author S. Xiao Fernandez
*@param 
*@return game
*Date: 20-03-2019
*/
struct Game *game_ini();

/*@Brief It adds an space useful for creating 
*@author Teacher
*@param *game *space
*@return OK or ERROR
*Date: 13-03-2019
*/
STATUS game_add_space(Game* game, Space* space);

/*@Brief It adds an link useful for creating 
*@author Carlos Garcia
*@param *game *link
*@return OK or ERROR
*Date: 2-4-2019
*/
STATUS game_add_link(Game* game, Link* link);

/*@Brief It adds a player to the game 
*@author S. Xiao Fernández
*@param Game, Player
*@return OK or ERROR
*Date: 4-5-2019
*/
STATUS game_add_player(Game* game, Player* player);

/*@Brief returns a link which has said id
*@author Carlos Garcia
*@param *game id
*@return link
*Date: 2-4-2019
*/
Link* game_get_link_from_id(Game* game, Id id);

/*@Brief Initializes to empty a game struct which is the info with last commands object and player location
 and the spaces it has
*@author modified by Rodrigo Juez
*@param *game
*@return game->player=player_create(1)
* game->last_cmd = NO_CMD
* game->ok=OK
* Date: 13-03-2019
*/
Game* game_create();

/*@Brief Reads de file data.dat and loads the game. Loads the spaces in the board from a file
 and then sets the player and the object to the position
 assigned to id 0
*@author modified by Miguel Arnaiz
*@param *game and filename
*@return OK or ERROR
*Date: 13-03-2019
*/
Game* game_create_from_file( char* filename);


/*@Brief Updates the information of the game to the command inserted by the user
*@author modified by Rodrigo Juez
*@param *game and T_command cmd
*@return OK or ERROR
*Date: 13-03-2019
*/
STATUS game_update(Game* game, T_Command cmd);

/*@Brief The function frees the memory allocated for the struct of the game
*@author modified by Rodrigo Juez
*@param game
*@return Ok or ERROR
*Date: 13-03-2019
*/
STATUS game_destroy(Game* game);

/*@Brief It is useless at the moment because it always returns false.
*It is meant to be implemented in future iterations.
*@author Teacher
*@param *game
*@return False
*Date: 13-03-2019
*/
BOOL game_is_over(Game* game);
/*
*@Brief Obtains info about the object and player location and displays,
*it also puts the text prompt
*@author modified by Rodrigo Juez
*@param
*Date: 13-03-2019
*/
void game_print_data(Game* game);

/*@Brief Getter of the space's struct for the game implementation
*@author modified by Rodrigo Juez
*@param *game and Id id
*Date: 13-03-2019
*/
Space* game_get_space(Game* game, Id id);

/*@Brief Returns the cuantity of objects
*@author Rodrigo Juez
*@param *game
*Date: 13-03-2019
*/
int game_get_max_objects(Game *game);

/**
 * @brief Getter of the player location
 * @author Rodrigo Juez
 * @param game 
 * @return Id
 * Date: 13-03-2019
 */
Id game_get_player_location(Game* game);

/*@Brief Getter of the object location and saved in the game's struct
*@author modified by Rodrigo Juez
*@param *game
*@return player_get_space(game->player)
*Date: 13-03-2019
*/
Id game_get_object_location(Game* game, Id id_object);

/*@Brief Gets the status of the function that is in the private file
*@author S. Xiao Fernandez
*@param *game
*@return game->status
*Date: 20-03-2019
*/
STATUS game_get_status(Game *game);

/*@Brief Gets the object in the position var of the function that is in the private file
*@author S. Xiao Fernandez
*@param *game, var
*@return game->object[var]
*Date: 20-03-2019
*/
Object *game_get_object(Game *game, int var);

/*@Brief Gets the player of the function that is in the private file
*@author S. Xiao Fernandez
*@param *game
*@return game->player
*Date: 20-03-2019
*/
Player *game_get_player(Game *game);

/*@Brief It is used in the graphic engine to display the command log
*@author Rodrigo Juez
*@param *game and id_object
*@return space_get_id(game->spaces[i]); i= place in the array that matches
*Date: 13-03-2019
*/
T_Command game_get_last_command(Game* game);

/**
 * @brief Get the object's id and stores it in the game's struct
 * @param game, object_id 
 * @author modified by Rodrigo Juez
 * @return Object*
 * Date: 13-03-2019 
 */
Object *game_get_object_from_id(Game *game, Id object_id);
/*@Brief It sets the Id of the object to the space selected by the ID in the arguments
* @author modified by Rodrigo Juez
* @param *game, id_space, id_object
* @return
* Date: 13-03-2019
*/
STATUS game_set_object_location(Game* game, Id id_space, Id id_object);

/*@Brief Adds an object to the game struct checking before if theres space for it 
*@author Miguel Arnaiz
*@param *game and *object
*@return OK or ERROR
*Date: 13-03-2019
*/
STATUS game_add_object(Game *game, Object *object);

/*
*@Brief Gets the description in game
*@author Carlos Garcia
*@param *game
*@return char*
*Date: 13-04-2019
*/
char* game_get_description(Game* game);

/*
*@Brief returns the space of a given position of the array
*@author Carlos Garcia
*@param *game, int
*@return Space*
*Date: 30-04-2019
*/
Space* game_get_space_by_position(Game* game, int position);

/*
*@Brief returns the link of a given position of the array
*@author Carlos Garcia
*@param *game, int
*@return Link*
*Date: 30-04-2019
*/
Link* game_get_link_by_position(Game* game, int position);

/*
*@Brief sets the player to a location
*@author Carlos Garcia
*@param *game, id
*@return STATUS
*Date: 26-04-2019
*/
STATUS game_set_player_location(Game* game, Id id);

STATUS game_set_player_object(Game*game, Id id);
#endif
