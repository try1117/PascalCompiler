#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>

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
	PType type;

	SyntaxNode() {}
	SyntaxNode(PToken token, PType type, std::initializer_list<PSyntaxNode> children = {});
	void print(std::string &output, std::string prefix = "", bool end = true);
	std::string toString(std::string prefix = "");
};

class VarNode : public SyntaxNode {
public:
	using SyntaxNode::SyntaxNode;
};

class UnaryOpNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class BinaryOpNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class NotNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class ConstNode : public SyntaxNode {
public:
	PIdentifierValue value;
	ConstNode(PToken token, PType type, PIdentifierValue value)
		: SyntaxNode(token, type), value(value)
	{
		token->text = value->toString();
	}
};

class TypedConstNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class CastNode : public SyntaxNode {
public:
	PType newType;
	CastNode(PSyntaxNode node, PType newType)
		: SyntaxNode(node->token, node->type, std::initializer_list<PSyntaxNode>({ node })), newType(newType)
	{}
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
