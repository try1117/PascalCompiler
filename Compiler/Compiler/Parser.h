#pragma once
#include "Tokenizer.h"
#include "SyntaxObject.h"
#include "Types.h"
#include "SymbolTable.h"

class Parser {
public:
	Parser(std::shared_ptr<Tokenizer> tokenizer);
	PType parse();

private:
	std::shared_ptr<Tokenizer> tokenizer;
	std::string programName;
	std::vector<SymbolTable> tables;

	void goToNextToken();
	PToken currentToken();
	TokenType currentTokenType();
	void requireCurrent(std::initializer_list<TokenType> types);
	void requireNext(std::initializer_list<TokenType> types);

	void parseProgram();

	void declarationPart();
	void typeDeclarationPart();
	PType parseType();

	void variableDeclarationPart();
	std::vector<PSyntaxNode> identifierList();

	PSyntaxNode compoundStatement();
	PSyntaxNode statementList();
	PSyntaxNode parseStatement();
};
