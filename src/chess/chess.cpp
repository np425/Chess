#include "chess.h"
#include <cstring>

namespace chess {

const Tags& ChessGame::getTags() const {
	return tags;
}

const Moves& ChessGame::getMoves() const {
	return moves;
}

ChessGame::ChessGame(Board newBoard, PositionInfo posInfo, Tags tags, Moves moves) 
            : Position(newBoard, posInfo), tags(tags), moves(moves) {
}

bool ChessGame::buildPosFromMoves() {
	Moves::iterator move = moves.begin();
	while (move < moves.end() && !isGameOver()) {
		if (!Position::makeMove(move->move)) {
			// Invalid move
			return false;
		}
		++move;
	}
	if (move < moves.end()) {
		// Game ended before all moves were read
		return false; 
	} else {
		return true;
	}
}

bool ChessGame::buildPosFromMoves(Moves& newMoves) {
	moves = newMoves;
	return buildPosFromMoves();
}

void ChessGame::updateTag(Tag tag) {
	if (tags.find(tag.first) == tags.end()) {
		tags.insert(tag);
	} else {
		tags[tag.first] = tag.second;
	}
}

}
