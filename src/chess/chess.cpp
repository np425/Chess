#include "chess.h"
#include <utility>

namespace chess {

ChessGame::ChessGame(const Piece *pieces, PositionInfo posInfo) : initialPos(pieces, posInfo), curPos(pieces, posInfo),
                                                                  curMove(moves.end()) { }

const Tags &ChessGame::getTags() const {
    return tags;
}

const Moves &ChessGame::getMoves() const {
    return moves;
}

Moves::const_iterator ChessGame::getCurMove() const {
    return curMove;
}

const Position &ChessGame::getPos() const {
    return curPos;
}

void ChessGame::setMoves(Moves newMoves) {
    moves = std::move(newMoves);
    curMove = moves.end();
}

bool ChessGame::buildPos() {
    if (!switchMove(moves.size())) {
        return false;
    }

    curMove = moves.end();
    return true;
}

bool ChessGame::switchMove(unsigned moveNo) {
    // Ensure moveNo does not exceed how many moves are actually stored
    moveNo = moveNo > moves.size() ? moves.size() : moveNo;
    unsigned m = 0;

    // Load initial position
    curPos = initialPos;

    while (m < moveNo && !isGameOver()) {
        if (!curPos.makeMove(moves[m].move)) {
            // Invalid move
            return false;
        }
        ++m;
    }

    curMove = moves.begin() + m;

    return m >= moveNo;
}

void ChessGame::updateTag(Tag tag) {
    if (tags.find(tag.first) == tags.end()) {
        tags.insert(tag);
    } else {
        tags[tag.first] = tag.second;
    }
}

void ChessGame::addTags(Tags newTags) {
    tags.insert(newTags.begin(), newTags.end());
}

bool ChessGame::makeMove(NotatedMove move) {
    if (!curPos.makeMove(move.move)) {
        return false;
    }
    if (curMove != moves.end()) {
        curMove = moves.erase(curMove, moves.end());
    }
    curMove = moves.insert(curMove, move);
    ++curMove;

    return true;
}

void ChessGame::setBoard(Piece *pieces) {
    curPos.setBoard(pieces);
    initialPos.setBoard(pieces);
    moves.clear();
    curMove = moves.end();
}

void ChessGame::setPosition(const Piece *pieces, PositionInfo posInfo) {
    initialPos.setPosition(pieces, posInfo);
    curPos.setPosition(pieces, posInfo);
    moves.clear();
    curMove = moves.end();
}

void ChessGame::setPositionInfo(const PositionInfo &posInfo) {
    initialPos.setPositionInfo(posInfo);
    curPos.setPositionInfo(posInfo);
    moves.clear();
    curMove = moves.end();
}

bool ChessGame::isGameOver() {
    return curPos.getState() != PLAYING;
}

bool ChessGame::validate() {
    return initialPos.validate() && curPos.validate();
}

}
