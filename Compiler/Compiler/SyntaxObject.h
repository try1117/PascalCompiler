#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <set>

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

	enum Category {
		NIL,
		CONST_NODE,
	} category;

	SyntaxNode() {}
	SyntaxNode(PToken token, PType type, std::initializer_list<PSyntaxNode> children = {}, Category category = NIL);
	void print(std::string &output, std::string prefix = "", bool end = true);
	std::string toString(std::string prefix = "");
};

class VarNode : public SyntaxNode {
public:
	using SyntaxNode::SyntaxNode;
};

class UnaryMinusNode : public SyntaxNode {
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
		: SyntaxNode(token, type, std::initializer_list<PSyntaxNode>(), CONST_NODE), value(value)
	{
		token->text = value->toString();
	}
};

typedef std::shared_ptr<ConstNode> PConstNode;

class TypedConstNode : public SyntaxNode {
public:
	PType type;
	TypedConstNode(PType type, std::string name)
		: SyntaxNode(std::make_shared<Token>(IDENTIFIER, 0, 0, name), type, std::initializer_list<PSyntaxNode>(), CONST_NODE)
	{}
};

class CastNode : public SyntaxNode {
public:
	PType newType;
	CastNode(PSyntaxNode node, PType newType)
		: SyntaxNode(node->token, node->type, std::initializer_list<PSyntaxNode>({ node })), newType(newType)
	{}
};

class IndexNode : public SyntaxNode {
public:
	IndexNode(PType type, std::initializer_list<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(SEP_BRACKET_SQUARE_LEFT, 0, 0, "[]"), type, children)
	{}
};

class FieldAccessNode : public SyntaxNode {
public:
	FieldAccessNode(PType type, std::initializer_list<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(SEP_DOT, 0, 0, "."), type, children)
	{}
};

class AssignStatement : public SyntaxNode {
public:
	AssignStatement(PType type, std::initializer_list<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_ASSIGN, 0, 0, ":="), type, children)
	{}
};

class IfStatement : public SyntaxNode {
public:
	PSyntaxNode condition, ifPart, elsePart;
	IfStatement(PSyntaxNode condition, PSyntaxNode ifPart, PSyntaxNode elsePart, PType type)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_IF, 0, 0, "If"), type),
		condition(condition), ifPart(ifPart), elsePart(elsePart)
	{
		children.push_back(condition);
		children.push_back(ifPart);
		if (elsePart != nullptr) {
			children.push_back(elsePart);
		}
	}
};
