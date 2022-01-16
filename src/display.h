#ifndef DISPLAY_H
#define DISPLAY_H

#include "chess/standard.h"

/*
Chess representation in console window
*/

// ----------- Constants
#ifdef __unix__ 
#define COLOUR_END "\033[0m"
#define COLOUR_WHITE "\033[37m"
//const char* BLACK_COLOUR = "\033[30m";
#define COLOUR_BLACK "\033[31m"
#endif

// ----------- Display functions
void displayBoard(StandardChess& chess);

char pieceToChar(StandardChess::Piece piece);

#ifdef __unix__
const char* setPieceColour(StandardChess::Piece piece);
#endif

#endif
