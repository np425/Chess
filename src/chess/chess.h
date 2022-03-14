#ifndef CHESS_H
#define CHESS_H

#include "position.h"

namespace chess {

class ChessGame : public Position {
	Tags tags;
	Moves moves;

public:
	ChessGame(const char*);
	ChessGame(Board={}, PositionInfo={}, Tags={}, Moves={});

	const Tags& getTags() const;
	const Moves& getMoves() const;

	bool loadPGN(const char*);
	bool buildPos();

	void updateTag(Tag);
	bool makeMove(NotatedMove);

	void setTags(Tags);
	void addTags(Tags);
	void setMoves(Moves);
};

}

#endif
