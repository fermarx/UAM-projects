#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H
#include "types.h"
#include "game.h"

/**
 * @brief Reads a file and starts to add spaces until all the data file is loaded 
and the game is created
 * @author modified by Rodrigo Juez
 * @param game 
 * @param filename 
 * @return ERROR or OK
 * Date: 13-03-2019
 */
STATUS game_load_spaces(Game* game, char* filename);
/**
 * @brief Reads a file and starts to add the objects to their spaces
 * @author Miguel Arnaiz
 * @param game 
 * @param filename 
 * @return ERROR or OK 
 * Date: 13-03-2019
 */
STATUS game_load_objects(Game* game, char* filename);
/**
 * @brief reads a file and loads al the links in the file to the game
 * @author Carlos Garcia
 * @param game 
 * @param filename 
 * @return ERROR or OK 
 * Date: 25-03-2019
 */
STATUS game_load_links(Game* game, char* filename);

/**
 * @brief reads a file and saves in it the state of a game
 * @author Carlos Garcia
 * @param game 
 * @param filename 
 * @return ERROR or OK 
 * Date: 29-04-2019
 */
STATUS gamemanagement_save(Game* game, char* filename);

/*
 * @brief reads a file and loads the player of a saved game
 * @author Carlos Garcia 
 * @param game* and char*
 * @return ERROR or OK 
 * Date: 30-04-2019
*/
STATUS game_load_player(Game* game, char* filename);
/**
 * @brief reads a file and loads the state of a saved game
 * @author Carlos Garcia
 * @param game 
 * @param filename 
 * @return ERROR or OK 
 * Date: 29-04-2019
 */
Game* gamemanagement_load(Game *game, char* filename);

/*
 * @brief reads a file and loads the player objects of a saved game
 * @author S. Xiao Fernández 
 * @param game* and char*
 * @return ERROR or OK 
 * Date: 30-04-2019
*/
STATUS game_load_player_objects(Game *game, char *filename);

#endif /* GAMEMANAGEMENT_H */