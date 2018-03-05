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

std::shared_ptr<Token> Parser::currentToken()
{
	return tokenizer->getCurrentToken();
}

TokenType Parser::currentTokenType()
{
	return tokenizer->getCurrentToken()->type;
}

void Parser::require(std::initializer_list<TokenType> types)
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
		"Required " + res + " but found " + currentToken()->text);
}

void Parser::parse()
{
	programName = "Main";
	parseProgram();
}

void Parser::parseProgram()
{
	goToNextToken();
	if (currentTokenType() == KEYWORD_PROGRAM) {
		goToNextToken();
		require({ VARIABLE });
		programName = currentToken()->text;
		goToNextToken();
		require({ SEP_SEMICOLON });
	}

	parseBlock();
}

void Parser::parseBlock()
{
	declarationPart();
	statementPart();
}

void Parser::declarationPart()
{
	while (true) {
		goToNextToken();

		if (currentTokenType() == KEYWORD_TYPE) {
			goToNextToken();
			require({ VARIABLE });
			//while ()
		}
		else {
			return;
		}
	}
}

void Parser::typeDeclarationPart()
{

}

void Parser::statementPart()
{
}
