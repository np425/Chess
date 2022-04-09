//
// Created by np on 2022-03-20.
//

#include <cctype>
#include "switch_move_notation.h"

bool MoveSwitchParser::parse() {
    while (isspace(*it)) {
        ++it;
    }

    if (!readChar(':')) {
        return false;
    }

    return readUnsignedInt(*moveNo);
}

MoveSwitchParser::MoveSwitchParser(unsigned *move) : moveNo(move) { }