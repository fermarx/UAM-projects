/** 
 * @brief It defines which how many objects the player is going to have
 * @file inventory.h
 * @author S. Xiao Fernandez 
 * @version 1.0 
 * @date 20-03-2019
 * @copyright GNU Public License
 */

#ifndef INVENTORY_H
#define INVENTORY_H
#include "set.h"

typedef struct _Inventory Inventory;

/*@Brief allocates memory for the inventory and initializes the space
*@author S. Xiao Fernandez
*@param 
*@return inventory
*Date: 20-03-2019
*/
Inventory *inventory_create(int max_obj);

/*@Brief frees the memory of the parameter passed
*@author S. Xiao Fernandez
*@param inventory
*@return Ok
*Date: 20-03-2019
*/
STATUS inventory_destroy (Inventory *inventory);

/*@Brief sets how many objects the player can carry
*@author S. Xiao Fernandez
*@param inventory, maxObj
*@return Ok
*Date: 20-03-2019
*/
STATUS inventory_set_max_obj(Inventory *inventory, int maxObj);

/*@Brief sets the id of the object the player is carring
*@author S. Xiao Fernandez
*@param inventory, id_obj
*@return Ok
*Date: 20-03-2019
*/
STATUS inventory_set_object(Inventory* inventory, Id id_obj);

/*@Brief returns haw many objects the player can have
*@author S. Xiao Fernandez
*@param inventory
*@return inventory->max_obj
*Date: 20-03-2019
*/
int inventory_get_max_obj (Inventory * inventory);

/*@Brief returns the id of the object
*@author S. Xiao Fernandez
*@param inventory
*@return inventory->objects
*Date: 20-03-2019
*/
Set *inventory_get_objects(Inventory* inventory);

/*@Brief prints all the parameters of the inventory data structure
*@author S. Xiao Fernandez
*@param inventory
*@return Ok
*Date: 20-03-2019
*/
STATUS inventory_print(Inventory* inventory);


/*@Brief compares all the ids of the objects that the player has with the one that you send
*@author S. Xiao Fernandez
*@param inventory, id
*@return Ok
*Date: 20-03-2019
*/
BOOL inventory_contain_id(Inventory* inv, Id id);

/*@Brief checks if the inventory is full
*@author Carlos Garcia
*@param inventory
*@return BOOL
*Date: 6-5-2019
*/
BOOL inventory_is_full(Inventory* inv);

/*@Brief deletes an id from the array
*@author Carlos Garcia 
*@param inventory, id
*@return STATUS
*Date: 6-5-2019
*/
STATUS inventory_delete_object(Inventory* inv, Id id);

/*@Brief returns the number of objects
*@author Carlos Garcia 
*@param inventory
*@return int
*Date: 6-5-2019
*/
int inventory_get_number_of_objects(Inventory* inv);

#endif