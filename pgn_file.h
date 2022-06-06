#ifndef PGN_FILE_H
#define PGN_FILE_H

#include "chessl/chess.h"
#include <iostream>

bool loadPGNFromFile(std::istream &f, chess::ChessGame &chess);
bool savePGNToFile(std::ostream &f, const chess::ChessGame &chess);

#endif
