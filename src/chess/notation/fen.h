#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#include "notation.h"
#include "../position.h"

namespace chess {

    class FENParser : public BasicNotationParser {
protected:
	Position* pos;

	int readSquare(Piece&);
	bool readBoard(Board&);
	int readCastlingRights(CastlingPerms[2]);
	bool readPassant(Coord&);

public:
	explicit FENParser(Position*, const char* = nullptr);
	bool parse() override;

	Position* getPos() const;
};

}

#endif
