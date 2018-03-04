#pragma once
#include "Tokenizer.h"
#include "SyntaxObject.h"

class Parser {
public:
	Parser(std::shared_ptr<Tokenizer> tokenizer);
	void parse();

private:
	std::shared_ptr<Tokenizer> tokenizer;
	std::string programName;

	void goToNextToken();
	std::shared_ptr<Token> currentToken();
	TokenType currentTokenType();
	void require(std::initializer_list<TokenType> types);

	void parseProgram();
	void parseBlock();
};
