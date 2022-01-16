//
// Created by np on 2021-12-26.
//

/*
 * Shared notation
 * */

#ifndef CHESS_SHARED_NOTATION_H
#define CHESS_SHARED_NOTATION_H

bool readX(const char*& it, int &x);
bool readY(const char*& it, int &y);

bool readCapture(const char*& it, bool &capture);
bool readOptFinalChecks(const char*& it, bool &check, bool &checkmate);

#endif //CHESS_SHARED_NOTATION_H
