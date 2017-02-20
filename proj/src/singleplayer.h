#ifndef __SINGLEPLAYER_H
#define __SINGLEPLAYER_H

short *game_field;

/**
 * @brief Calls functions to start Singleplayer
 */

void start_singleplayer();

/**
 * @brief What to do every tick
 */

void single_local_tick();

/**
 * @brief Calls functions to end Singleplayer
 */

void end_singleplayer();

#endif
