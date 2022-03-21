#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "../basic_types.h"

namespace chess {

class BasicNotationParser {
protected:
	const char* it = nullptr;

	// Basic reading functions
	bool readX(int&);
	bool readY(int&);
	bool readPlayer(Player& pl);
    bool readChar(char);
	int readUnsignedInt(unsigned&);
	int readInsensitiveString(const char*);
	int readString(const char*);

    virtual bool parse() = 0;
public:
	bool parseStr(const char*);

	// Returns pointer to parsing end
	const char* end() const;
};

}

#endif
