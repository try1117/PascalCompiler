#pragma once
#include <string>
#include "FileReader.h"
#include "Token.h"

class Tokenizer {
private:
	FileReader reader;
	Token token;
	bool tokenIsNumber;
	std::string tokenText;
	int tokenRow, tokenCol;

	void parseNumber(char c);
	void parseWord(char c);
	void parseOperator(char c);
	void parseSeparator(char c);

public:
	Tokenizer(std::string fileName);
	bool next();
	Token getCurrentToken();
	std::string toString();
};
