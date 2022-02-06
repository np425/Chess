#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include "types.h"

namespace chess {
char pieceToChar(const Piece piece);
void displayBoard(const Piece* board);
void displayPosInfo(const Position& pos);
void displayPieces(const Piece* board, const CoordArray& squares);
const char* stateToString(const GameState gameState);
const char* playerToString(const Player pl);
void displayMoveInfo(const MoveInfo& move);
void displayTags(const TagsArray& tags);
void displayMoves(const MovesArray& moves);
void displayPGNInfo(const TagsArray& tags, const MovesArray& moves, const GameState state);
const char* stateToString(const GameState gameState);
}

#endif
