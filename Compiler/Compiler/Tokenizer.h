#pragma once
#include <string>
#include <memory>

#include "FileReader.h"
#include "Token.h"

class Tokenizer {
private:
	FileReader reader;
	std::shared_ptr<Token> token;
	
	void parseNumber(char c);
	void parseWord(char c);
	void parseOperator(char c);
	void parseSeparator(char c);

public:
	Tokenizer(std::string fileName);
	bool next();
	std::shared_ptr<Token> getCurrentToken();
	std::shared_ptr<Token> getNextToken();
};
