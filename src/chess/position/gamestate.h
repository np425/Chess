#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

/*

Functions related to the state of the game

*/

#include "../types.h"

namespace chess {

// Checks if the position is checkmate for the player
bool isCheckmate(const Position& pos, const Player pl);

// Checks if position is stalemate for the player
bool isStalemate(const Position& pos, const Player pl);

// Updates game state for the player to move next
void updateGameState(Position& pos);

}

#endif
