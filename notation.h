#ifndef NOTATION_H
#define NOTATION_H

/* 
Notation reading & conversion to notation functions 
*/

#include <string>
#include "board_def.h"
using namespace std;

/* Type declarations */
struct NotationInfo {
	// Displays whether given notation is not valid
	bool badNotation = false;

	// Piece information: type, where piece moves, if it captures other piece, and where it was prior moving
	PieceType pieceType;
    
    // Algebraic notation (prior)
	int px = -1;
	int py = -1;

    // Move notation (where it moves to)
	int mx = -1;
	int my = -1;

	// Other status information: takes, checks, checkmates, castles and draw offers
	bool takes = false;
	bool check = false;
	bool checkmate = false;
	bool qCastles = false;
	bool kCastles = false;
	bool drawOffer = false;
};


/* Generic helper functions */
//void strToLower(string& str);


/* Specific read functions */
bool readX(const string& notation, string::iterator cur, int &x); // Expect X
bool readY(const string& notation, string::iterator cur, int &y); // Expect Y
bool readTakes(const string& notation, string::iterator cur, bool& takes); // Expect takes (x)
// Expect either the end of notation, check or checkmate symbolic (+,#)
bool readOptFinalChecks(const string& notation, string::iterator cur, bool& check, bool& checkmate);


/* Multi-read functions */

// Expect Y->(+,#)
bool readYOptChecks(const string& notation, string::iterator cur, int& y, bool& check, bool& checkmate);
// Expect X->Y->(+,#)
bool readXYOptChecks(const string& notation, string::iterator cur, int& x, int& y, bool& check, bool& checkmate);
// Expect (x)->X->Y->(+,#)
bool readOptTakesXYOptChecks(const string& notation, string::iterator cur, bool takes, int& x, int& y, bool &check, bool& checkmate);


void translateNotation(string notation, NotationInfo& info);


#endif
