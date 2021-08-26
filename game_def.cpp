#include "game_def.h"
#include <cstdlib>
#include <iostream>
using namespace std;

/* Conversion functions */
char pieceTypeToChar(const PieceType type) {
     switch (type) {
         case P_NONE:
             return '_';
         case P_ROOK:
             return 'R';
         case P_KNIGHT:
             return 'N';
         case P_BISHOP:
             return 'B';
         case P_QUEEN:
             return 'Q';
         case P_KING:
             return 'K';
         case P_PAWN:
             return 'P';
         default:
             cerr << "Could not convert PieceType to char (" << (int)type << ")" << endl;
             abort();
     }
}

char pieceToChar(const Piece piece) {
    return pieceTypeToChar(pieceToPieceType(piece));
}

string playerToString(const Player player) {
    switch (player) {
        case P_WHITE:
            return "White";
        case P_BLACK:
            return "Black";
        default:
            cerr << "Unknown player" << (int)player << endl;
            abort();
    }
}

/* Conversion functions */
/*char PieceTypeToChar(enum PieceType type) {
     switch (type) {
         case P_NONE:
             return '_';
         case P_ROOK:
             return 'R';
         case P_KNIGHT:
             return 'N';
         case P_BISHOP:
             return 'B';
         case P_QUEEN:
             return 'Q';
         case P_KING:
             return 'K';
         case P_PAWN:
             return 'P';
         default:
             cerr << "Could not convert PieceType to char (" << (int)type << ")" << endl;
             abort();
     }
}

char PieceToChar(const Piece piece) {
    return PieceTypeToChar(PieceToPieceType(piece));
}*/
