#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#include "notation.h"
#include "../position.h"
#include "chess.h"

namespace chess {

class FENParser : public BasicNotationParser {
protected:
    ChessGame *game;

    int readSquare(Piece &);
    bool readBoard(Piece *pieces);
    int readCastlingRights(CastlingPerms[2]);
    bool readPassant(Coord &);

    bool parse() override;

public:
    explicit FENParser(ChessGame *);
};

}

#endif
