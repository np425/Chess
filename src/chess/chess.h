#ifndef CHESS_H
#define CHESS_H

#include "position.h"

namespace chess {

class ChessGame {
	Tags tags;
	Moves moves;
    Moves::iterator curMove;
    Position initialPos;
    Position curPos;

public:
	explicit ChessGame(const Piece* = DEFAULT_BOARD, PositionInfo = {});

	const Tags& getTags() const;
	const Moves& getMoves() const;
    Moves::const_iterator getCurMove() const;

    const Position& getPos() const;

    void setBoard(Piece*);
    void setPosition(const Piece * = DEFAULT_BOARD, PositionInfo= {});
    void setPositionInfo(const PositionInfo &);

	bool buildPos();
    bool switchMove(unsigned);

	void updateTag(Tag);
	bool makeMove(NotatedMove);

	void addTags(Tags);
	void setMoves(Moves);

    bool isGameOver();
    bool validate();
};

}

#endif
