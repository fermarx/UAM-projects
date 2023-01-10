/** 
 * @brief It defines a Player
 * 
 * @file player.h
 * @author Enmanuel Abreu
 * @version 1.0 
 * @date 13-01-2015
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H


#include "object.h"
#include "inventory.h"
#include "link.h"
#include "set.h"
/**
 * @brief It can store the place is located in, name, object and Id of the player
 * @author Enmanuel Abreu
 * @Date: 13-03-2019
 */
typedef struct _Player Player;
/**
 * @brief Initializes a new player 
 * @author modified by S. Xiao Fernandez
 * @param id 
 * @return Player* 
 * @Date: 13-03-2019
 */
Player *player_create(Id id);
/**
 * @brief Frees the dinamic memory allocated for the player
 * @author modified by S. Xiao Fernandez
 * @param player 
 * @return OK or ERROR 
 * @Date: 13-03-2019
 */
STATUS player_destroy(Player *player);
/**
 * @brief Checks if player and the space exist, if so, assign the space to the player
 * @author Enmanuel Abreu
 * @param player 
 * @param id 
 * @return OK or ERROR 
 * @Date: 13-03-2019
 */
STATUS player_set_space(Player *player, Id id);

/**
 * @brief Assign the name to the player which the user introduced
 * @author Enmanuel Abreu
 * @param player 
 * @param name 
 * @return OK or ERROR 
 * @Date: 13-03-2019
 */
STATUS player_set_name(Player *player, char *name);
/**
 * @brief Assign the object to the player
 * @author modified by S. Xiao Fernandez
 * @param player 
 * @param id 
 * @return OK or ERROR
 * @Date: 13-03-2019
 */
STATUS player_set_object(Player *player, Id id);
/**
 * @brief Obtains the name to the player introduced by the user
 * @author Enmanuel Abreu
 * @param player 
 * @return const char* 
 * @Date: 13-03-2019
 */
const char *player_get_name(Player *player);

/**
 * @brief returns the inventory of a player
 * @author Carlos Garcia
 * @param player 
 * @return const char* 
 * @Date: 6-5-2019
 */
Inventory *player_get_inventory (Player* player);

/**
 * @brief Obtains the ID of the player 
 * @author Enmanuel Abreu
 * @param player 
 * @return Id 
 * @Date: 13-03-2019
 */
Id player_get_id(Player *player);
/**
 * @brief Returns how many objects the player has
 * @author modified by S. Xiao Fernandez
 * @param player 
 * @return integer (num of objects)
 * @Date: 13-03-2019
 */
int player_get_number_of_objects(Player *player);

/*@Brief returns the maximum number of objects the player can have
*@author S. Xiao Fernandez
*@param 
*@return inventory
*Date: 20-03-2019
*/
int player_get_max_number_of_objects(Player*player);
/**
 * @brief Obtains the ADT space from the player aka the position)
 * @author Enmanuel Abreu
 * @param player 
 * @return Id 
 * @Date: 13-03-2019
 */
Id player_get_space(Player *player);
/**
 * @brief Prints the player's ID, name, the object assigned and the space (location) in an external screen
 * @author Enmanuel Abreu
 * @param *player
 * @return OK or ERROR
 * @Date: 13-03-2019
 */
STATUS player_print(Player *player);

/**
 * @brief If the Id that you send is in the inventory the function returns OK
 * @author S. Xiao Fernández
 * @param *player, Id
 * @return OK or ERROR
 * @Date: 27-03-2019
 */
BOOL player_contain_object_id(Player* p, Id id);


Id* player_get_objects_array(Player* p);


#endif /* PLAYER_H */

