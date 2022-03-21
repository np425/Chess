#ifndef CHESS_PGN_H
#define CHESS_PGN_H

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

    bool parse() override;

	// Result
	bool readResult(GameState&, unsigned);
public:
	explicit PGNParser(ChessGame*);

};

}

#endif
