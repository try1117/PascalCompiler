#include <algorithm>
#include "Token.h"
#include "Exceptions.h"

#include <memory>

Token::Token(TokenType type, int row, int col, std::string text)
	: type(type), row(row), col(col), text(text), stringAllocated(false)
{
}

Token::~Token()
{
	if (stringAllocated) {
		delete value.string;
	}
}

std::string stringPadding(int len, std::string s) {
	return s + std::string(std::max<int>(0, len - s.length()), ' ');
}

std::string Token::toString()
{
	std::string res =
		stringPadding(3, std::to_string(row)) + "| " +
		stringPadding(3, std::to_string(col)) + "| " +
		stringPadding(25, TokenName[type]) + "| " +
		stringPadding(25, text) + "| " +
		textValue;

	return res;
}

void Token::assignValue(std::string text, int base)
{
	try {
		if (type == CONST_INTEGER) {
			value.integer = std::stoi(text, 0, base);
			textValue = std::to_string(value.integer);
		}
		else if (type == CONST_DOUBLE) {
			value.real = std::stod(text);
			char number[40];
			sprintf(number, "%.15lf", value.real);
			textValue = number;
		}
		else if (type == CONST_CHARACTER) {
			if (std::stoi(text) < 0 || 255 < std::stoi(text)) {
				throw LexicalException(row, col, "Illegal char constant");
			}
			value.string = new char[2];
			value.string[0] = char(std::stoi(text));
			value.string[1] = 0;
			stringAllocated = true;
			textValue = value.string;
		}
		else {
			textValue = "";
		}
	}
	catch (std::out_of_range e) {
		throw LexicalException(row, col, "Number is too big");
	}
	catch (LexicalException e) {
		throw e;
	}
	catch (std::exception e) {
		throw LexicalException(row, col, e.what());
	}
}

std::string TokenName[] = {
	"UNDEFINED",

	// Data type
	"CONST_INTEGER",
	"CONST_DOUBLE",
	"CONST_CHARACTER",
	"CONST_STRING",

	// Keywords
	"KEYWORD_INTEGER",
	"KEYWORD_DOUBLE",
	"KEYWORD_CHARACTER",
	"KEYWORD_VAR",
	"KEYWORD_AND",
	"KEYWORD_ARRAY",
	"KEYWORD_BEGIN",
	"KEYWORD_BREAK",
	"KEYWORD_CASE",
	"KEYWORD_CONTINUE",
	"KEYWORD_CONST",
	"KEYWORD_DIV",
	"KEYWORD_DO",
	"KEYWORD_DOWNTO",
	"KEYWORD_ELSE",
	"KEYWORD_END",
	"KEYWORD_EXIT",
	"KEYWORD_FILE",
	"KEYWORD_FOR",
	"KEYWORD_FUNCTION",
	"KEYWORD_IF",
	"KEYWORD_IN",
	"KEYWORD_MOD",
	"KEYWORD_NIL",
	"KEYWORD_NOT",
	"KEYWORD_OF",
	"KEYWORD_OR",
	"KEYWORD_PROCEDURE",
	"KEYWORD_RECORD",
	"KEYWORD_REPEAT",
	"KEYWORD_SET",
	"KEYWORD_SHL",
	"KEYWORD_SHR",
	"KEYWORD_STRING",
	"KEYWORD_THEN",
	"KEYWORD_TO",
	"KEYWORD_TYPE",
	"KEYWORD_UNTIL",
	"KEYWORD_WHILE",
	"KEYWORD_WITH",
	"KEYWORD_XOR",
	"KEYWORD_LABEL",
	"KEYWORD_PROGRAM",
	"KEYWORD_ASSIGN",
	"KEYWORD_UNRESERVED",
	"KEYWORD_WRITE",
	"KEYWORD_WRITELN",
	"KEYWORD_READ",
	"KEYWORD_READLN",
	"KEYWORD_EOF",
	"VARIABLE",

	// Separators
	"SEP_BRACKET_LEFT",
	"SEP_BRACKET_RIGHT",
	"SEP_BRACKET_SQUARE_LEFT",
	"SEP_BRACKET_SQUARE_RIGHT",
	"SEP_BRACKET_FIGURE_LEFT",
	"SEP_BRACKET_FIGURE_RIGHT",
	"SEP_SEMICOLON",
	"SEP_COMMA",
	"SEP_DOT",
	"SEP_DOUBLE_DOT",

	// Operators
	"OP_MINUS",
	"OP_PLUS",
	"OP_MULT",
	"OP_DIVISION",
	"OP_NOT_EQUAL",
	"OP_COLON",
	"OP_CAP",
	"OP_AT",
	"OP_LESS",
	"OP_GREATER",
	"OP_EQUAL",
	"OP_LESS_OR_EQUAL",
	"OP_GREATER_OR_EQUAL",
	"OP_DOLLAR",
};
