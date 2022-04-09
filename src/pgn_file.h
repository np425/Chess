#ifndef PGN_FILE_H
#define PGN_FILE_H

#include "chess/chess.h"

bool loadPGNFromFile(const std::string &fileName, chess::ChessGame &chess);
bool savePGNToFile(const std::string &fileName, const chess::ChessGame &chess);

#endif
