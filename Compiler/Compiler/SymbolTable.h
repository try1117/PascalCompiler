#pragma once
#include "Types.h"
#include "SyntaxObject.h"
#include "Exceptions.h"

class Symbol;
typedef std::shared_ptr<Symbol> PSymbol;

class Symbol {
	PSyntaxNode node;
	PType type;

	Symbol(PSyntaxNode node, PType type)
		: node(node), type(type)
	{}
};

class SymbolTable {
public:	
	void checkDuplication(PToken token);
	void addType(PToken token, PType type);
	void addVariable(PSyntaxNode identifier, PType type);
	void addVariables(std::vector<PSyntaxNode> identifiers, PType type);

private:
	std::vector<PSymbol> symbolsArray;
	std::map<std::string, PSymbol> symbolsMap;
};
