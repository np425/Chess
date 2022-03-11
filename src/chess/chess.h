#ifndef CHESS_H
#define CHESS_H

#include "position.h"
#include "notation/pgn.h"

namespace chess {

class ChessGame : public Position {
	Tags tags;
	Moves moves;

	bool buildPosFromMoves();
public:
	ChessGame(const char*);
	ChessGame(Board={}, PositionInfo={}, Tags tags={}, Moves moves={});

	const Tags& getTags() const;
	const Moves& getMoves() const;

	bool loadPGN(const char*);
	bool buildPosFromMoves(Moves& moves);

	void updateTag(Tag tag);
	bool makeMove(const char*);

	void setTags(Tags tags);
	void setMoves(Moves moves);
};

}

#endif
