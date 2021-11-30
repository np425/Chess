#ifndef NOTATION_H
#define NOTATION_H

/* 
Notation to moves 
*/

#include "chess.h" // PieceType, Square

struct MoveInfo {
    PieceType pieceType = PT_NONE;
	PieceType promote = PT_NONE;

    // Prior coordinates: which piece is moving
    Square from = {-1,-1};

    // Move coordinates: where the piece is moving
    Square to = {-1,-1};

    bool capture = false;

	// bool drawOffer = false;

    bool check = false;
    bool checkmate = false;
    bool castles[2] = { false, false }; // king-side and queen-side castling
};

// ----------- Helper function prototypes
bool readX(char*& it, int &x);
bool readY(char*& it, int &y);

bool readCapture(char*& it, bool &capture);
bool readOptFinalChecks(char*& it, bool &check, bool &checkmate);

bool readOptCaptureXY(char*& it, bool& capture, int& x, int& y);
bool readOptPromoteFinalChecks(char*& it, PieceType& promote, bool& check, bool& checkmate);

// ----------- Main function prototypes
bool translateNotation(char* notation, MoveInfo& info);

#endif
