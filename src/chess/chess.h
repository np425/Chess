#ifndef CHESS_H
#define CHESS_H

#include "position.h"
#include "notation/pgn.h"

namespace chess {

class ChessGame : public Position {
	TagsArray tags;
	MovesArray moves;

	bool buildPosFromMoves();
	Tag* findTag(const char*);
public:
	ChessGame(const char*);
	ChessGame(Board={}, PositionInfo={}, TagsArray tags={}, MovesArray moves={});

	const TagsArray& getTags() const;
	const MovesArray& getPreviousMoves() const;

	bool loadPGN(const char*);
	bool buildPosFromMoves(MovesArray& moves);

	void updateTag(Tag tag);
	bool makeMove(const char*);
};

}

#endif
