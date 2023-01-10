/** 
 * @brief It implements the command interpreter
 * 
 * @file command.h
 * @author Profesores PPROG
 * @version 1.0 
 * @date 19-12-2014 
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#define CMD_LENGHT 30
#define N_CMD 12
/**
 * @brief Enumeration of the different commmands available to the user, the order has to coincidence with the ones in command.c
 * @autor Modified Enmanuel Abreu
 * @Date: 13-03-2019
 */
typedef enum enum_Command {
  NO_CMD = -1, 
  UNKNOWN,
  EXIT, 
  PICK,
  DROP,
  MOVE,
  INSPECT,
  TURNON,
  TURNOFF,
  OPEN,
  SAVE,
  LOAD
  } T_Command;
  
/**
 * @brief It's basically a scanf for the commands in the promt where it compares the input to the
to the commands in cmd_to_str
 * @return T_Command 
 * @Date: 13-03-2019
 */
T_Command get_user_input();

#endif
