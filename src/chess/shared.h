//
// Created by np on 2021-12-26.
//

#ifndef CHESS_SHARED_H
#define CHESS_SHARED_H

inline unsigned pow_u(unsigned base, unsigned power) { // No negative powers
    unsigned res = 1; // 1 for negative powers
    while (power > 0) {
        --power;
        res *= base;
    }
    return res;
}

inline const char* textToPositiveInteger(const char* it, unsigned& number) {
    number = 0;
    int l = 0;

    // Find number length
    while (*(it + l) >= '0' && *(it + l) <= '9') {
        ++l;
    }

    if (!l) return it; // No number

    while (l) { // Construct number
        int digit = *it - '0';
        number += (pow_u(10, l - 1) * digit);
        --l;
        ++it;
    }

    return it;
}

inline int numSign(const int num) {
    if (num > 0)
        return 1;
    else if (num < 0)
        return -1;
    else
        return 0;
}

#endif //CHESS_SHARED_H
