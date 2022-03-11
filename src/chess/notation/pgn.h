#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#define PGN_MAX_LENGTH 10000

#include "notation.h"
#include "../chess.h"

namespace chess {

class PGNParser : public BasicNotationParser {
	ChessGame game;

	// Tags
	bool readTag(Tag& tag);
	bool readTags(Tags& tags);

	// Moves
	bool readMoveNum(unsigned& num);
	bool readMove(NotatedMove& move);
	bool readMoves(Moves& moves, unsigned& lastMoveNum);

	// Result
	bool readResult(GameState& state, const unsigned lastMoveNum);
public:
	PGNParser();
	PGNParser(const char*);

	virtual bool parse() override;

	ChessGame& getGame();
};

}

#endif
