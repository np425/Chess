#ifndef CHESS_H
#define CHESS_H

#include "position.h"

namespace chess {

class ChessGame : public Position {
	Tags tags;
	Moves moves;

public:
	explicit ChessGame(Board={}, PositionInfo={}, Tags={}, Moves={});

	Tags getTags() const;
	Moves getMoves() const;

	bool buildPos();

	void updateTag(Tag);
	bool makeMove(NotatedMove);

	void setTags(Tags);
	void addTags(Tags);
	void setMoves(Moves);
};

}

#endif
