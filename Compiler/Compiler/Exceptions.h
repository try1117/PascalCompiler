#pragma once
#include <string>

class LexicalException : public std::exception {
public:
	LexicalException(int row, int col, std::string mes);
	const char * what() const;

private:
	char message[200];
};

class SyntaxException : public std::exception {
public:
	SyntaxException(int row, int col, std::string mes);
	const char * what() const;

private:
	char message[200];
};
