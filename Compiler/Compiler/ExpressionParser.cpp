#include "ExpressionParser.h"
#include "Exceptions.h"

ExpressionParser::ExpressionParser(std::shared_ptr<Tokenizer> tokenizer)
	: tokenizer(tokenizer)
{
}

std::shared_ptr<Expression> ExpressionParser::parse()
{
	return parseExpr();
}

std::shared_ptr<Expression> ExpressionParser::parseExpr()
{
	auto node = parseTerm();
	auto token = tokenizer->getCurrentToken();

	while (isExpr(token)) {
		tokenizer->next();
		node = std::make_shared<ExprBinaryOp>(token, std::initializer_list<std::shared_ptr<SyntaxNode>>({ node, parseTerm() }));
		token = tokenizer->getCurrentToken();
	}

	return node;
}

std::shared_ptr<Expression> ExpressionParser::parseTerm()
{
	auto node = parseFactor();
	auto token = tokenizer->getCurrentToken();

	while (isTerm(token)) {
		tokenizer->next();
		node = std::make_shared<ExprBinaryOp>(token, std::initializer_list<std::shared_ptr<SyntaxNode>>({ node, parseFactor() }));
		token = tokenizer->getCurrentToken();
	}

	return node;
}

std::shared_ptr<Expression> ExpressionParser::parseFactor()
{
	auto token = tokenizer->getCurrentToken();
	tokenizer->next();

	if (token->type == SEP_BRACKET_LEFT) {
		auto node = parseExpr();
		if (tokenizer->getCurrentToken()->type != SEP_BRACKET_RIGHT) {
			throw SyntaxException(token->row, token->col, "Unclosed brackets");
		}
		tokenizer->next();
		return node;
	}
	else if (isVar(token)) {
		return std::make_shared<ExprVar>(token);
	}
	else if (token->type == OP_MINUS || token->type == OP_PLUS) {
		return std::make_shared<ExprUnaryOp>(token, std::initializer_list<std::shared_ptr<SyntaxNode>>({ parseFactor() }));
	}
	else {
		throw SyntaxException(token->row, token->col, "Expected identifier, constant or expression");
	}
}

bool ExpressionParser::isVar(std::shared_ptr<Token> token)
{
	return
		token->type == VARIABLE ||
		token->type == CONST_DOUBLE ||
		token->type == CONST_INTEGER;
}

bool ExpressionParser::isExpr(std::shared_ptr<Token> token)
{
	return token->type == OP_PLUS || token->type == OP_MINUS;
}

bool ExpressionParser::isTerm(std::shared_ptr<Token> token)
{
	return token->type == OP_MULT || token->type == OP_DIVISION;
}
