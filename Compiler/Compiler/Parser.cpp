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

PType Parser::parse()
{
	parseProgram();
	declarationPart();
	std::shared_ptr<FunctionType> res(new FunctionType(Type::getSimpleType(Type::Category::NIL), compoundStatement(), programName));
	requireNext({ SEP_DOT });
	return res;
}

void Parser::parseProgram()
{
	requireNext({ KEYWORD_PROGRAM });
	requireNext({ VARIABLE });
	programName = currentToken()->text;
	requireNext({ SEP_SEMICOLON });
}

void Parser::declarationPart()
{
	while (true) {
		goToNextToken();
		if (currentTokenType() == KEYWORD_TYPE) {
			typeDeclarationPart();
		}
		else if (currentTokenType() == KEYWORD_VAR) {
			variableDeclarationPart();
		}
		else {
			return;
		}
	}
}

void Parser::typeDeclarationPart()
{
	goToNextToken();
	do {
		requireCurrent({ VARIABLE });
		auto identifier = currentToken();
		requireNext({ OP_EQUAL });
		PType type = parseType();
		// TODO: push type to the current symtable
	} while (currentTokenType() == VARIABLE);
	
	requireCurrent({ SEP_SEMICOLON });
}

PType Parser::parseType()
{
	goToNextToken();

	std::map<TokenType, Type::Category> simpleCategories = {
		{ TokenType::KEYWORD_INTEGER, Type::Category::INTEGER },
		{ TokenType::KEYWORD_DOUBLE, Type::Category::DOUBLE },
		{ TokenType::KEYWORD_CHARACTER, Type::Category::CHAR },
		{ TokenType::KEYWORD_STRING, Type::Category::STRING },
	};

	if (simpleCategories.count(currentTokenType())) {
		return Type::getSimpleType(simpleCategories[currentTokenType()]);
	}
	else {
		throw LexicalException(currentToken()->row, currentToken()->col, "Expected type but found " + currentToken()->text);
	}
}

void Parser::variableDeclarationPart()
{
	goToNextToken();
	do {
		requireCurrent({ VARIABLE });
		auto identifiers = identifierList();
		PType type = parseType();
		// TODO: initialization
		tables.back().addVariables(identifiers, type);
	} while (currentTokenType() == VARIABLE);

	requireCurrent({ SEP_SEMICOLON });
}

std::vector<PSyntaxNode> Parser::identifierList()
{
	std::vector<PSyntaxNode> res;
	while (true) {
		res.push_back(std::make_shared<VarNode>(currentToken()));
		requireNext({ SEP_COMMA, OP_COLON });
		if (currentTokenType() == OP_COLON) {
			break;
		}
		goToNextToken();
	}
	return res;
}

PSyntaxNode Parser::compoundStatement()
{
	requireCurrent({ KEYWORD_BEGIN });
	PSyntaxNode statement = statementList();
	requireCurrent({ KEYWORD_END });
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
