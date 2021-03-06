#pragma once
#include <map>
#include <string>
#include <memory>

enum TokenType;
extern std::string TokenName[];
extern std::string TokenFriendlyName[];

class Token;
typedef std::shared_ptr<Token> PToken;

class IdentifierValue {
public:
	union Value {
		int integer;
		double _double;
		char *string;
	} get;
	bool stringAllocated;

	enum Category {
		INTEGER, DOUBLE, CHAR, STRING, NIL,
	} category;

	IdentifierValue() : stringAllocated(false) {}
	IdentifierValue(int integer);
	IdentifierValue(double _double);
	IdentifierValue(char c);
	IdentifierValue(std::string s);

	void setInteger(int val);
	void setDouble(double val);
	void setChar(char val);
	void setString(std::string val);

	int getInteger();
	double getDouble();
	char getChar();
	std::string getString();

	std::string toString();
	int toInteger();
	double toDouble();

	IdentifierValue::Value cloneValue();
	
	~IdentifierValue() {
		releaseMemory();
	}

private:
	void requireCategory(std::initializer_list<Category> categories);
	void releaseMemory();
};

typedef std::shared_ptr<IdentifierValue> PIdentifierValue;

class Token {
public:
	TokenType type;
	std::string text;
	std::string textValue;
	int row, col;

	PIdentifierValue value;

	Token(TokenType type, int row, int col, std::string text = "");
	std::string toString();
	void assignValue(std::string text, int base);
};

enum TokenType {
	UNDEFINED,

	// Data type
	CONST_INTEGER,
	CONST_DOUBLE,
	CONST_CHARACTER,
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
	KEYWORD_READ,
	KEYWORD_READLN,
	KEYWORD_EOF,
	IDENTIFIER,

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
