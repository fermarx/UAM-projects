/** 
 * @brief It defines a space
 * 
 * @file space.h
 * @author Profesores PPROG
 * @version 1.0 
 * @date 13-01-2015
 * @copyright GNU Public License
 */

#ifndef SPACE_H
#define SPACE_H

#include "types.h"
#include "set.h"
#include "link.h"
#define MAX_SPACES 100
#define FIRST_SPACE 1

/**
 * @brief Is one spot on the board and stores the connections it has with other spaces, the ID and the object on it 
 * @author  Teacher
 * @Date: 13-03-2019
 */
typedef struct _Space Space;



/**
 * @brief This function initializes a space to 0
 * @author modified by Rodrigo Juez
 * @param id 
 * @return Space* 
 * @Date: 13-03-2019
 */
Space* space_create(Id id);
/**
 * @brief Frees the allocated memory for space
 * @author modified by Rodrigo Juez
 * @param space 
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS space_destroy(Space* space);
/**
 * @brief Copies the name of the space block to another variable
 * @author modified by Rodrigo Juez
 * @param space 
 * @return Id 
 * @Date: 13-03-2019
 */
Id space_get_id(Space* space);
/**
 * @brief When moved to north it is set
 * @author modified by Rodrigo Juez
 * @param space 
 * @param name 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS space_set_name(Space* space, char* name);
/**
 * @brief Returns the space's name
 * @author modified by Rodrigo Juez
 * @param space 
 * @return const char* 
 * @Date: 13-03-2019
 */
const char* space_get_name(Space* space);
/**
 * @brief When moved to north it is set
 * @author modified by Rodrigo Juez
 * @param space 
 * @param id 
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS space_set_north(Space* space, Id id);
/**
 * @brief Checks if player can go north
 * @author modified by Rodrigo Juez
 * @param space 
 * @return Id 
 * @Date: 13-03-2019
 */
Id space_get_north(Space* space);
/**
 * @brief When moved to south it is set
 * @author modified by Rodrigo Juez
 * @param space 
 * @param id 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS space_set_south(Space* space, Id id);
/**
 * @brief Checks if player can go south
 * @author modified by Rodrigo Juez
 * @param space 
 * @return Id 
 * @Date: 13-03-2019
 */
Id space_get_south(Space* space);
/**
 * @brief When moved to east it is set
 * @author modified by Enmanuel Abreu
 * @param space 
 * @param id 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS space_set_east(Space* space, Id id);
/**
 * @brief Checks if player can go east
 * @author modified by Enmanuel Abreu
 * @param space 
 * @return Id 
 * @Date: 13-03-2019
 */
Id space_get_east(Space* space);
/**
 * @brief When moved to west it is set
 * @author modified by Enmanuel Abreu
 * @param space 
 * @param id 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS space_set_west(Space* space, Id id);
/**
 * @brief Checks if player can go west
 * @author modified by Enmanuel Abreu
 * @param space 
 * @return Id 
 * @Date: 13-03-2019
 */
Id space_get_west(Space* space);

/**
 * @brief sets the sets the value of the upwards link of a space
 * @author modified by Carlos Garcia
 * @param space, Id 
 * @return Status
 * @Date: 11-04-2019
 */
STATUS space_set_up(Space* space, Id id);
/**
 * @brief Checks if player can go up
 * @author modified by Carlos Garcia
 * @param space 
 * @return Id 
 * @Date: 11-04-2019
 */
Id space_get_up(Space* space);

/**
 * @brief sets the sets the value of the downwards link of a space
 * @author modified by Carlos Garcia
 * @param space, Id 
 * @return Status
 * @Date: 11-04-2019
 */
STATUS space_set_down(Space* space, Id id);
/**
 * @brief Checks if player can go down
 * @author modified by Carlos Garcia
 * @param space 
 * @return Id 
 * @Date: 11-04-2019
 */
Id space_get_down(Space* space);

/**
 * @brief sets the illumination of a space
 * @author modified by Carlos Garcia
 * @param space, bool 
 * @return Status
 * @Date: 11-04-2019
 */
STATUS space_set_illumination(Space* space, BOOL state);

/**
 * @brief returns if the space is illuminated
 * @author modified by Carlos Garcia
 * @param space 
 * @return Bool
 * @Date: 11-04-2019
 */
BOOL space_get_illumination(Space* space);

/**
 * @brief adds an object in to a space
 * @author modified by Enmanuel Abreu
 * @param space 
 * @param id_object 
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS space_set_object(Space* space, Id id_object);
/**
 * @brief Gets the id of the current space
 * @author modified by Rodrigo Juez
 * @param space 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
Set *space_get_objects(Space* space);
/**
 * @brief It prints the space indicated to the function
 * @author modified by Rodrigo Juez
 * return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS space_print(Space* space);

/**
 * @brief It gets the description of the space and returns it
 * @author Enmanuel Abreu
 * return char(the description)
 * @Date: 11-03-2019
 */
char* space_get_description(Space *space);
/**
 * @brief Setter for the description read from the gamereader
 * @author Enmanuel Abreu
 * return OK or ERROR
 * @Date: 11-04-2019
 */
STATUS space_set_description(Space* s, char* desc);

/**
 * @brief Checks if the object sent by id is in the space
 * @author Xiao?
 * return TRUE or FALSE
 * @Date: 11-04-2019
 */
BOOL space_contain_object(Space *space, Id id);

/**
 * @brief sets the short description of a space
 * @author Carlos Garcia
 * return TRUE or FALSE
 * @Date: 11-04-2019
 */
STATUS space_set_short_description(Space* space, char* description);

/**
 * @brief gets the short description of a space
 * @author Carlos Garcia
 * return char*
 * @Date: 11-04-2019
 */
char* space_get_short_description(Space* space);

/**
 * @brief checks if a space has a link (id) in one of his links
 * @author Carlos Garcia
 * return BOOL
 * @Date: 15-04-2019
 */
BOOL space_contain_link(Space* space, Id id);

/**
 * @brief returns the gdesc of given line
 * @author Carlos Garcia
 * return char*
 * @Date: 6-5-2019
 */
char *space_get_gdesc(Space *space,int line);

/**
 * @brief sets the gdesc variable of given line
 * @author Carlos Garcia
 * return STATUS
 * @Date: 6-5-2019
 */
STATUS space_set_gdesc(Space *space, char *gdesc, int line);
#endif