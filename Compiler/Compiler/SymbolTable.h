#pragma once
#include <vector>
//#include "SyntaxObject.h"
#include "Exceptions.h"
#include "Token.h"

class Symbol;
typedef std::shared_ptr<Symbol> PSymbol;

class Type;
typedef std::shared_ptr<Type> PType;

class Symbol {
public:
	enum Category {
		NIL,
		TYPE,
		CONST,
		VAR_PARAMETER,
	};

	//PSyntaxNode node;
	PToken token;
	PType type;
	Category category;

	static const std::vector<std::string> categoryName;

	//Symbol(PSyntaxNode node, PType type, Category category)
	//	: node(node), type(type), category(category)
	//{}
	Symbol(PToken token, PType type, Category category)
		: token(token), type(type), category(category)
	{}
};

class SymbolTable;
typedef std::shared_ptr<SymbolTable> PSymbolTable;

class SymbolTable {
public:	
	std::vector<PSymbol> symbolsArray;
	std::map<std::string, PSymbol> symbolsMap;

	void checkDuplication(PToken token);
	void addType(PToken token, PType type);
	//void addVariable(PSyntaxNode identifier, PType type);
	//void addVariables(std::vector<PSyntaxNode> identifiers, PType type);
	
	void addVariable(PToken token, PType type);
	void addVariables(std::vector<PToken> tokens, PType type);


	PSymbol getSymbol(PToken token);

private:
	void addSymbol(PSymbol symbol);
};
