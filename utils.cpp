#include "utils.h"

bool isUppercase(const char c) {
	return (c >= 'A' && c <= 'Z');
}

char charToLower(const char c) {
	if (c >= 'A' && c <= 'Z') return c + 32;
	else return c;
}

void textToLower(char* text) {
	for (char* c = text; *c; ++c) {
		*c = charToLower(*c);
	}
}

int pow(int base, int power) { // No negative powers
    int res = 1; // 1 for negative powers
    while (power > 0) {
        --power;
        res *= base;
    }
    return res;
}

char* textToPositiveInteger(char* it, unsigned& number) {
	number = 0;
	int l = 0;

	// Find number length
    while (*(it + l) >= '0' && *(it + l) <= '9') {
        ++l;
    }

    if (!l) return it; // No number

    while (l) { // Construct number
        int digit = *it - '0';
        number += (pow(10, l - 1) * digit);
        --l;
        ++it;
    }	

	return it;
}

void skipSpaces(char*& it) {
	while (*it == ' ') ++it;
}
