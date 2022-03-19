#include "chess.h"
#include <utility>

namespace chess {

Tags ChessGame::getTags() const {
	return tags;
}

Moves ChessGame::getMoves() const {
	return moves;
}

ChessGame::ChessGame(Board newBoard, PositionInfo posInfo, Tags tags, Moves moves) 
            : Position(newBoard, posInfo), tags(std::move(tags)), moves(std::move(moves)) {
}

void ChessGame::setMoves(Moves newMoves) {
	moves = std::move(newMoves);
}

void ChessGame::setTags(Tags newTags) {
    tags = std::move(newTags);
}

bool ChessGame::buildPos() {
	auto move = moves.begin();
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
	if (!Position::makeMove(move.move)) {
		return false;
	}
	moves.push_back(move);
	return true;
}

}
