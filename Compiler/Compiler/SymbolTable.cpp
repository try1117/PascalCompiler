#include <algorithm>
#include "SymbolTable.h"
#include "Utils.h"

const std::vector<std::string> Symbol::categoryName = {
	"",
	"Type",
	"Const",
	"Var",
};

void SymbolTable::checkDuplication(PToken token)
{
	std::string lowText = token->text;
	std::transform(lowText.begin(), lowText.end(), lowText.begin(), ::tolower);
	if (symbolsMap.count(lowText)) {
		throw LexicalException(token->row, token->col, "Duplication found " + token->text);
	}
}

void SymbolTable::addType(PToken token, PType type)
{
	checkDuplication(token);
	PSymbol sym = std::make_shared<Symbol>(token, type, Symbol::Category::TYPE);
	addSymbol(sym);
}

void SymbolTable::addVariable(PToken token, PType type)
{
	checkDuplication(token);
	PSymbol sym = std::make_shared<Symbol>(token, type, Symbol::Category::NIL);
	addSymbol(sym);
}

void SymbolTable::addVariables(std::vector<PToken> tokens, PType type)
{
	for (auto it : tokens) {
		addVariable(it, type);
	}
}

PSymbol SymbolTable::getSymbol(PToken token)
{
	std::string low = lowerString(token->text);
	if (symbolsMap.count(low))
		return symbolsMap[low];
	return nullptr;
}

void SymbolTable::addSymbol(PSymbol symbol)
{
	symbolsArray.push_back(symbol);
	std::string low = lowerString(symbol->token->text);
	symbolsMap[low] = symbol;
}
