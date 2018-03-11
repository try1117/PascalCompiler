#pragma once
#include <memory>
#include <map>
#include <set>

#include "SyntaxObject.h"

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

	Category category;
	Type(Category category = Category::NIL)
		: category(category) {}

	static PType getSimpleType(Type::Category category);
	virtual std::string toString();
};

class FunctionType : public Type {
public:
	PType returnType;
	PSyntaxNode body;
	std::string name;

	static std::string indent;

	FunctionType(PType returnType, PSyntaxNode body, std::string name);
	std::string toString();
};
