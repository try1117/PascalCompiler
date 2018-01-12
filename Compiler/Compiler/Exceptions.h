#pragma once
#include <string>

class LexicalException : public std::exception {
private:
	char message[200];

public:
	LexicalException(int row, int col, std::string mes);
	const char * what() const;
};
