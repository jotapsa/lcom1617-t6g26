#ifndef __MP_LOCAL_H
#define __MP_LOCAL_H

short *game_field;

/**
 * @brief Calls functions to start local Multiplayer
 */

void start_mp_local ();

/**
 * @brief What to do every tick
 */

void mp_local_tick ();

/**
 * @brief Calls functions to end local Multiplayer
 */

void end_mp_local ();


#endif
