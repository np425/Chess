#ifndef CHESS_MOVE_NOTATION_H
#define CHESS_MOVE_NOTATION_H

#include "notation.h"
#include "../basic_types.h"

#define MOVE_LENGTH 20

namespace chess {

class MoveParser : public BasicNotationParser {
	MoveInfo move;

	int readCoordPoints(Coord&);
	bool readPromoteType(PieceType&);	
	bool readPromoteSymbol();
	bool readCaptureSymbol();
	bool readChecks(CheckType& checks);
	int readComment();
	bool readCastling(CastlingSide&);
	bool readMove(MoveInfo&);

public:
	MoveParser();
	MoveParser(const char*);
	virtual bool parse() override;

	MoveInfo& getMove();
};

}

#endif
