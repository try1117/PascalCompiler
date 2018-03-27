#pragma once
#include <memory>
#include <map>
#include <set>

#include "SymbolTable.h"
#include "SyntaxObject.h"
#include "Utils.h"

class Type;
typedef std::shared_ptr<Type> PType;

class Type {
public:
	enum Category {
		INTEGER,
		DOUBLE,
		CHAR,
		STRING,
		ARRAY,
		RECORD,
		FUNCTION,
		NIL,
	};

	static const std::vector<std::string> categoryName;
	static const std::set<Category> simpleCategories;

	Category category;
	int size;

	Type(Category category = Category::NIL, int size = 0)
		: category(category), size(size)
	{}

	static std::string indent;

	static PType getSimpleType(Type::Category category);
	virtual std::string toString();
};

class ArrayType : public Type {
public:
	PType elementType;
	PConstNode left, right;

	ArrayType(PType elementType, PConstNode left, PConstNode right)
		: Type(Category::ARRAY), elementType(elementType), left(left), right(right)
	{}

	std::string toString();
};

class RecordType : public Type {
public:
	PSymbolTable fields;

	RecordType(PSymbolTable table)
		: Type(Category::RECORD), fields(table)
	{}

	std::string toString();
};

class FunctionType : public Type {
public:
	PSymbolTable parameters, declarations;
	PType returnType;
	PSyntaxNode body;
	std::string name;

	FunctionType(PSymbolTable parameters, PSymbolTable declarations, PType returnType, PSyntaxNode body, std::string name)
		: Type(Category::FUNCTION), parameters(parameters), declarations(declarations), returnType(returnType), body(body), name(name)
	{}

	std::string toString();
};
