#pragma once
#include "Tokenizer.h"
#include "SyntaxObject.h"
#include "Types.h"
#include "SymbolTable.h"
#include "Operation.h"

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
	PType getOperationType(PType left, PType right, PToken operation);
	PSyntaxNode cast(PSyntaxNode node, PType operationType);
	PSyntaxNode castConstNode(PSyntaxNode node, PType to);
	PSyntaxNode createOperationNode(PSyntaxNode left, PSyntaxNode right, PToken operation);
	
	PIdentifierValue evalOperation(PSyntaxNode left, PSyntaxNode right, PToken operation, PType operationType);

	void parseProgram();

	void declarationPart();
	void typeDeclarationPart();
	PType parseType();
	PType typeAlias(PToken token);

	PSyntaxNode parseConstValue(std::vector<PToken> identifiers, PType type);
	void variableDeclarationPart();
	std::vector<PToken> identifierList();

	bool instanceOfConstNode(PSyntaxNode node);
	void constDeclarationPart();
	void requireTypesCompatibility(PType left, PType right);
	PSyntaxNode typedConstant(PType type);

	PSyntaxNode compoundStatement();
	PSyntaxNode statementList();
	PSyntaxNode parseStatement();
};
