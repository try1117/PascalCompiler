#include <map>
#include <algorithm>
#include "Tokenizer.h"

std::map<std::string, Token> operators = {
	{ "+", OP_PLUS },
	{ "-", OP_MINUS },
	{ "*", OP_MULT },
	{ "/", OP_DIVISION },
	{ "@", OP_AT },
	{ "#", OP_LATTICE },
	{ "$", OP_DOLLAR },
	{ "^", OP_CAP },
	{ "=", OP_EQUAL },
	{ ">", OP_GREATER },
	{ "<", OP_LESS },
	{ ":", OP_COLON },
};

std::map<std::string, Token> separators = {
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

std::map<std::string, Token> keywords = {
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

LexicalException::LexicalException(int row, int col, std::string mes)
{
	sprintf(message, "Lexical exception in row : %d column : %d - %s", row, col, mes.c_str());
}

const char * LexicalException::what() const
{
	return message;
}

Tokenizer::Tokenizer(std::string fileName) :
	reader(fileName), token(UNDEFINED)
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

bool Tokenizer::next()
{
	token = UNDEFINED;
	while (token == UNDEFINED) {
		tokenValue = "";
		char c = reader.nextSymbol();
		if (c == 0)
			return false;

		if (wordFirstSymbol(c)) {
			parseWord(c);
		}
		else if (separators.count(std::string({ c }))) {
			parseSeparator(c);
		}
		else if (operators.count(std::string({ c }))) {
			parseOperator(c);
		}
		else if (isdigit(c)) {
			parseNumber(c);
		}
	}

	return true;
}

Token Tokenizer::getCurrentToken()
{
	return token;
}

std::string stringPadding(int len, std::string s) {
	return s + std::string(std::max<int>(0, len - s.length()), ' ');
}

std::string Tokenizer::toString()
{
	std::string srow = std::to_string(tokenRow);
	std::string scol = std::to_string(tokenCol);
	std::string res =
		stringPadding(3, srow) + "| " +
		stringPadding(3, scol) + "| " +
		stringPadding(25, TokenName[token]) + "| " +
		stringPadding(25, tokenText) + "| " +
		tokenValue;

	return res;
}

enum NumberState {
	NS_INITIAL,
	NS_BEFORE_DOT,
	NS_BEFORE_EXP,
	NS_BEFORE_SIGN,
	NS_AFTER_SIGN,
	NS_COMPLETE,
};

std::string Tokenizer::transformNumber(Token token, std::string text)
{
	try {
		if (token == CONST_HEX) {
			return std::to_string(std::stoll(text, 0, 16));
		}
		else if (token == CONST_INTEGER) {
			return std::to_string(std::stoll(text, 0, 10));
		}
		else {
			char number[40];
			sprintf(number, "%.15lf", std::stod(text));
			return number;
		}
	}
	catch (std::out_of_range e) {
		throw LexicalException(tokenRow, tokenCol, "Number is too big");
	}
	catch (std::exception e) {
		throw LexicalException(tokenRow, tokenCol, e.what());
	}
}

void Tokenizer::parseNumber(char c)
{
	tokenRow = reader.getRow();
	tokenCol = reader.getCol();

	tokenText = c;
	NumberState state = NS_INITIAL;
	bool isInteger = true;
	
	while (state != NS_COMPLETE) {
		if (state == NS_INITIAL) {
			state = NS_BEFORE_DOT;
		}
		else if (state == NS_BEFORE_DOT) {
			if (c == '.') {
				state = NS_BEFORE_EXP;
				tokenText.push_back(c);
				isInteger = false;
			}
			else if (c == 'e' || c == 'E') {
				state = NS_BEFORE_SIGN;
				tokenText.push_back(c);
				isInteger = false;
			}
			else if (isdigit(c)) {
				tokenText.push_back(c);
			}
			else {
				state = NS_COMPLETE;
			}
		}
		else if (state == NS_BEFORE_EXP) {
			if (c == 'e' || c == 'E') {
				state = NS_BEFORE_SIGN;
				tokenText.push_back(c);
			}
			else if (isdigit(c)) {
				tokenText.push_back(c);
			}
			// double dot ".." case
			else if (c == '.') {
				tokenText.pop_back();
				reader.symbolRollback();
				state = NS_BEFORE_DOT;
				break;
			}
			else {
				state = NS_COMPLETE;
			}
		}
		else if (state == NS_BEFORE_SIGN) {
			if (c == '+' || c == '-' || isdigit(c)) {
				tokenText.push_back(c);
				state = NS_AFTER_SIGN;
			}
			else {
				throw LexicalException(reader.getRow(), reader.getCol(), "Number can't end at exp");
			}
		}
		else if (state == NS_AFTER_SIGN) {
			if (isdigit(c)) {
				tokenText.push_back(c);
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

	if (isInteger) {
		token = CONST_INTEGER;
	}
	else {
		token = CONST_DOUBLE;
	}

	tokenValue = transformNumber(token, tokenText);
}

void Tokenizer::parseWord(char c)
{
	tokenRow = reader.getRow();
	tokenCol = reader.getCol();

	tokenText = "";
	if (c == '\'') {
		while (true) {
			if (reader.endOfLine()) {
				throw LexicalException(tokenRow, tokenCol, "Missing terminating ' character");
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
			tokenText.push_back(c);
		}
		token = CONST_STRING;
		return;
	}
	
	while (true) {
		tokenText.push_back(c);
		if (reader.endOfLine()) {
			break;
		}
		c = reader.nextSymbol();
		if (!wordSymbol(c)) {
			reader.symbolRollback();
			break;
		}
	}
	
	std::string low = tokenText;
	std::transform(low.begin(), low.end(), low.begin(), ::tolower);

	if (keywords.count(low)) {
		token = keywords[low];
	}
	else {
		token = VARIABLE;
	}
}

void Tokenizer::parseSeparator(char c)
{
	tokenRow = reader.getRow();
	tokenCol = reader.getCol();

	tokenText = c;
	if (c == '.') {
		if (reader.endOfLine()) {
			token = separators["."];
		}
		else {
			c = reader.nextSymbol();
			// double dot '..'
			if (c == '.') {
				token = SEP_DOUBLE_DOT;
				tokenText = "..";
			}
			else {
				reader.symbolRollback();
				token = separators["."];
			}
		}
	}
	// comment using symbols '{' and '}'
	else if (c == '{') {
		while (c != '}') {
			if (reader.endOfFile()) {
				throw LexicalException(tokenRow, tokenCol, "Unclosed comment");
			}
			c = reader.nextSymbol();
		}
	}
	else if (c == '(') {
		// not a comment
		if (reader.endOfLine()) {
			token = separators["("];
		}
		else if (reader.nextSymbol() != '*') {
			token = separators["("];
			reader.symbolRollback();
		}
		// comment using symbols '(*' and '*)'
		else {
			while (c != ')') {
				c = reader.nextSymbol();
				if (reader.endOfFile()) {
					throw LexicalException(tokenRow, tokenCol, "Unclosed comment");
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
		token = separators[std::string({ c })];
	}
}

void Tokenizer::parseOperator(char c)
{
	tokenRow = reader.getRow();
	tokenCol = reader.getCol();

	tokenText = c;
	if (reader.endOfLine()) {
		token = operators[std::string({ c })];
		return;
	}

	char nxt = reader.nextSymbol();
	if (c == ':') {
		if (nxt == '=') {
			token = OP_EQUAL;
			tokenText.push_back(nxt);
		}
		else {
			token = OP_COLON;
			reader.symbolRollback();
		}
	}
	else if (c == '<') {
		if (nxt == '>') {
			token = OP_NOT_EQUAL;
			tokenText.push_back(nxt);
		}
		else if (nxt == '=') {
			token = OP_LESS_OR_EQUAL;
			tokenText.push_back(nxt);
		}
		else {
			token = OP_LESS;
			reader.symbolRollback();
		}
	}
	else if (c == '>') {
		if (nxt == '=') {
			token = OP_GREATER_OR_EQUAL;
			tokenText.push_back(nxt);
		}
		else {
			token = OP_GREATER;
			reader.symbolRollback();
		}
	}
	else if (c == '/') {
		if (nxt == '/') {
			reader.nextLine();
		}
		else {
			token = OP_DIVISION;
			reader.symbolRollback();
		}
	}
	// hex number
	else if (c == '$') {
		c = nxt;
		tokenText = "";

		if (!isalnum(nxt)) {
			throw LexicalException(tokenRow, tokenCol, "Hex number excepted");
		}

		while (isalnum(c)) {
			tokenText.push_back(c);
			if (reader.endOfLine()) {
				break;
			}
			c = reader.nextSymbol();
			if (!isalnum(c)) {
				reader.symbolRollback();
				break;
			}
		}
		token = CONST_HEX;
		tokenValue = transformNumber(token, tokenText);
	}
	else {
		token = operators[std::string({ c })];
		reader.symbolRollback();
	}
}
