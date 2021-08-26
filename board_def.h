#ifndef BOARD_DEF_H
#define BOARD_DEF_H

/* 
Board and piece mechanics definitions
*/

#include "game_def.h"
#include <memory>
#include <vector>
using namespace std;


/* Type declarations */
// first bit x sign, second bit x, third bit y sign, fourth bit y
// for all continuous movement pieces (bishop, rook, queen)
typedef int Movement;

struct SquareRef;
struct Pin;


/* Type definitions */
struct SquareRef {
    Piece piece; // piece on the square
    vector<SquareRef*> moves; // pieces that can move to this square (projections)
    vector<SquareRef*> protects; // pieces that protect this square
    // Pins for this square (either this square is the attacking piece, pinned piece or the pinned-to piece)
    vector<Pin> pins;
};

struct Pin {
    SquareRef* from; // attacking piece
    SquareRef* pinned; // piece behind the pinned piece (pinned-to piece)
    SquareRef* to; // pinned piece
    Movement movement; // movement number from attacker to pin
};


/* Global variables declarations */

// Indicates which pawn just moved, for en passant
extern SquareRef* pawnPass[2];

// Variables for castling
extern bool leftRookMoved[2];
extern bool rightRookMoved[2];
extern bool kingMoved[2];

// Keeps track of checks
extern vector<SquareRef*> checks[2];

extern SquareRef board[BOARD_SIZE];

/* Helper functions */
void translateMovement(const Movement mov, int &xacc, int &yacc);
bool navigate(int& x, int& y, const int xacc, const int yacc);
bool isPinnedToKing(const SquareRef * const sqr, const int xacc = 0, const int yacc = 0);
bool isSquareProtected(const SquareRef * const sqr, const Player player);
bool preventsCheck(SquareRef * moveFrom, SquareRef * moveTo);
constexpr bool inBounds(const int x, const int y) {
    return (x >= 0 && x < BOARD_XSIZE && y >= 0 && y < BOARD_YSIZE);
}

/* Pin functions */
void getPins(SquareRef * const sqr, vector<Pin>& pins, const Movement * const movementMap, 
             const int movementSize);
void getPins(SquareRef * const sqr, vector<Pin>& pins);


/* Move projection functions */
void getMoves(SquareRef * sqr, vector<SquareRef*>& moves,
                 const Movement * const movementMap, const int movementSize);
void getMoves(SquareRef * sqr, vector<SquareRef*>& moves);


/* Square protection functions */
void getProtects(const SquareRef * const sqr, vector<SquareRef*>& protects,
                 const Movement * const movementMap, const int movementSize);
void getProtects(const SquareRef * const sqr, vector<SquareRef*>& protects);


/* Conversion functions */
string coordToString(const int coord);
string squareToString(const SquareRef * const sqr);
string pinToString(const Pin& pin);

#endif
