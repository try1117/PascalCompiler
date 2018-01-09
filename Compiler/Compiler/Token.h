#pragma once
#include <map>

enum Token {
	UNDEFINED,

	// Data type
	CONST_INTEGER,
	CONST_DOUBLE,
	CONST_CHARACTER,
	CONST_HEX,
	CONST_STRING,

	// Keywords
	KEYWORD_INTEGER,
	KEYWORD_DOUBLE,
	KEYWORD_CHARACTER,
	KEYWORD_VAR,
	KEYWORD_AND,
	KEYWORD_ARRAY,
	KEYWORD_BEGIN,
	KEYWORD_BREAK,
	KEYWORD_CASE,
	KEYWORD_CONTINUE,
	KEYWORD_CONST,
	KEYWORD_DIV,
	KEYWORD_DO,
	KEYWORD_DOWNTO,
	KEYWORD_ELSE,
	KEYWORD_END,
	KEYWORD_EXIT,
	KEYWORD_FILE,
	KEYWORD_FOR,
	KEYWORD_FUNCTION,
	KEYWORD_IF,
	KEYWORD_IN,
	KEYWORD_MOD,
	KEYWORD_NIL,
	KEYWORD_NOT,
	KEYWORD_OF,
	KEYWORD_OR,
	KEYWORD_PROCEDURE,
	KEYWORD_RECORD,
	KEYWORD_REPEAT,
	KEYWORD_SET,
	KEYWORD_SHL,
	KEYWORD_SHR,
	KEYWORD_STRING,
	KEYWORD_THEN,
	KEYWORD_TO,
	KEYWORD_TYPE,
	KEYWORD_UNTIL,
	KEYWORD_WHILE,
	KEYWORD_WITH,
	KEYWORD_XOR,
	KEYWORD_LABEL,
	KEYWORD_PROGRAM,
	KEYWORD_ASSIGN,
	KEYWORD_UNRESERVED,
	KEYWORD_WRITE,
	KEYWORD_WRITELN,
	OP_LATTICE,
	OP_DOLLAR,
	KEYWORD_READ,
	KEYWORD_READLN,
	KEYWORD_EOF,
	VARIABLE,

	// Separators
	SEP_BRACKET_LEFT,
	SEP_BRACKET_RIGHT,
	SEP_BRACKET_SQUARE_LEFT,
	SEP_BRACKET_SQUARE_RIGHT,
	SEP_BRACKET_FIGURE_LEFT,
	SEP_BRACKET_FIGURE_RIGHT,
	SEP_SEMICOLON,
	SEP_COMMA,
	SEP_DOT,
	SEP_DOUBLE_DOT,

	// Operators
	OP_MINUS,
	OP_PLUS,
	OP_MULT,
	OP_DIVISION,
	OP_NOT_EQUAL,
	OP_COLON,
	OP_CAP,
	OP_AT,
	OP_LESS,
	OP_GREATER,
	OP_EQUAL,
	OP_LESS_OR_EQUAL,
	OP_GREATER_OR_EQUAL,
};

extern std::string TokenName[];
