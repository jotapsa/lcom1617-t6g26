#ifndef __EVENTS_H
#define __EVENTS_H


/**
 * @brief handles timer interrupts for each mode
 * - MAIN_MENU: Draws the menu and the mouse
 * - MULTIPLAYER_LOCAL: Draws the level, updates positions checks to see if anyone has crashed
 * - SINGLEPLAYER: Draws the level, updates positions checks to see if anyone has crashed
 * - INSTRUCTIONS: Draws the instructions
 */

void tick ();


/**
 * @brief handles keyboard interrupts for each mode
 * - MAIN_MENU: Changes selection or makes selection
 * - MULTIPLAYER_LOCAL: Changes player 1 and player 2 direction
 * - SINGLEPLAYER: Changes player 1 direction
 * - INSTRUCTIONS: Waits for user to press ESC
 */

void keyboard_event ();

/**
 * @brief handles mouse interrupts and updates mouse position
 * - MAIN_MENU: If user clicks on a button changes to that modeS
 */

void mouse_event ();

#endif
