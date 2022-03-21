#ifndef CHESS_MOVE_NOTATION_H
#define CHESS_MOVE_NOTATION_H

#include "notation.h"
#include "../basic_types.h"

namespace chess {

class MoveParser : public BasicNotationParser {
	MoveInfo* move;

	int readCoordPoints(Coord&);
	bool readPromoteType(PieceType&);	
	bool readPromoteSymbol();
	bool readCaptureSymbol();
	bool readChecks(CheckType& checks);
	int readComment();
	bool readCastling(CastlingSide&);
	bool readMove(MoveInfo&);

    bool parse() override;

public:
	explicit MoveParser(MoveInfo*);

};

}

#endif
