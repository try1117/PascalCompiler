#include "Types.h"

const std::vector<std::string> Type::categoryName = {
	"Integer",
	"Double",
	"Char",
	"String",
	"Array",
	"Record",
	"Function",
	"Nil",
};

std::string FunctionType::indent = "";

PType Type::getSimpleType(Type::Category category)
{
	static const std::map<Category, PType> types = {
		{ INTEGER, std::make_shared<Type>(INTEGER) },
		{ DOUBLE, std::make_shared<Type>(DOUBLE) },
		{ CHAR, std::make_shared<Type>(CHAR) },
		{ STRING, std::make_shared<Type>(STRING) },
		{ NIL, std::make_shared<Type>(NIL) },
	};
	return types.at(category);
	//return types[category];
}

std::string Type::toString()
{
	return categoryName[category];
}

FunctionType::FunctionType(PType returnType, PSyntaxNode body, std::string name)
	: returnType(returnType), body(body), name(name)
{
}

std::string FunctionType::toString()
{
	std::string res;
	res += indent + name + " : function()" + "\n";
	indent += "   ";
	res += indent + "resultType : " + returnType->toString() + "\n";
	if (body != nullptr) {
		res += body->toString(indent);
	}
	return res;
}
