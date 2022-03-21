//
// Created by np on 2022-03-20.
//

#ifndef CHESS_SWITCH_MOVE_NOTATION_H
#define CHESS_SWITCH_MOVE_NOTATION_H

#include "chess/notation/notation.h"

class MoveSwitchParser : public chess::BasicNotationParser {
    unsigned* moveNo;

    bool parse() override;
public:
    explicit MoveSwitchParser(unsigned*);
};

#endif //CHESS_SWITCH_MOVE_NOTATION_H
