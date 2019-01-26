#ifndef GARDUINO_TASK_SHOW_MENU_H
#define GARDUINO_TASK_SHOW_MENU_H

/**
 * Handles control joystick movement and shows user menu.
 * 
 * Registers:
 *  - First     Contains last menu state to keep track of movement among
 *              different menus. When ShowMenu is run for the first time,
 *              just set it to MenuMain state.
*/
void TaskShowMenu();

#endif