#include <functional>
#include "Parser.h"
#include "Exceptions.h"

Parser::Parser(std::shared_ptr<Tokenizer> tokenizer)
	: tokenizer(tokenizer)
{
}

void Parser::goToNextToken()
{
	tokenizer->next();
}

PToken Parser::currentToken()
{
	return tokenizer->getCurrentToken();
}

TokenType Parser::currentTokenType()
{
	return tokenizer->getCurrentToken()->type;
}

void Parser::requireCurrent(std::initializer_list<TokenType> types)
{
	std::string res;
	for (auto type : types) {
		if (currentTokenType() == type)
			return;
		if (!res.empty()) {
			res += " or ";
		}
		res += TokenName[type];
	}

	throw SyntaxException(currentToken()->row, currentToken()->col,
		"Expected " + res + " but found " + currentToken()->text);
}

void Parser::requireNext(std::initializer_list<TokenType> types)
{
	goToNextToken();
	requireCurrent(types);
}

std::set<TokenType> Parser::logicalTypes = {
	OP_GREATER,
	OP_LESS,
	OP_GREATER_OR_EQUAL,
	OP_LESS_OR_EQUAL,
	OP_EQUAL,
	OP_NOT_EQUAL,
};

std::set<TokenType> Parser::exprTypes = {
	OP_PLUS,
	OP_MINUS,
	KEYWORD_OR,
	KEYWORD_XOR,
};

std::set<TokenType> Parser::termTypes = {
	OP_MULT,
	OP_DIVISION,
	KEYWORD_DIV,
	KEYWORD_MOD,
	KEYWORD_AND,
	KEYWORD_SHL,
	KEYWORD_SHR,
};

PSyntaxNode Parser::parseLogical()
{
	auto node = parseExpr();
	auto token = currentToken();

	while (logicalTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseExpr(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseExpr()
{
	auto node = parseTerm();
	auto token = currentToken();

	while (exprTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseTerm(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseTerm()
{
	auto node = parseFactor();
	auto token = currentToken();

	while (termTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseFactor(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseFactor()
{
	auto token = currentToken();
	goToNextToken();

	if (token->type == SEP_BRACKET_LEFT) {
		auto node = parseLogical();
		requireCurrent({ SEP_BRACKET_RIGHT });
		goToNextToken();
		return node;
	}
	else if (token->type == OP_MINUS || token->type == OP_PLUS) {
		return std::make_shared<UnaryOpNode>(token, std::initializer_list<PSyntaxNode>({ parseFactor() }));
	}
	else if (token->type == KEYWORD_NOT) {
		return std::make_shared<NotNode>(token, std::initializer_list<PSyntaxNode>({ parseFactor() }));
	}
	else if (token->type == VARIABLE) {
		return std::make_shared<VarNode>(token, getSymbol(token)->type);
	}
	else {
		throw SyntaxException(token->row, token->col, "Expected identifier, constant or expression");
	}
}

PSymbol Parser::getSymbol(PToken token)
{
	for (int i = tables.size() - 1; i >= 0; --i) {
		if (tables[i]->getSymbol(token)) {
			return tables[i]->getSymbol(token);
		}
	}
	throw LexicalException(token->row, token->col, "Identifier " + token->text + " not found");
}

PSyntaxNode Parser::createOperationNode(PSyntaxNode left, PSyntaxNode right, PToken operation)
{
	return PSyntaxNode();
}

PType Parser::parse()
{
	tables.push_back(std::make_shared<SymbolTable>());
	parseProgram();
	declarationPart();
	std::shared_ptr<FunctionType> res(new FunctionType(tables.back(), Type::getSimpleType(Type::Category::NIL), compoundStatement(), programName));
	requireCurrent({ SEP_DOT });
	return res;
}

void Parser::parseProgram()
{
	requireNext({ KEYWORD_PROGRAM });
	requireNext({ VARIABLE });
	programName = currentToken()->text;
	requireNext({ SEP_SEMICOLON });
	goToNextToken();
}

void Parser::declarationPart()
{
	while (true) {
		if (currentTokenType() == KEYWORD_TYPE) {
			goToNextToken();
			typeDeclarationPart();
		}
		else if (currentTokenType() == KEYWORD_VAR) {
			goToNextToken();
			variableDeclarationPart();
		}
		else if (currentTokenType() == KEYWORD_CONST) {
			goToNextToken();
			constDeclarationPart();
		}
		else {
			return;
		}
	}
}

void Parser::typeDeclarationPart()
{
	do {
		requireCurrent({ VARIABLE });
		auto identifier = currentToken();
		requireNext({ OP_EQUAL });
		goToNextToken();
		PType type = parseType();
		tables.back()->addType(identifier, type);
		requireCurrent({ SEP_SEMICOLON });
		goToNextToken();
	} while (currentTokenType() == VARIABLE);
}

PType Parser::parseType()
{
	std::map<TokenType, Type::Category> simpleCategories = {
		{ TokenType::KEYWORD_INTEGER, Type::Category::INTEGER },
		{ TokenType::KEYWORD_DOUBLE, Type::Category::DOUBLE },
		{ TokenType::KEYWORD_CHARACTER, Type::Category::CHAR },
		{ TokenType::KEYWORD_STRING, Type::Category::STRING },
	};

	PToken token = currentToken();
	goToNextToken();

	if (token->type == VARIABLE) {
		return typeAlias(token);
	}
	else if (simpleCategories.count(token->type)) {
		return Type::getSimpleType(simpleCategories[token->type]);
	}
	else {
		throw LexicalException(token->row, token->col, "Expected type but found " + token->text);
	}
}

PType Parser::typeAlias(PToken token)
{
	return getSymbol(token)->type;
}

void Parser::variableDeclarationPart()
{
	do {
		requireCurrent({ VARIABLE });
		auto identifiers = identifierList();
		PType type = parseType();
		// TODO: initialization
		tables.back()->addVariables(identifiers, type);
		requireCurrent({ SEP_SEMICOLON });
		goToNextToken();
	} while (currentTokenType() == VARIABLE);
}

std::vector<PToken> Parser::identifierList()
{
	std::vector<PToken> res;
	while (true) {
		//res.push_back(std::make_shared<VarNode>currentToken(), nullptr));
		res.push_back(currentToken());
		requireNext({ SEP_COMMA, OP_COLON });
		if (currentTokenType() == OP_COLON) {
			goToNextToken();
			break;
		}
		goToNextToken();
	}
	return res;
}

void Parser::constDeclarationPart()
{
	requireNext({ VARIABLE });
	PToken identifier = currentToken();
	goToNextToken();

	// untyped constant
	if (currentTokenType() == OP_EQUAL) {
		PSyntaxNode expr = parseExpr();
	}
}

PSyntaxNode Parser::compoundStatement()
{
	requireCurrent({ KEYWORD_BEGIN });
	PSyntaxNode statement = statementList();
	requireCurrent({ KEYWORD_END });
	goToNextToken();
	return statement;
}

PSyntaxNode Parser::statementList()
{
	PSyntaxNode statement = std::make_shared<SyntaxNode>();// BodyFunction();
	do {
		goToNextToken();
		PSyntaxNode node = parseStatement();
		if (node != nullptr)
			statement->children.push_back(node);
	} while (currentTokenType() == SEP_SEMICOLON);
	return statement;
}

PSyntaxNode Parser::parseStatement()
{
	switch (currentTokenType()) {
		case VARIABLE:
			return nullptr;
		case KEYWORD_BEGIN:
			return compoundStatement();
		case SEP_SEMICOLON:
		case KEYWORD_END:
			return nullptr;
		default:
			throw LexicalException(currentToken()->row, currentToken()->col, "Unexpected token " + currentToken()->text);
	}
}
