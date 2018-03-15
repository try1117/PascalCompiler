#pragma once
#include <set>
#include "Token.h"

class Operation {
public:
	static std::set<TokenType> logicalTypes, exprTypes, termTypes;
	static std::set<TokenType> integerOperationTypes, simpleArithmeticTypes;

	static int evalIntegers(int left, int right, PToken operation);
	static double evalDoubles(double left, double right, PToken operation);
	static std::string evalStrings(std::string left, std::string right, PToken operation);

	template<class T>
	static int evalLogicalOperation(T left, T right, PToken operation);
};

template<class T>
inline int Operation::evalLogicalOperation(T left, T right, PToken operation)
{
	switch (operation->type) {
		case OP_GREATER:			return left > right;
		case OP_LESS:				return left < right;
		case OP_GREATER_OR_EQUAL:	return left >= right;
		case OP_LESS_OR_EQUAL:		return left <= right;
		case OP_EQUAL:				return left == right;
		case OP_NOT_EQUAL:			return left != right;
	}
}
