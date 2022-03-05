#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include "utils.h"
#include "board.h"
#include "position.h"

namespace chess {

char pieceToChar(const Piece piece);
const char* stateToString(const GameState gameState);
const char* playerToString(const Player player);

void displayBoard(const Board& board);
void displayPosInfo(const Position& pos);
void displayPieces(const Board& board, const CoordArray& squares);

void displayMoveInfo(const MoveInfo& move);

}

#endif
