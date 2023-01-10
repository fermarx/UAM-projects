/** 
 * @brief It defines a textual graphic engine
 * 
 * @file graphic_engine.h
 * @author Profesores PPROG
 * @version 1.0 
 * @date 18-01-2017
 * @copyright GNU Public License
 */

#ifndef __GRAPHIC_ENGINE__
#define __GRAPHIC_ENGINE__

#include "game.h"
#define MAX_TXT 8

typedef struct _Graphic_engine Graphic_engine;

/**
 * @brief Initializes the screen printing the map, description, banner, help and feedback
 to different coordinates in the screen
 * @author modified by Rodrigo Juez
 * @return Graphic_engine* 
 * Date: 13-03-2019
 */
Graphic_engine* graphic_engine_create();
/**
 * @brief This frees the struct of the graphic engine
 * @author modified by Rodrigo Juez
 * @param *ge 
 * Date: 13-03-2019
 */
void graphic_engine_destroy(Graphic_engine *ge);
/**
 * @brief This function paints the squares and the player draw if there is a player there
 * @author modified by Rodrigo Juez
 * @param *ge , *game
 * Date: 13-03-2019
 */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

/**
 * @brief Writes the commands specified to the user in the help banner
 * @param ge 
 * @param str 
 * Date: 13-03-2019
 */
void graphic_engine_write_command(Graphic_engine *ge, char *str);

#endif
