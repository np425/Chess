#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#define PGN_MAX_LENGTH 200

#include "types.h"

/*

https://en.wikipedia.org/wiki/Portable_Game_Notation

Simplified PGN notation:
- Simplified move notation
- Allowed whitespace
- Simplified result notation, deduced from white

*/

namespace chess {

// Checks if a given tag exists in tags array
bool tagExists(const TagName given, TagsArray& tags);

// Reads a tag into tagsArray
// Returns: 
// - nullptr if no tag or malformed tag
// - pointer to where it stopped reading otherwise
bool readTag(const char*& it, TagsArray& tags);

// Reads all tags until none are left into the tags array
void readTags(const char*& it, TagsArray& tags);

// Reads final result
// lastMoveNum deduces which player drew the game
// Results:
// 1 = Checkmate by white
// 1- = Stalemate
// 0 = Checkmate by black
// Returns: nullptr if expression malformed, otherwise pointer to where it finished reading
bool readFinalResult(const char*& it, GameState& state, const unsigned lastMoveNum);

// Reads a move number, move number must be higher than 0
// and preceding a dot
bool readMoveNum(const char*& it, unsigned& num);

// Reads a move and saves it in moves array, including given notation
bool readMove(const char*& it, MovesArray& moves);

// Reads all moves until none are left into moves array, including notation
// Full move number is optional except for the first move, 
// every real move is assumed to be increased by one unless specified
// If specified real move number must be increased by one
// Parameter lastFullMove: last move's number
bool readMoves(const char*& it, MovesArray& moves, unsigned& lastMoveNum);

// Reads tags, moves and final result
// Everything is optional, except for the moves
// Parameter lastFullMove: last full move's number
bool readPGN(const char*& it, TagsArray& tags, MovesArray& moves, 
             unsigned& lastMoveNum, GameState& state);
}

#endif
