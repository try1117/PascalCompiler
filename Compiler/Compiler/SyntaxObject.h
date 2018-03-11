#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Token.h"

class SyntaxNode;
typedef std::shared_ptr<SyntaxNode> PSyntaxNode;

class SyntaxNode {
public:
	std::vector<std::shared_ptr<SyntaxNode>> children;
	std::shared_ptr<Token> token;

	SyntaxNode() {}
	SyntaxNode(std::shared_ptr<Token> token, std::initializer_list<std::shared_ptr<SyntaxNode>> children = {});
	void print(std::stringstream &output, std::string prefix = "", bool end = true);
	std::string toString(std::string prefix = "");
};

class VarNode : public SyntaxNode {
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
