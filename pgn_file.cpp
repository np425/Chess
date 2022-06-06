#include "pgn_file.h"
#include "chessl/pgn.h"
#include <iostream>

using namespace chess;

bool loadPGNFromFile(std::istream &f, ChessGame &game) {
    std::string pgn((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    const char *it = pgn.c_str();

    return PGNParser::parsePGN(game, it);
}

bool savePGNToFile(std::ostream &f, const ChessGame &game) {
    const TagsMap &tags = game.getTags();

    for (const auto &tag: tags) {
        f << "[" << tag.first << " \"" << tag.second << "\"]" << std::endl;
    }

    if (!tags.empty()) {
        f << std::endl;
    }

    int moveNum = 0;

    MovesList moves = game.getMovesList();
    for (auto &move: moves) {
        if (moveNum % 2 == 0) {
            f << (moveNum / 2) + 1 << ". ";
        }
        f << move.notation << " ";
        ++moveNum;
    }

    // Game can only end by stalemate or checkmate
    State state = game.getState();
    Player player = stateToPlayer(state);

    switch (stateToType(state)) {
        case WIN:
        case CHECKMATE:
            if (player == WHITE) {
                f << "1-0";
            } else {
                f << "0-1";
            }
            break;
        case DRAW:
        case STALEMATE:
        case PLAYING:
            f << "1/2-1/2";
            break;
    }

    f << std::endl;

    return true;
}
