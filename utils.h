#ifndef UTILS_H
#define UTILS_H

/*
Functions shared between various files
*/

#include "utils.h"
#include <cctype> //tolower

inline void textToLower(char* text) {
	for (char* c = text; *c; ++c) {
		*c = tolower(*c);
	}
}

inline unsigned upow(unsigned base, unsigned power) { // No negative powers
    int res = 1; // 1 for negative powers
    while (power > 0) {
        --power;
        res *= base;
    }
    return res;
}

inline char* textToPositiveInteger(char* it, unsigned& number) {
	number = 0;
	int l = 0;

	// Find number length
    while (*(it + l) >= '0' && *(it + l) <= '9') {
        ++l;
    }

    if (!l) return it; // No number

    while (l) { // Construct number
        int digit = *it - '0';
        number += (upow(10, l - 1) * digit);
        --l;
        ++it;
    }	

	return it;
}

#endif
