// TODO: Put in separate cpp

#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#include "notation.h"
#include "move_notation.h"
#include "chess.h"

namespace chess {
class PGNParser : public NotationParser {
public:
    static bool parseTag(Tag &outTag, const char *&outIt) {
        Tag tag;
        auto it{outIt};

        if (!expectChar('[', it)) {
            // Missing opening tag brackets
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        // Form a tag name (could be separated by spaces)
        while (*it != '"' && *it) {
            while (*it != ' ' && *it != '"') {
                tag.first += *(it++);
            }

            tag.first += ' ';
            CharUtils::skipWhiteSpace(it);
        }

        if (tag.first.empty()) {
            return false;
        }

        // Remove remaining space
        tag.first.pop_back();

        CharUtils::skipWhiteSpace(it);

        // Opening tag value quotes
        if (!expectChar('"', it)) {
            return false;
        }

        // Read till second quotes
        while (*it != '"' && *it) {
            tag.second += *(it++);
        }

        // Closing tag value quotes
        if (tag.second.empty() || !expectChar('"', it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        // Closing tag bracket
        if (!expectChar(']', it)) {
            return false;
        }

        outIt = it;
        outTag = std::move(tag);
        return true;
    }

    static bool parseTags(TagsMap &outTags, const char *&outIt) {
        auto it{outIt};
        TagsMap tags;
        Tag tag;

        while (parseTag(tag, it)) {
            if (tags.find(tag.first) != tags.end()) {
                // Tag already exists
                return false;
            }

            CharUtils::skipWhiteSpace(it);
            tags.insert(tag);
        }

        outTags = std::move(tags);
        outIt = it;
        return true;
    }

    static bool parseResult(State &outState, int lastMoveNum, const char *&outIt) {
        if (expectInsensitiveString("1/2", outIt)) {
            int plSign = (lastMoveNum % 2 == 0 ? +1 : -1);
            outState = DRAW * plSign;
        } else if (expectInsensitiveString("1-0", outIt)) {
            outState = WIN;
        } else if (expectInsensitiveString("0-1", outIt)) {
            outState = -WIN;
        } else {
            return false;
        }
        return true;
    }

    static bool parseMoveNum(int &outMoveNum, const char *&outIt) {
        auto it{outIt};
        int moveNum;

        if (!parseUnsignedInt(moveNum, it) || moveNum == 0 || !expectChar('.', it)) {
            return false;
        }

        outIt = it;
        outMoveNum = (moveNum - 1) * 2;
        return true;
    }

    static bool parseMove(NotatedMove &outMove, const char *&outIt) {
        NotatedMove move;
        auto itEnd{outIt};

        if (!MoveNotationParser::parseMove(move, itEnd)) {
            return false;
        }

        move.notation.assign(outIt, itEnd);

        outIt = itEnd;
        outMove = move;
        return true;
    }

    static bool parseMoves(MovesList &outMoves, int &outLastMoveNum, const char *&outIt) {
        int expectedMoveNum;
        int gotMoveNum;
        auto it{outIt};

        // First move in PGN, every following move assumes in increasing-by-1 order
        if (!parseMoveNum(expectedMoveNum, it)) {
            expectedMoveNum = 0;
        } else {
            CharUtils::skipWhiteSpace(it);
        }

        NotatedMove move;
        MovesList moves;
        bool moveAfterNum{false};

        while (parseMove(move, it)) {
            CharUtils::skipWhiteSpace(it);

            ++expectedMoveNum;
            moveAfterNum = true;

            // Every full move, check for optional move number
            if (expectedMoveNum % 2 == 0 && parseMoveNum(gotMoveNum, it)) {
                CharUtils::skipWhiteSpace(it);
                if (expectedMoveNum != gotMoveNum) {
                    // Move numbers must be increasing by one
                    return false;
                }

                moveAfterNum = false;
            }

            moves.push_back(move);
        }

        if (!moveAfterNum) {
            return false;
        }

        // After not being able to read a move
        // assume it is the end of the moves list
        outMoves = std::move(moves);
        outLastMoveNum = expectedMoveNum;
        outIt = it;
        return true;
    }

    static bool parsePGN(ChessGame &outGame, const char *&outIt) {
        TagsMap tags;
        MovesList moves;
        State state;

        int lastMoveNum;
        auto it{outIt};

        if (!parseTags(tags, it)) {
            return false;
        }
        CharUtils::skipWhiteSpace(it);

        // Skips space
        if (!parseMoves(moves, lastMoveNum, it)) {
            return false;
        }

        auto priorIt{it};
        CharUtils::skipWhiteSpace(it);

        if (!parseResult(state, lastMoveNum, it)) {
            it = priorIt;
        }

        outGame = ChessGame{tags, moves, literals::STANDARD_POSITION};
        outIt = it;
        return true;
    }
};
}

#endif //CHESS_PGN_H
