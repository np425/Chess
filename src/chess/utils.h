#ifndef CHESS_UTILS_H
#define CHESS_UTILS_H

#include "basic_types.h"
#include <cmath> // abs

/*

Utility functions for chess

*/

namespace chess {

inline Player pieceToPlayer(const Piece piece) {
	if (piece > 0) return WHITE;
	else if (piece < 0) return BLACK;
	else return PL_NONE;
}

inline PieceType pieceToType(const Piece piece) {
	return (PieceType) abs(piece);
}
	
inline bool inBounds(const int x, const int y) {
	return (x >= 0 && x < BOARD_SIZE_X && y >= 0 && y < BOARD_SIZE_Y);
}

// Assume player is not PL_NONE
inline int getPlayerSign(const Player pl) {
	return (pl == WHITE ? 1 : -1);
}

inline GameStateType stateToType(const GameState state) {
	return (GameStateType) abs(state);
}

inline Player stateToPlayer(const GameState state) {
	if (state > 0) return WHITE;
	else if (state < 0) return BLACK;
	else return PL_NONE;
}

// Gets the sign of a number, returns 0 if number is 0
inline int numSign(const int num) {
	if (num > 0) return 1;
	else if (num < 0) return -1;
	else return 0;
}

}

#endif
