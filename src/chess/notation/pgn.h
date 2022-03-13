#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#define PGN_MAX_LENGTH 10000

#include "notation.h"
#include "../chess.h"

namespace chess {

class PGNParser : public BasicNotationParser {
	ChessGame* game;

	// Tags
	bool readTag(Tag&);
	bool readTags(Tags&);

	// Moves
	bool readMoveNum(unsigned&);
	bool readMove(NotatedMove&);
	bool readMoves(Moves&, unsigned&);

	// Result
	bool readResult(GameState&, const unsigned);
public:
	PGNParser(ChessGame*, const char* = nullptr);

	virtual bool parse() override;

	ChessGame* getGame() const;
};

}

#endif
