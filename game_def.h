#ifndef GAME_DEF_H
#define GAME_DEF_H

/*
Basic game definitions 
*/

#include <string>
using namespace std;

/* Type declarations */
enum PieceType {
    P_NONE = 0, P_ROOK, P_KNIGHT, P_BISHOP, P_QUEEN, P_KING, P_PAWN
};

enum Player {
    P_WHITE = 0, P_BLACK = 1
};

typedef int Piece; // PieceType except sign represents whose piece it is


/* Constants */
constexpr int BOARD_XSIZE = 8;
constexpr int BOARD_YSIZE = 8;
constexpr int BOARD_SIZE = BOARD_YSIZE * BOARD_XSIZE;

constexpr int PIECE_MASK_SIZE = 16;
constexpr char PIECE_MASK[PIECE_MASK_SIZE] = { 1,2,3,4,5,3,2,1,6,6,6,6,6,6,6,6 };


/* Conversion functions */
char pieceTypeToChar(const PieceType type);
char pieceToChar(const Piece piece); 
string playerToString(const Player player);

constexpr Player pieceToPlayer(const Piece piece) { return (Player)(piece < 0); }
constexpr PieceType pieceToPieceType(const Piece piece) { return (PieceType)(abs(piece)); }

#endif
