#ifndef FEN_H
#define FEN_H

#include "chess.h"

/* 
FEN notation to position 

https://en.wikipedia.org/wiki/Forsyth-Edwards_Notation
*/

// ----------- Helper function prototypes
int readPiece(char*& it, Piece& piece);

bool readPlayer(char*& it, Player& player);

bool readPassant(char*& it, Square& sqr);
bool readCastlingRights(char*& it, bool castlingRights[2][2]);

bool readPositiveInteger(char*& start, unsigned& number);

// ----------- Main function prototypes
bool loadFEN(char* fen);

#endif
