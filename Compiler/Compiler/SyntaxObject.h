#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include "Token.h"
#include "Generator.h"

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
		VAR_NODE,
	} category;

	SyntaxNode() {}
	SyntaxNode(PToken token, PType type, std::vector<PSyntaxNode> children = {}, Category category = NIL);
	void print(std::string &output, std::string prefix = "", bool end = true);
	std::string toString(std::string prefix = "");
	virtual void toAsmCode(AsmCode &code);
};

class VarNode : public SyntaxNode {
public:
	VarNode(PToken token, PType type, std::vector<PSyntaxNode> children = {}, Category category = VAR_NODE)
		: SyntaxNode(token, type, children, category)
	{}
	void toAsmCode(AsmCode &code) override;
};

class UnaryMinusNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class BinaryOpNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;

	void toAsmCode(AsmCode &code) override;
};

class NotNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class ConstNode : public SyntaxNode {
public:
	PIdentifierValue value;
	ConstNode(PToken token, PType type, PIdentifierValue value)
		: SyntaxNode(token, type, std::vector<PSyntaxNode>(), CONST_NODE), value(value)
	{
		if (value != nullptr)
			token->text = value->toString();
	}

	void toAsmCode(AsmCode &code) override;
};

typedef std::shared_ptr<ConstNode> PConstNode;

class TypedConstNode : public SyntaxNode {
public:
	PType type;
	TypedConstNode(PType type, std::string name)
		: SyntaxNode(std::make_shared<Token>(IDENTIFIER, 0, 0, name), type, std::vector<PSyntaxNode>(), CONST_NODE)
	{}
};

class CastNode : public SyntaxNode {
public:
	PType newType;
	CastNode(PSyntaxNode node, PType newType, std::string typeName)
		: SyntaxNode(std::make_shared<Token>(UNDEFINED, node->token->row, node->token->col, typeName),
			newType, std::vector<PSyntaxNode>({ node })), newType(newType)
	{}
};

class IndexNode : public SyntaxNode {
public:
	PToken variableToken;
	IndexNode(PType type, std::vector<PSyntaxNode> children, PToken variableToken)
		: SyntaxNode(std::make_shared<Token>(SEP_BRACKET_SQUARE_LEFT, 0, 0, "[]"), type, children, VAR_NODE), variableToken(variableToken)
	{}
};

class FieldAccessNode : public SyntaxNode {
public:
	PToken variableToken;
	FieldAccessNode(PType type, std::vector<PSyntaxNode> children, PToken variableToken)
		: SyntaxNode(std::make_shared<Token>(SEP_DOT, 0, 0, "."), type, children, VAR_NODE), variableToken(variableToken)
	{}
};

class AssignStatement : public SyntaxNode {
public:
	AssignStatement(PType type, std::vector<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_ASSIGN, 0, 0, ":="), type, children)
	{}

	void toAsmCode(AsmCode &code) override;
};

class IfStatement : public SyntaxNode {
public:
	PSyntaxNode condition, ifPart, elsePart;
	IfStatement(PToken token, PType type, PSyntaxNode condition, PSyntaxNode ifPart, PSyntaxNode elsePart)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_IF, token->row, token->col, "If"), type),
		condition(condition), ifPart(ifPart), elsePart(elsePart)
	{
		children.push_back(condition);
		if (ifPart != nullptr) children.push_back(ifPart);
		if (elsePart != nullptr) children.push_back(elsePart);
	}

	void toAsmCode(AsmCode &code) override;
};

class WhileNode : public SyntaxNode {
public:
	PSyntaxNode condition, body;
	WhileNode(PToken token, PType type, PSyntaxNode condition, PSyntaxNode body)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_WHILE, token->row, token->col, "While"), type,
			std::vector<PSyntaxNode>({ condition })), condition(condition), body(body)
	{
		if (body != nullptr) children.push_back(body);
	}

	void toAsmCode(AsmCode &code) override;
};

class ForNode : public SyntaxNode {
public:
	PSyntaxNode counter, from, to, body;
	bool downTo;
	ForNode(PToken token, PType type, PSyntaxNode counter, PSyntaxNode from, PSyntaxNode to, bool downTo, PSyntaxNode body)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_FOR, token->row, token->col, "For"), type,
			std::vector<PSyntaxNode>({ counter, from, to })),
		counter(counter), from(from), to(to), downTo(downTo), body(body)
	{
		if (body != nullptr) children.push_back(body);
	}

	void toAsmCode(AsmCode &code) override;
};

class ContinueNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class BreakNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class ExitNode : public SyntaxNode {
	using SyntaxNode::SyntaxNode;
};

class ReadNode : public SyntaxNode {
public:
	ReadNode(PToken token, PType type, std::vector<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_READ, token->row, token->col, "Read"), type, children)
	{}
};

class WriteNode : public SyntaxNode {
public:
	WriteNode(PToken token, PType type, std::vector<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(KEYWORD_WRITE, token->row, token->col, "Write"), type, children)
	{}
	void toAsmCode(AsmCode &code) override;
};

class FunctionCallNode : public SyntaxNode {
public:
	FunctionCallNode(PToken token, PType type, std::vector<PSyntaxNode> children)
		: SyntaxNode(std::make_shared<Token>(token->type, token->row, token->col, "Call " + token->text), type, children, VAR_NODE)
	{}
};
