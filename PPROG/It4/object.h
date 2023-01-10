/** 
 * @brief It defines an Object
 * 
 * @file Object.h
 * @author Enmanuel Abreu y Rodrigo Juez
 * @version 1.0 
 * @date 13-01-2015
 * @copyright GNU Public License
 */


#ifndef OBJECT_H
#define OBJECT_H
#include "types.h"
/**
 * @brief It stores the definitions of the ID of the object which is the place where the item is stored and its name
 * @author  Teacher
 * @Date: 13-03-2019
 */
typedef struct _Object Object;

/**
 * @brief Allocates memory for a new object structure => ID and name
 * @author Rodrigo Juez
 * @param id 
 * @return Object* 
 * @Date: 13-03-2019
 */
Object *object_create(Id id);

/**
 * @brief Frees the memory assigned to the object pointer previously created
 * @author Rodrigo Juez
 * @param *a 
 * @Date: 13-03-2019
 */
void object_destroy(Object *a);

/**
 * @brief Sets a new name for an object
 * @author Rodrigo Juez
 * @param *a *name
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS object_set_name(Object *a, char *name);

/**
 * @brief Getter for the object's name
 * @author Rodrigo Juez
 * @param *a 
 * @return char* 
 * @Date: 13-03-2019
 */
char *object_get_name(Object *a);

/**
 * @brief Getter for the object's id
 * @author Rodrigo Juez
 * @param *a 
 * @return Id 
 * @Date: 13-03-2019
 */
Id object_get_id(Object *a);

/**
 * @brief Prints the object's id and name in external screen 
 * @author Rodrigo Juez
 * @param a 
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS object_print(Object *a);

/**
 * @brief Sets the description of an object
 * @author S. Xiao Fernandez
 * @param *o *desc
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_description(Object *o, char *desc);

/**
 * @brief returns the description of an object 
 * @author Enmanuel
 * @param Object*, Id
 * @return ERROR or OK
 * @Date: 01-04-2019
 */

char *object_get_description(Object* o);

/**
 * @brief Sets the illumination field of the object to TRUE or FALSE
 * @author Enmanuel
 * @param Object*, Id
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_illuminate(Object *o, BOOL state);

/**
 * @brief returns the illumination(bool) of an object 
 * @author Carlos Garcia
 * @param Object
 * @return bool
 * @Date: 11-04-2019
 */
BOOL object_get_illuminate(Object *o);

/**
 * @brief returns the movable of an object 
 * @author Carlos Garcia
 * @param Object
 * @return bool
 * @Date: 11-04-2019
 */
BOOL object_get_movable(Object* o);

/**
 * @brief Sets the movable field of the object to TRUE or FALSE
 * @author Carlos Garcia
 * @param Object*, bool
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_movable(Object* o, BOOL state);

/**
 * @brief returns the moved of an object 
 * @author Carlos Garcia
 * @param Object
 * @return bool
 * @Date: 11-04-2019
 */
BOOL object_get_moved(Object* o);

/**
 * @brief Sets the moved field of the object to TRUE or FALSE
 * @author Carlos Garcia
 * @param Object*, bool
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_moved(Object* o, BOOL state);

/**
 * @brief returns the hidden of an object 
 * @author Carlos Garcia
 * @param Object
 * @return bool
 * @Date: 11-04-2019
 */
BOOL object_get_hidden(Object* o);

/**
 * @brief Sets the hidden field of the object to TRUE or FALSE
 * @author Carlos Garcia
 * @param Object*, bool
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_hidden(Object* o, BOOL status);

/**
 * @brief returns the open of an object 
 * @author Carlos Garcia
 * @param Object
 * @return bool
 * @Date: 11-04-2019
 */
Id object_get_open(Object* o);

/**
 * @brief Sets the open field of the object to the id
 * of the link it can open
 * @author Carlos Garcia
 * @param Object*, bool
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_open(Object* o, Id id);

/**
 * @brief Checks if the object can illuminate and then turns it on
 * @author Enmanuel Abreu
 * @param Object*, bool
 * @return ERROR or OK
 * @Date: 11-04-2019
 */
STATUS object_set_turnedOn(Object* o, BOOL state);

/**
 * @brief returns the turnedOn field
 * @author Carlos Garcia
 * @param Object*
 * @return BOOL
 * @Date: 13-04-2019
 */
BOOL object_get_turnedOn(Object* o);

/**
 * @brief sets the description for when the object has been moved
 * @author Carlos Garcia
 * @param Object*, char*
 * @return ERROR or OK
 * @Date: 13-04-2019
 */
STATUS object_set_moved_description(Object* o, char* description);

/**
 * @brief returns the (moved) description field
 * @author Carlos Garcia
 * @param Object*
 * @return char*
 * @Date: 13-04-2019
 */
char* object_get_moved_description(Object* o);

/**
 * @brief sets the location the objects starts in
 * @author Carlos Garcia
 * @param Object*, id
 * @return ERROR or OK
 * @Date: 15-04-2019
 */
STATUS object_set_starter_loc(Object* o, Id id);

/**
 * @brief returns the location the objects starts in
 * @author Carlos Garcia
 * @param Object*
 * @return Id
 * @Date: 15-04-2019
 */
Id object_get_starter_loc(Object* o);

#endif /* OBJECT_H */

