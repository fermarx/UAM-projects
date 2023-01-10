/** 
 * @brief It defines a screen
 * 
 * @file screen.h
 * @author Profesores PPROG
 * @version 1.0 
 * @Date: 13-03-2019
 * @copyright GNU Public License
 */

#ifndef __SCREEN__
#define __SCREEN__

#define SCREEN_MAX_STR 80

/**
 * @brief It stores the spot where the different banners and information is placed
 * @author Teacher
 * @Date: 13-03-2019
 */
typedef struct _Area Area;

/**
 * @brief Creates the screen.
 * @author Teacher
 * @Date: 13-03-2019
 */
void  screen_init();

/**
 * @brief It deletes the screen by freeing the global variable __data defined in screen.c 
 * @author Teacher
 * @Date: 13-03-2019
 */
void  screen_destroy();

/**
 * @brief It clears the screen, dumps to the terminal the banners and the colors of the 
interface (blue)
 * @author Teacher
 * @Date: 13-03-2019
 */
void screen_paint();
/**
 * @brief Obtains the screen types for a space from a file
 * @author Teacher
 * @param str 
 * @Date: 13-03-2019
 */
void  screen_gets(char *str);

/**
 * @brief It creates a screen with the sizes specified to the function so that it can be later printed
 * @author Teacher
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @return Area* 
 * @Date: 13-03-2019
 */
Area* screen_area_init(int x, int y, int width, int height);

/**
 * @brief It deletes the screen by freeing the variable of the area
 * @author Teacher
 * @param *area 
 * @Date: 13-03-2019
 */
void  screen_area_destroy(Area* area);

/**
 * @brief It puts the whole area to 0
 * @author Teacher
 * @param *area 
 * @Date: 13-03-2019
 */
void  screen_area_clear(Area* area);
/**
 * @brief It resets the cursor
 * @author Teacher
 * @param *area
 * @Date: 13-03-2019 
 */
void  screen_area_reset_cursor(Area* area);

/**
 * @brief It sets the screen back if you the screen goes out of bounds
 * @author Teacher
 * @param area 
 * @param str 
 * @Date: 13-03-2019
 */
void  screen_area_puts(Area* area, char *str);

#endif
