//
// Created by np on 2021-12-26.
//

#include "shared_notation.h"
#include <cctype>

/*
 * Shared notation
 * */

bool readX(const char*& it, int &x) {
    const unsigned char chr = std::tolower(*it);
    if (chr < 'a' || chr > 'h') return false;
    x = chr - 'a'; // a-h

    ++it;
    return true;
}

bool readY(const char*& it, int &y) {
    if (*it < '1' || *it > '8') return false;
    y = *it - '1'; // 1-8

    ++it;
    return true;
}

bool readCapture(const char*& it, bool &capture) {
    if (*it != 'x') return false;
    capture = true;

    ++it;
    return true;
}

// Optional Checks, has to be the end of notation
bool readOptFinalChecks(const char*& it, bool &check, bool &checkmate) {
    if (*it == '+') check = true;
    else if (*it == '#') checkmate = true;
    else return !(*it);

    ++it;

    return !(*it);
}

