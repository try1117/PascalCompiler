#pragma once
#include <string>
#include "FileReader.h"
#include "Token.h"

class Tokenizer {
private:
	FileReader reader;
	Token token;
	std::string tokenText;
	std::string tokenValue;
	int tokenRow, tokenCol;

	void parseNumber(char c);
	void parseWord(char c);
	void parseOperator(char c);
	void parseSeparator(char c);

	std::string Tokenizer::transformNumber(Token token, std::string text);

public:
	Tokenizer(std::string fileName);
	bool next();
	Token getCurrentToken();
	std::string toString();
};

class LexicalException : public std::exception {
private:
	char message[200];

public:
	LexicalException(int row, int col, std::string mes);
	const char * what() const;
};
