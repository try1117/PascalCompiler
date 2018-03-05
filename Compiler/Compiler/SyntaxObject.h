#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>

#include "Token.h"

class SyntaxObject {
public:
	std::vector<std::shared_ptr<SyntaxObject>> children;
	std::shared_ptr<Token> token;

	SyntaxObject(std::shared_ptr<Token> token, std::initializer_list<std::shared_ptr<SyntaxObject>> children = {});
	void print(std::ofstream &output, std::string prefix = "", bool end = true);
};

class Expression : public SyntaxObject {
	using SyntaxObject::SyntaxObject;
};

class ExprVar : public Expression {
	using Expression::Expression;
};

class ExprConst : public Expression {
	using Expression::Expression;
};

class ExprOp : public Expression {
	using Expression::Expression;
};

class ExprUnaryOp : public ExprOp {
	using ExprOp::ExprOp;
};

class ExprBinaryOp : public ExprOp {
	using ExprOp::ExprOp;
};

class ExprFunc : public Expression {
	using Expression::Expression;
};
