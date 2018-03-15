#include "Types.h"
#include "Utils.h"

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

const std::set<Type::Category> Type::simpleCategories = {
	Type::INTEGER,
	Type::DOUBLE,
	Type::CHAR,
	Type::STRING,
	Type::NIL,
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

FunctionType::FunctionType(PSymbolTable declarations, PType returnType, PSyntaxNode body, std::string name)
	: declarations(declarations), returnType(returnType), body(body), name(name)
{
}

std::string FunctionType::toString()
{
	std::string res;
	res += indent + name + " : function()" + "\n";
	indent += "   ";
	res += indent + "resultType : " + returnType->toString() + "\n";
	
	res += indent + "\nDeclarations:\n";

	for (auto it : declarations->symbolsArray) {
		if (it->type->category == Type::Category::FUNCTION) {
			continue;
		}
		else {
			std::string symcat = Symbol::categoryName[it->category];
			res += indent + it->token->text + " : " + symcat + (!symcat.empty() ? " " : "") + Type::categoryName[it->type->category] + "\n";
			std::string strValue;
			if (it->value != nullptr) {
				strValue = it->value->toString(std::string((indent + it->token->text).length() - 1, ' '));
			}
			res += (strValue.empty() ? "" : strValue + "\n");
		}
	}
	
	if (body != nullptr) {
		res += body->toString(indent);
	}
	return res;
}
