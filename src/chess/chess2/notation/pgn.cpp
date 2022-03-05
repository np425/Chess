#include "pgn.h"
#include "shared_notation.h" // readPositiveInteger
#include "move_notation.h" // readMoveNotation
#include <cctype> // isspace

namespace chess {

bool tagExists(const TagName given, TagsArray& tags) {
	for (const TagName* it = tags.tags; it < tags.end; it += 2) {
		const char *origIt = *it;
		const char *givenIt = given;

		while (*origIt && *origIt == *givenIt) {
			++origIt;
			++givenIt;
		}

		if (!*givenIt) return true;
	}
	return false;
}

bool readTag(const char*& it, TagsArray& tags) {
	if (*it != '[') return false; // Missing opening tag brackets	
	++it;

	while (isspace(*it)) ++it;

	char *tagIt = *tags.end;

	// Form tag name (could be separated by spaces)
	while (*it != '"' && *it) {
		while (!isspace(*it) && *it != '"') {
			*(tagIt++) = *(it++);
		}

		*(tagIt++) = ' ';
		while (isspace(*it)) ++it;
	}

	// Replace space with null terminator
	if (*tags.end) *(--tagIt) = 0; 

	if (!*tags.end || tagExists(*tags.end, tags)) {
		return false; // End of string, no tag name or tag exists
	}

	++tags.end;

	// Opening tag value quotes
	if (*it != '"') return false; 
	++it;

	tagIt = *tags.end;

	// Read till second quotes
	while (*it != '"' && *it) {
		*(tagIt++) = *(it++);
	}

	// Closing tag value quotes
	if (!*it || *it != '"' || tagIt == *tags.end) {
		--tags.end; // Reset tags head
		return false; // End of string, no tag value or no quotes
	} 
	++it; 
		
	while (isspace(*it)) ++it;

	// Closing tag bracket
	if (*it != ']') {
		--tags.end; // Reset tags head
		return false; 
	}
	++it;

	++tags.end;

	return true;
}
	
void readTags(const char*& it, TagsArray& tags) {
	const char* newIt = it;

	while (readTag(newIt, tags)) {
		while (isspace(*newIt)) ++newIt;
		it = newIt;
	}
}

// TODO: Write function that compares strings
bool readFinalResult(const char*& it, GameState& state, const unsigned lastMoveNum) {
	if (*it == '1') {
		++it;
		if (*it == '/') {
			++it;
			if (*it != '2') return false;
			
			++it;
			if (*it != '-') return false;

			++it;
			if (*it != '1') return false;

			++it;
			if (*it != '/') return false;
			
			++it;
			if (*it != '2') return false;

			int sign = (lastMoveNum % 2 == 0 ? +1 : -1);
			state = sign * GS_DRAW;
		} else if (*it == '-') {
			++it;
			if (*it != '0') return false;

			state = +GS_WIN;
		} else return false;
	} else if (*it == '0') {
		++it;
		if (*it != '-') return false;

		++it;
		if (*it != '1') return false;

		state = -GS_WIN;
	} else return false; // Unknown result

	++it;
	return it;
}

bool readMoveNum(const char*& it, unsigned& num) {
	const char* newIt = it;
	if (!readPositiveInteger(newIt, num) || num == 0) {
		return false; // Must be integer higher than 0
	}

	if (*newIt != '.') {
		return false; // Must have a dot afterwards
	}

	it = newIt;
	++it; // Skip dot
	return true;
}

bool readMove(const char*& it, MovesArray& moves) {
	const char *itEnd = it;

	// Comments inclusive
	if (!readMoveNotation(itEnd, moves.end->move)) {
		return false; 
	}

	// Set notation in the moves array
	char* notationIt = moves.end->notation;
	while (it != itEnd) {
		*(notationIt++) = *(it++);
	}
	*notationIt = 0; // Null terminator

	++moves.end;

	return true;
}

bool readMoves(const char*& it, MovesArray& moves, unsigned &lastMoveNum) {
	unsigned fullMoveNum;
		
	// First move in PGN, every following move assumes in increasing-by-1 order 
	if (!readMoveNum(it, fullMoveNum)) fullMoveNum = 1;

	lastMoveNum = (fullMoveNum-1) * 2;
	while (isspace(*it)) ++it; 

	unsigned count = 0; // For keeping track of full moves

	while (readMove(it, moves)) {
		while (isspace(*it)) ++it; 
		++lastMoveNum;
		++count;

		// Every full move, check for optional move number
		if (count % 2 == 0 && readMoveNum(it, fullMoveNum)) {
			if (lastMoveNum != (fullMoveNum-1)*2) {
				return false; // Move numbers must be increasing by one
			}
		} else fullMoveNum = 0;
	}

	// Must be move after move number
	if (fullMoveNum) return false;

	// After not being able to read a move
	// assume it's the end of moves list
	return true;
}

bool readPGN(const char*& it, TagsArray& tags, MovesArray& moves, 
             unsigned& lastMoveNum, GameState& state) {

	while (isspace(*it)) ++it;
	readTags(it, tags); // Skips whitespace

	if (!readMoves(it, moves, lastMoveNum)) return false;

	const char *newIt = it;

	// Result is optional
	if (readFinalResult(newIt, state, lastMoveNum)) it = newIt;

	return true;
}

}
