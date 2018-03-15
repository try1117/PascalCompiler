#include "Operation.h"

std::set<TokenType> Operation::logicalTypes = {
	OP_GREATER,
	OP_LESS,
	OP_GREATER_OR_EQUAL,
	OP_LESS_OR_EQUAL,
	OP_EQUAL,
	OP_NOT_EQUAL,
};

std::set<TokenType> Operation::exprTypes = {
	OP_PLUS,
	OP_MINUS,
	KEYWORD_OR,
	KEYWORD_XOR,
};

std::set<TokenType> Operation::termTypes = {
	OP_MULT,
	OP_DIVISION,
	KEYWORD_DIV,
	KEYWORD_MOD,
	KEYWORD_AND,
	KEYWORD_SHL,
	KEYWORD_SHR,
};

std::set<TokenType> Operation::simpleArithmeticTypes = {
	OP_PLUS,
	OP_MINUS,
	OP_MULT,
	OP_DIVISION,
};

std::set<TokenType> Operation::integerOperationTypes = {
	KEYWORD_OR,
	KEYWORD_XOR,
	KEYWORD_AND,
	KEYWORD_DIV,
	KEYWORD_MOD,
	KEYWORD_SHL,
	KEYWORD_SHR,
};

int Operation::evalIntegers(int left, int right, PToken operation)
{
	switch (operation->type) {
		case KEYWORD_OR:  return left | right;
		case KEYWORD_XOR: return left ^ right;
		case KEYWORD_AND: return left & right;
		case KEYWORD_DIV: return left / right;
		case KEYWORD_MOD: return left % right;
		case KEYWORD_SHL: return left << right;
		case KEYWORD_SHR: return left >> right;

		case OP_PLUS:  return left + right;
		case OP_MINUS: return left - right;
		case OP_MULT:  return left * right;
	
		throw std::exception("Illegal operation");
	}
}

double Operation::evalDoubles(double left, double right, PToken operation)
{
	switch (operation->type) {
		case OP_PLUS:		return left + right;
		case OP_MINUS:		return left - right;
		case OP_MULT:		return left * right;
		case OP_DIVISION:	return left / right;

		default: throw std::exception("Illegal operation");
	}
}

std::string Operation::evalStrings(std::string left, std::string right, PToken operation)
{
	if (operation->type == OP_PLUS) {
		return left + right;
	}
	else {
		throw std::exception("Illegal operation");
	}
}
