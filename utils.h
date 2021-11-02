#ifndef UTILS_H
#define UTILS_H

/*
Functions shared between various files
*/

bool isUppercase(const char c);

char charToLower(const char c);

void textToLower(char* text);

int pow(int base, int power); // No negative powers

char* textToPositiveInteger(char* it, unsigned& number);

void skipSpaces(char*& it);

#endif
