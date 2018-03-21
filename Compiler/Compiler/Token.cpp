#include <algorithm>
#include "Token.h"
#include "Exceptions.h"

#include <memory>

Token::Token(TokenType type, int row, int col, std::string text)
	: type(type), row(row), col(col), text(text), value(nullptr)
{
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
			value = std::make_shared<IdentifierValue>(std::stoi(text, 0, base));
			textValue = std::to_string(value->get.integer);
		}
		else if (type == CONST_DOUBLE) {
			value = std::make_shared<IdentifierValue>(std::stod(text));
			char number[40];
			sprintf(number, "%.15lf", value->get._double);
			textValue = number;
		}
		else if (type == CONST_CHARACTER) {
			if (std::stoi(text) < 0 || 255 < std::stoi(text)) {
				throw LexicalException(row, col, "Illegal char constant");
			}
			value = std::make_shared<IdentifierValue>(char(std::stoi(text)));
			textValue = value->get.string;
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
	"IDENTIFIER",

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

std::string TokenFriendlyName[] = {
	"Undefined",

	// Data type
	"Const Integer",
	"Const Double",
	"Const Char",
	"Const String",

	// Keywords
	"Integer",
	"Double",
	"Char",
	"Var",
	"And",
	"Array",
	"Begin",
	"Break",
	"Case",
	"Continue",
	"Const",
	"Div",
	"Do",
	"Downto",
	"Else",
	"End",
	"Exit",
	"File",
	"For",
	"Function",
	"If",
	"In",
	"Mod",
	"Nil",
	"Not",
	"Of",
	"Or",
	"Procedure",
	"Record",
	"Repeat",
	"Set",
	"Shl",
	"Shr",
	"String",
	"Then",
	"To",
	"Type",
	"Until",
	"While",
	"With",
	"Xor",
	"Label",
	"Program",
	":=",
	"Unreserved",
	"Write",
	"Writeln",
	"Read",
	"Readln",
	"Eof",
	"Identifier",

	// Separators
	"(",
	")",
	"[",
	"]",
	"{",
	"}",
	";",
	",",
	".",
	"..",

	// Operators
	"-",
	"+",
	"*",
	"/",
	"<>",
	":",
	"^",
	"@",
	"<",
	">",
	"=",
	"<=",
	">=",
	"$",
};

IdentifierValue::IdentifierValue(int integer)
	: IdentifierValue()
{
	setInteger(integer);
}

IdentifierValue::IdentifierValue(double _double)
	: IdentifierValue()
{
	setDouble(_double);
}

IdentifierValue::IdentifierValue(char c)
	: IdentifierValue()
{
	setChar(c);
}

IdentifierValue::IdentifierValue(std::string s)
	: IdentifierValue()
{
	setString(s);
}

void IdentifierValue::setInteger(int val)
{
	category = INTEGER;
	releaseMemory();
	get.integer = val;
}

void IdentifierValue::setDouble(double val)
{
	category = DOUBLE;
	releaseMemory();
	get._double = val;
}

void IdentifierValue::setChar(char val)
{
	setString(std::string({ val }));
	category = CHAR;
}

void IdentifierValue::setString(std::string val)
{
	category = STRING;
	releaseMemory();
	get.string = new char[val.length() + 1];
	stringAllocated = true;
	strcpy(get.string, val.c_str());
}

int IdentifierValue::getInteger()
{
	requireCategory({ INTEGER });
	return get.integer;
}

double IdentifierValue::getDouble()
{
	requireCategory({ DOUBLE });
	return get._double;
}

char IdentifierValue::getChar()
{
	requireCategory({ CHAR });
	return get.string[0];
}

std::string IdentifierValue::getString()
{
	requireCategory({ CHAR, STRING });
	return get.string;
}

std::string IdentifierValue::toString()
{
	if (category == INTEGER) {
		return std::to_string(get.integer);
	}
	else if (category == DOUBLE) {
		return std::to_string(get._double);
	}
	else if (category == CHAR || category == STRING) {
		return "'" + std::string(get.string) + "'";
	}
	else {
		throw std::exception("Uninitialized IdentifierValue");
	}
}

int IdentifierValue::toInteger()
{
	if (category == INTEGER) return getInteger();
	else return getChar();
}

double IdentifierValue::toDouble()
{
	if (category == INTEGER) return getInteger();
	else if (category == CHAR) return getChar();
	else return getDouble();
}

IdentifierValue::Value IdentifierValue::cloneValue()
{
	if (category == CHAR || category == STRING) {
		Value res;
		res.string = new char[strlen(get.string) + 1];
		strcpy(res.string, get.string);
		return res;
	}
	return get;
}

void IdentifierValue::requireCategory(std::initializer_list<Category> categories)
{
	for (auto it : categories) {
		if (category == it)
			return;
	}
	throw std::exception("Identifier value exception : Wrong category");

}

void IdentifierValue::releaseMemory()
{
	if (stringAllocated) {
		delete get.string;
		stringAllocated = false;
	}
}
