#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Token.h"
//#include "Types.h" // ALERT circular dependencies, need to move FunctionType to another module?

class SyntaxNode;
typedef std::shared_ptr<SyntaxNode> PSyntaxNode;

class Type;
typedef std::shared_ptr<Type> PType;

class SyntaxNode {
public:
	std::vector<std::shared_ptr<SyntaxNode>> children;
	PToken token;

	SyntaxNode() {}
	SyntaxNode(PToken token, std::initializer_list<PSyntaxNode> children = {});
	void print(std::stringstream &output, std::string prefix = "", bool end = true);
	std::string toString(std::string prefix = "");
};

class VarNode : public SyntaxNode {
public:
	PType type;

	VarNode(PToken token, PType type)
		: SyntaxNode(token), type(type)
	{}
};

class UnaryOpNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class NotNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

//class ExprConst : public Expression {
//	using Expression::Expression;
//};
//
//class ExprOp : public Expression {
//	using Expression::Expression;
//};
//
//class ExprUnaryOp : public ExprOp {
//	using ExprOp::ExprOp;
//};
//
//class ExprBinaryOp : public ExprOp {
//	using ExprOp::ExprOp;
//};
//
//class ExprFunc : public Expression {
//	using Expression::Expression;
//};
