#include "notation.h"
#include <cctype>

namespace {

// Unsigned power to 10 function
unsigned u_pow10(unsigned power) {
    unsigned res = 1;

    while (power > 0) {
        --power;
        res *= 10;
    }
    return res;
}

}

namespace chess {

bool BasicNotationParser::parseStr(const char *expr) {
    it = expr;
    return parse();
}

const char *BasicNotationParser::end() const {
    return it;
}

bool BasicNotationParser::readX(int &x) {
    unsigned char chr = toupper(*it);
    if (chr < 'A' || chr > 'H') {
        return false;
    }

    x = chr - 'A';

    ++it;
    return true;
}

bool BasicNotationParser::readY(int &y) {
    unsigned char chr = toupper(*it);
    if (chr < '1' || chr > '8') {
        return false;
    }

    y = chr - '1';

    ++it;
    return true;
}

bool BasicNotationParser::readPlayer(Player &pl) {
    switch (toupper(*it)) {
        case 'W':
            pl = WHITE;
            break;
        case 'B':
            pl = BLACK;
            break;
        default:
            return false;
    }
    ++it;
    return true;
}

bool BasicNotationParser::readChar(const char expected) {
    if (*it != expected) {
        return false;
    }

    ++it;
    return true;
}

int BasicNotationParser::readUnsignedInt(unsigned &num) {
    int l = 0;
    num = 0;

    while (*(it + l) >= '0' && *(it + l) <= '9') {
        ++l;
    }

    if (!l) {
        // No number
        return 0;
    }

    for (int i = l; i; --i) { // Construct number
        unsigned digit = *it - '0';
        num += (u_pow10(l - 1) * digit);
        ++it;
    }
    return l;
}

int BasicNotationParser::readInsensitiveString(const char *str) {
    int i = 0;
    while (str[i]) {
        if (toupper(str[i]) != toupper(it[i])) {
            return false;
        }
        ++i;
    }

    it += i;
    return i;
}

int BasicNotationParser::readString(const char *str) {
    int i = 0;
    while (str[i]) {
        if (toupper(str[i]) != toupper(it[i])) {
            return false;
        }
        ++i;
    }

    it += i;
    return i;
}

}
