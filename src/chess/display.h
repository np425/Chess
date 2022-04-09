#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include "utils.h"
#include "board.h"
#include "position.h"
#include "chess.h"

namespace chess {

char pieceToChar(Piece);
const char *stateToString(GameState);
const char *playerToString(Player);

void displayBoard(const Board &);
void displayPosInfo(const Position &);
void displayPieces(const Board &, const Coords &);

void displayMoveInfo(const MoveInfo &);
void displayTags(const Tags &);

void displayInterface(const ChessGame &);

}

#endif
