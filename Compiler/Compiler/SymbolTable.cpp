#include <algorithm>
#include "SymbolTable.h"
#include "Utils.h"

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

}

void SymbolTable::addVariable(PSyntaxNode identifier, PType type)
{
	checkDuplication(identifier->token);
	PSymbol sym = std::make_shared<Symbol>(identifier, type);
	symbolsArray.push_back(sym);
	std::string low = lowerString(identifier->token->text);
	symbolsMap[low] = sym;
}

void SymbolTable::addVariables(std::vector<PSyntaxNode> identifiers, PType type)
{
	for (auto it : identifiers) {
		addVariable(it, type);
	}
}
