#include "chess.h"

namespace chess {

const TagsArray& ChessGame::getTags() const {
	return tags;
}

const MovesArray& ChessGame::getPreviousMoves() const {
	return moves;
}

ChessGame::ChessGame(const char* pgn) {
	loadPGN(pgn);
}

ChessGame::ChessGame(Board newBoard, PositionInfo posInfo, TagsArray newTags, MovesArray newMoves) 
            : Position(newBoard, posInfo), tags(newTags), moves(newMoves) {}

bool ChessGame::loadPGN(const char* pgn) {
	return readPGN(pgn, tags, moves, fullMoves, state);
}

}
