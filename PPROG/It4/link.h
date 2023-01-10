/** 
 * @brief It defines the link, which spaces it links, and whether it open or closed
 * @file link.h
 * @author S. Carlos Garcia
 * @version 1.0 
 * @date 20-03-2019
 * @copyright GNU Public License
 */

#ifndef LINK_H
#define LINK_H
#include "types.h"
#define MAX_LINK 100
/**
 * @brief allocates Link struct with the fields: (Id, name, first connection id and second)
 * and the state (able to connect with other or not)
 * @author Carlos Garcia
 * @param id, first, second
 * @return Link* 
 * @Date: 20-03-2019
 */
typedef struct _Link Link;

/**
 * @brief allocates memory for the new link, 
 *  sets the 2 id of the spaces linked and 
 *  sets the state of the link to closed
 * @author Carlos Garcia
 * @param id, first, second
 * @return Link* 
 * @Date: 20-03-2019
 */
Link* link_create(Id id);

/**
 * @brief frees the memory of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  
 * @Date: 20-03-2019
 */
void link_destroy(Link* lk);


/**
 * @brief changes the name of a link
 * @author Carlos Garcia
 * @param Link*, name
 * @return  status
 * @Date: 20-03-2019
 */
STATUS link_set_name(Link* lk, char* name);

/**
 * @brief changes the first space of a link
 * @author Carlos Garcia
 * @param Link*, id
 * @return  status
 * @Date: 20-03-2019
 */
STATUS link_set_first(Link* lk, Id first);

/**
 * @brief changes the second space of a link
 * @author Carlos Garcia
 * @param Link*, id
 * @return  status
 * @Date: 20-03-2019
 */
STATUS link_set_second(Link* lk, Id second);

/**
 * @brief changes the both spaces ids of a link
 * @author Carlos Garcia
 * @param Link*, id, id
 * @return  status
 * @Date: 20-03-2019
 */
STATUS link_set_connect(Link* lk, Id first, Id second);

/**
 * @brief changes the state of a link
 * @author Carlos Garcia
 * @param Link*, bool
 * @return  status
 * @Date: 20-03-2019
 */
STATUS link_set_state(Link* lk, BOOL state);

/**
 * @brief returns the id of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  id
 * @Date: 20-03-2019
 */
Id link_get_id(Link* lk);

/**
 * @brief returns the name of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  char*
 * @Date: 20-03-2019
 */
char* link_get_name(Link* lk);

/**
 * @brief returns the first space id of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  id
 * @Date: 20-03-2019
 */
Id link_get_first(Link* lk);

/**
 * @brief returns the second space id of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  id
 * @Date: 20-03-2019
 */
Id link_get_second(Link* lk);

/**
 * @brief returns the state of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  bool
 * @Date: 20-03-2019
 */
BOOL link_get_state(Link* lk);

/**
 * @brief prints the contents of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  int
 * @Date: 20-03-2019
 */
STATUS link_print(FILE* pf, Link* lk);

/**
 * @brief returns a copy of a link
 * @author Carlos Garcia
 * @param Link*
 * @return  status
 * @Date: 28-03-2019
 */
Link* link_copy(Link* lk);

/**
 * @brief returns the connectionof the link in a space(given its id)
 * @author Carlos Garcia
 * @param Id, Link*
 * @return  Id
 * @Date: 30-03-2019
 */
Id link_get_connection(Id s, Link* l);

#endif