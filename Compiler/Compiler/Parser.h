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
	std::vector<PSymbolTable> tables;

	void goToNextToken();
	PToken currentToken();
	TokenType currentTokenType();
	void requireCurrent(std::initializer_list<TokenType> types);
	void requireNext(std::initializer_list<TokenType> types);

	PSyntaxNode parseLogical();
	PSyntaxNode parseExpr();
	PSyntaxNode parseTerm();
	PSyntaxNode parseFactor();

	PSymbol getSymbol(PToken token);
	PSyntaxNode createOperationNode(PSyntaxNode left, PSyntaxNode right, PToken operation);

	static std::set<TokenType> logicalTypes, exprTypes, termTypes;

	void parseProgram();

	void declarationPart();
	void typeDeclarationPart();
	PType parseType();
	PType typeAlias(PToken token);

	void variableDeclarationPart();
	std::vector<PToken> identifierList();

	void constDeclarationPart();

	PSyntaxNode compoundStatement();
	PSyntaxNode statementList();
	PSyntaxNode parseStatement();
};
