#include <map>
#include <algorithm>
#include <functional>

#include "Tokenizer.h"
#include "Exceptions.h"
#include "Utils.h"

std::map<std::string, TokenType> operators = {
	{ "+", OP_PLUS },
	{ "-", OP_MINUS },
	{ "*", OP_MULT },
	{ "/", OP_DIVISION },
	{ "@", OP_AT },
	{ "^", OP_CAP },
	{ "=", OP_EQUAL },
	{ ">", OP_GREATER },
	{ "<", OP_LESS },
	{ ":", OP_COLON },
};

std::map<std::string, TokenType> separators = {
	{ "(",  SEP_BRACKET_LEFT },
	{ ")",  SEP_BRACKET_RIGHT },
	{ "[",  SEP_BRACKET_SQUARE_LEFT },
	{ "]",  SEP_BRACKET_SQUARE_RIGHT },
	{ "{",  SEP_BRACKET_FIGURE_LEFT },
	{ "}",  SEP_BRACKET_FIGURE_RIGHT },
	{ ";",  SEP_SEMICOLON },
	{ ",",  SEP_COMMA },
	{ ".",  SEP_DOT },
};

std::map<std::string, TokenType> keywords = {
	{ "integer",   KEYWORD_INTEGER },
	{ "double",    KEYWORD_DOUBLE },
	{ "char",      KEYWORD_CHARACTER },
	{ "var",       KEYWORD_VAR },
	{ "and",       KEYWORD_AND },
	{ "array",     KEYWORD_ARRAY },
	{ "begin",     KEYWORD_BEGIN },
	{ "break",     KEYWORD_BREAK },
	{ "case",      KEYWORD_CASE },
	{ "continue",  KEYWORD_CONTINUE },
	{ "const",     KEYWORD_CONST },
				   
	{ "div",       KEYWORD_DIV },
	{ "do",        KEYWORD_DO },
	{ "downto",    KEYWORD_DOWNTO },
	{ "else",      KEYWORD_ELSE },
	{ "end",       KEYWORD_END },
	{ "exit",      KEYWORD_EXIT },

	{ "file",      KEYWORD_FILE },
	{ "for",       KEYWORD_FOR },
	{ "function",  KEYWORD_FUNCTION },
	{ "end",       KEYWORD_END },
	{ "exit",      KEYWORD_EXIT },
	{ "if",        KEYWORD_IF },
	{ "in",        KEYWORD_IN },
	{ "mod",       KEYWORD_MOD },
	{ "nil",       KEYWORD_NIL },
	{ "not",       KEYWORD_NOT },
	{ "of",        KEYWORD_OF },
	{ "or",        KEYWORD_OR },
	{ "procedure", KEYWORD_PROCEDURE },
	{ "record",    KEYWORD_RECORD },
	{ "repeat",    KEYWORD_REPEAT },
	{ "set",       KEYWORD_SET },
	{ "shl",       KEYWORD_SHL },
	{ "shr",       KEYWORD_SHR },
	{ "string",    KEYWORD_STRING },
	{ "then",      KEYWORD_THEN },
	{ "to",        KEYWORD_TO },
	{ "type",      KEYWORD_TYPE },
	{ "while",     KEYWORD_WHILE },
	{ "until",     KEYWORD_UNTIL },
	{ "with",      KEYWORD_WITH },
	{ "xor",       KEYWORD_XOR },
	{ "goto",      KEYWORD_XOR },
	{ "label",     KEYWORD_LABEL },
	{ "program",   KEYWORD_PROGRAM },
	{ "write",     KEYWORD_WRITE },
	{ "writeln",   KEYWORD_WRITELN },
	{ "read",      KEYWORD_READ },
	{ "readln",    KEYWORD_READLN },
};

Tokenizer::Tokenizer(std::string fileName) :
	reader(fileName), token(nullptr)
{
}

bool wordFirstSymbol(char c)
{
	return (isalpha(c) || c == '_' || c == '\'');
}

bool wordSymbol(char c)
{
	return (isalpha(c) || isdigit(c) || c == '_');
}

bool specialNumberSymbol(char c)
{
	return c == '$' || c == '%' || c == '#';
}

bool Tokenizer::next()
{
	if (token != nullptr && token->type == KEYWORD_EOF) {
		return false;
	}

	token = std::make_shared<Token>(UNDEFINED, reader.getRow(), reader.getCol());
	while (token->type == UNDEFINED) {
		char c = reader.nextSymbol();
		token->row = reader.getRow();
		token->col = reader.getCol();
		token->text = "";
		
		if (c == 0) {
			token->text = "end of file";
			token->type = KEYWORD_EOF;
			return false;
		}

		if (wordFirstSymbol(c)) {
			parseWord(c);
		}
		else if (separators.count(std::string({ c }))) {
			parseSeparator(c);
		}
		else if (operators.count(std::string({ c }))) {
			parseOperator(c);
		}
		else if (specialNumberSymbol(c)) {
			parseSpecialNumber(c);
		}
		else if (isdigit(c)) {
			parseNumber(c);
		}
	}

	return true;
}

std::shared_ptr<Token> Tokenizer::getCurrentToken()
{
	if (token == nullptr)
		next();
	return token;
}

std::shared_ptr<Token> Tokenizer::getNextToken()
{
	next();
	return getCurrentToken();
}

enum NumberState {
	NS_INITIAL,
	NS_BEFORE_DOT,
	NS_BEFORE_EXP,
	NS_BEFORE_SIGN,
	NS_AFTER_SIGN,
	NS_COMPLETE,
};

void Tokenizer::parseSpecialNumber(char c)
{
	token->text = c;
	NumberState state = NS_INITIAL;

	token->type = (c == '#' ? CONST_CHARACTER : CONST_INTEGER);

	std::map<char, std::function<bool(char)>> checkFunctions = {
		std::make_pair('%', [](char c) { return c == '0' || c == '1'; }),
		std::make_pair('$', [](char c) { return isalnum(c); }),
		std::make_pair('#', [](char c) { return isdigit(c); }),
	};

	std::map<char, std::string> errorMessages = {
		std::make_pair('%', "Binary number expected"),
		std::make_pair('$', "Hex number expected"),
		std::make_pair('#', "Decimal number from 0 to 255 expected"),
	};

	std::map<char, int> bases = {
		std::make_pair('%', 2),
		std::make_pair('$', 16),
		std::make_pair('#', 10),
	};
	
	std::function<bool(char)> check = checkFunctions[c];
	std::string errorMessage = errorMessages[c];
	int base = bases[c];

	c = reader.nextSymbol();
	if (!check(c)) {
		throw LexicalException(token->row, token->col, errorMessage);
	}

	while (check(c)) {
		token->text += c;
		if (reader.endOfLine()) {
			break;
		}
		c = reader.nextSymbol();
		if (!isalnum(c)) {
			reader.symbolRollback();
			break;
		}
	}
	token->assignValue(token->text.substr(1, token->text.length()), base);
}

void Tokenizer::parseNumber(char c)
{
	token->text = c;
	NumberState state = NS_INITIAL;
	token->type = CONST_INTEGER;

	while (state != NS_COMPLETE) {
		if (state == NS_INITIAL) {
			state = NS_BEFORE_DOT;
		}
		else if (state == NS_BEFORE_DOT) {
			if (c == '.') {
				state = NS_BEFORE_EXP;
				token->text += c;
				token->type = CONST_DOUBLE;
			}
			else if (c == 'e' || c == 'E') {
				state = NS_BEFORE_SIGN;
				token->text += c;
				token->type = CONST_DOUBLE;
			}
			else if (isdigit(c)) {
				token->text += c;
			}
			else {
				state = NS_COMPLETE;
			}
		}
		else if (state == NS_BEFORE_EXP) {
			if (c == 'e' || c == 'E') {
				state = NS_BEFORE_SIGN;
				token->text += c;
			}
			else if (isdigit(c)) {
				token->text += c;
			}
			// double dot ".." case
			else if (c == '.') {
				token->text.pop_back();
				reader.symbolRollback();
				reader.symbolRollback();
				state = NS_BEFORE_DOT;
				token->type = CONST_INTEGER;
				break;
			}
			else {
				state = NS_COMPLETE;
			}
		}
		else if (state == NS_BEFORE_SIGN) {
			if (c == '+' || c == '-' || isdigit(c)) {
				token->text += c;
				state = NS_AFTER_SIGN;
			}
			else {
				throw LexicalException(reader.getRow(), reader.getCol(), "Number can't end at exp");
			}
		}
		else if (state == NS_AFTER_SIGN) {
			if (isdigit(c)) {
				token->text += c;
			}
			else {
				state = NS_COMPLETE;
			}
		}

		if (state != NS_COMPLETE) {
			if (!reader.endOfLine()) {
				c = reader.nextSymbol();
			}
			else break;
		}
	}

	if (state == NS_COMPLETE) {
		reader.symbolRollback();
	}

	token->assignValue(token->text, 10);
}

void Tokenizer::parseWord(char c)
{
	token->text = "";
	if (c == '\'') {
		while (true) {
			if (reader.endOfLine()) {
				throw LexicalException(token->row, token->col, "Missing terminating ' character");
				return;
			}

			c = reader.nextSymbol();
			
			// end of the string
			// or symbol apostrophe that is inside the string
			if (c == '\'' && reader.endOfLine()) {
				break;
			}
			if (c == '\'' && reader.nextSymbol() != '\'') {
				reader.symbolRollback();
				break;
			}
			token->text += c;
		}

		if (token->text.length() == 1) {
			token->type = CONST_CHARACTER;
			token->value = std::make_shared<IdentifierValue>(token->text[0]);
		}
		else {
			token->type = CONST_STRING;
			token->value = std::make_shared<IdentifierValue>(token->text);
		}
		return;
	}
	
	while (true) {
		token->text += c;
		if (reader.endOfLine()) {
			break;
		}
		c = reader.nextSymbol();
		if (!wordSymbol(c)) {
			reader.symbolRollback();
			break;
		}
	}
	
	std::string low = lowerString(token->text);
	token->type = (keywords.count(low) ? keywords[low] : IDENTIFIER);
}

void Tokenizer::parseSeparator(char c)
{
	token->text = c;
	if (c == '.') {
		if (reader.endOfLine()) {
			token->type = separators["."];
		}
		else {
			c = reader.nextSymbol();
			// double dot '..'
			if (c == '.') {
				token->type = SEP_DOUBLE_DOT;
				token->text = "..";
			}
			else {
				reader.symbolRollback();
				token->type = SEP_DOT;
			}
		}
	}
	// comment using symbols '{' and '}'
	else if (c == '{') {
		while (c != '}') {
			if (reader.endOfFile()) {
				throw LexicalException(token->row, token->col, "Unclosed comment");
			}
			c = reader.nextSymbol();
		}
	}
	else if (c == '(') {
		// not a comment
		if (reader.endOfLine()) {
			token->type = SEP_BRACKET_LEFT;
		}
		else if (reader.nextSymbol() != '*') {
			token->type = SEP_BRACKET_LEFT;
			reader.symbolRollback();
		}
		// comment using symbols '(*' and '*)'
		else {
			while (c != ')') {
				c = reader.nextSymbol();
				if (reader.endOfFile()) {
					throw LexicalException(token->row, token->col, "Unclosed comment");
				}
				if (c == '*') {
					if (reader.nextSymbol() == ')') {
						break;
					}
					else {
						// in case of '(***)'
						reader.symbolRollback();
					}
				}
			}
		}
	}
	else {
		token->type = separators[std::string({ c })];
	}
}

void Tokenizer::parseOperator(char c)
{
	token->text = c;
	if (reader.endOfLine()) {
		token->type = operators[std::string({ c })];
		return;
	}

	char nxt = reader.nextSymbol();
	if (c == ':') {
		if (nxt == '=') {
			token->type = KEYWORD_ASSIGN;
			token->text.push_back(nxt);
		}
		else {
			token->type = OP_COLON;
			reader.symbolRollback();
		}
	}
	else if (c == '<') {
		if (nxt == '>') {
			token->type = OP_NOT_EQUAL;
			token->text.push_back(nxt);
		}
		else if (nxt == '=') {
			token->type = OP_LESS_OR_EQUAL;
			token->text.push_back(nxt);
		}
		else {
			token->type = OP_LESS;
			reader.symbolRollback();
		}
	}
	else if (c == '>') {
		if (nxt == '=') {
			token->type = OP_GREATER_OR_EQUAL;
			token->text.push_back(nxt);
		}
		else {
			token->type = OP_GREATER;
			reader.symbolRollback();
		}
	}
	else if (c == '/') {
		if (nxt == '/') {
			reader.nextLine();
		}
		else {
			token->type = OP_DIVISION;
			reader.symbolRollback();
		}
	}
	else {
		token->type = operators[std::string({ c })];
		reader.symbolRollback();
	}
}
