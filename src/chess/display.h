#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include "utils.h"
#include "board.h"
#include "position.h"

namespace chess {

char pieceToChar(Piece piece);
const char* stateToString(GameState gameState);
const char* playerToString(Player player);

void displayBoard(const Board& board);
void displayPosInfo(const Position& pos);
void displayPieces(const Board& board, const Coords& squares);

void displayMoveInfo(const MoveInfo& move);
void displayTags(const Tags& tags);

}

#endif
