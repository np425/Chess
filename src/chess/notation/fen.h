#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#include "notation.h"
#include "../position.h"

namespace chess {

const unsigned FEN_MAX_LENGTH = 100;

class FENParser : public BasicNotationParser {
protected:
	Position* pos;

	int readSquare(Piece&);
	bool readBoard(Board&);
	int readCastlingRights(CastlingPerms[2]);
	bool readPassant(Coord&);

public:
	FENParser(Position*, const char* = nullptr);
	virtual bool parse() override;

	Position* getPos() const;
};

}

#endif
