#ifndef SET_H
#define SET_H
#include <stdio.h>
#include "types.h"
/**
 * @brief Manages an array of Id's and an integer to know how much id's we got stored
 * @author Miguel Arnaiz 
 * @Date: 13-03-2019
 */
typedef struct _Set Set;

/**
 * @brief Set the ini object
 * @author Miguel Arnaiz 
 * @return Set* 
 * @Date: 13-03-2019
 */
Set *set_ini();

/**
 * @brief Set the obtain Id by position object
 * @author Miguel Arnaiz 
 * @param s and index 
 * @return Id 
 * @Date: 13-03-2019
 */
Id set_obtain_Id_by_position(Set *s, int index);

/**
 * @brief Set the destroy object
 * @author Miguel Arnaiz 
 * @param *s 
 * @Date: 13-03-2019
 */
void set_destroy(Set *s);

/**
 * @brief Set the add object
 * @author Miguel Arnaiz 
 * @param *s id  
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS set_add(Set *s, Id id);

/**
 * @brief checks if full
 * @author Miguel Arnaiz 
 * @param *set
 * @return FALSE or TRUE 
 * @Date: 13-03-2019
 */
BOOL set_is_full(Set *s);

/**
 * @brief Set the del object
 * @author Miguel Arnaiz 
 * @param *s id 
 * @return ERROR or OK 
 * @Date: 13-03-2019
 */
STATUS set_del(Set *s, Id id);

/**
 * @brief Set the get numIds object
 * @author Miguel Arnaiz 
 * @param *s 
 * @return int 
 * @Date: 13-03-2019
 */
int set_get_num_ids(Set *s);

/**
 * @brief Set the print object
 * @author Miguel Arnaiz 
 * @param pf and *s  
 * @return ERROR or OK
 * @Date: 13-03-2019
 */
STATUS set_print(FILE *pf,Set *s);


/**
 * @brief Set the find object
 * @author Miguel Arnaiz 
 * @param set & id 
 * @return TRUE or FALSE
 * @Date: 13-03-2019 
 */
BOOL set_find(Set *set, Id id);

/**
 * @brief returns the array of ids
 * @author  
 * @param set
 * @return Id*
 * @Date: 29-04-2019 
 */
Id* set_get_ids(Set* set);

#endif
