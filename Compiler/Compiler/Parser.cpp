#include <functional>
#include <algorithm>
#include "Parser.h"
#include "Exceptions.h"

Parser::Parser(std::shared_ptr<Tokenizer> tokenizer)
	: tokenizer(tokenizer)
{
}

void Parser::goToNextToken()
{
	tokenizer->next();
}

PToken Parser::currentToken()
{
	return tokenizer->getCurrentToken();
}

TokenType Parser::currentTokenType()
{
	return tokenizer->getCurrentToken()->type;
}

void Parser::requireCurrent(std::initializer_list<TokenType> types)
{
	std::string res;
	for (auto type : types) {
		if (currentTokenType() == type)
			return;
		if (!res.empty()) {
			res += " or ";
		}
		res += TokenName[type];
	}

	throw SyntaxException(currentToken()->row, currentToken()->col,
		"Expected " + res + " but found " + currentToken()->text);
}

void Parser::requireNext(std::initializer_list<TokenType> types)
{
	goToNextToken();
	requireCurrent(types);
}

PSyntaxNode Parser::parseLogical()
{
	auto node = parseExpr();
	auto token = currentToken();

	while (Operation::logicalTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseExpr(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseExpr()
{
	auto node = parseTerm();
	auto token = currentToken();

	while (Operation::exprTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseTerm(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseTerm()
{
	auto node = parseFactor();
	auto token = currentToken();

	while (Operation::termTypes.count(token->type)) {
		goToNextToken();
		node = createOperationNode(node, parseFactor(), token);
		token = currentToken();
	}

	return node;
}

PSyntaxNode Parser::parseFactor()
{
	auto token = currentToken();
	goToNextToken();

	if (token->type == SEP_BRACKET_LEFT) {
		auto node = parseLogical();
		requireCurrent({ SEP_BRACKET_RIGHT });
		goToNextToken();
		return node;
	}
	else if (token->type == OP_MINUS || token->type == OP_PLUS) {
		return std::make_shared<UnaryOpNode>(token, nullptr, std::initializer_list<PSyntaxNode>({ parseFactor() }));
	}
	else if (token->type == KEYWORD_NOT) {
		return std::make_shared<NotNode>(token, nullptr, std::initializer_list<PSyntaxNode>({ parseFactor() }));
	}
	else if (token->type == IDENTIFIER) {
		if (instanceOfConstNode(getSymbol(token)->value)) {
			return getSymbol(token)->value;
		}
		return std::make_shared<VarNode>(token, getSymbol(token)->type);
	}
	else if (token->type == CONST_INTEGER) {
		return std::make_shared<ConstNode>(token, Type::getSimpleType(Type::Category::INTEGER),
			std::make_shared<IdentifierValue>(token->value->getInteger()));
	}
	else if (token->type == CONST_DOUBLE) {
		return std::make_shared<ConstNode>(token, Type::getSimpleType(Type::Category::DOUBLE),
			std::make_shared<IdentifierValue>(token->value->getDouble()));
	}
	else if (token->type == CONST_CHARACTER) {
		return std::make_shared<ConstNode>(token, Type::getSimpleType(Type::Category::CHAR),
			std::make_shared<IdentifierValue>(token->value->getChar()));
	}
	else if (token->type == CONST_STRING) {
		return std::make_shared<ConstNode>(token, Type::getSimpleType(Type::Category::STRING),
			std::make_shared<IdentifierValue>(token->value->getString()));
	}
	else {
		throw SyntaxException(token->row, token->col, "Expected identifier, constant or expression");
	}
}

PSymbol Parser::getSymbol(PToken token)
{
	for (int i = tables.size() - 1; i >= 0; --i) {
		if (tables[i]->getSymbol(token)) {
			return tables[i]->getSymbol(token);
		}
	}
	throw LexicalException(token->row, token->col, "Identifier " + token->text + " not found");
}

PType Parser::getOperationType(PType left, PType right, PToken operation)
{
	if (Operation::logicalTypes.count(operation->type)) {
		bool compatibilityTable[4][4] = {
			/* --- INT, DBL, CHR, STR */
			/* INT */ { 1, 1, 1, 0 },
			/* DBL */ { 1, 1, 1, 0 },
			/* CHR */ { 1, 1, 1, 1 },
			/* STR */ { 0, 0, 1, 1 },
		};

		if (compatibilityTable[left->category][right->category]) {
			return Type::getSimpleType(Type::Category::INTEGER);
		}
	}
	else if (Operation::integerOperationTypes.count(operation->type)) {
		if (left->category == Type::Category::INTEGER && right->category == Type::Category::INTEGER) {
			return Type::getSimpleType(Type::Category::INTEGER);
		}
	}
	else if (Operation::simpleArithmeticTypes.count(operation->type)) {
		std::set<Type::Category> numbers = { Type::Category::INTEGER, Type::Category::DOUBLE };
		if (numbers.count(left->category) && numbers.count(right->category)) {
			if (left->category == Type::Category::INTEGER && right->category == Type::Category::INTEGER) {
				return Type::getSimpleType(Type::Category::INTEGER);
			}
			else {
				return Type::getSimpleType(Type::Category::DOUBLE);
			}
		}

		std::set<Type::Category> strings = { Type::Category::CHAR, Type::Category::STRING };
		if (strings.count(left->category) && strings.count(right->category) && operation->type == OP_PLUS) {
			return Type::getSimpleType(Type::Category::STRING);
		}
	}

	throw LexicalException(operation->row, operation->col,
		"Unsupported operand types "
		+ Type::categoryName[left->category] + " and " + Type::categoryName[right->category]
		+ " for operation " + operation->text
	);
}

PSyntaxNode Parser::cast(PSyntaxNode node, PType to)
{
	if (node->type->category != to->category) {
		return std::make_shared<CastNode>(node, to);
	}
	return node;
}

PSyntaxNode Parser::castConstNode(PSyntaxNode node, PType to)
{
	if (node->type->category != to->category) {
		auto cur = std::static_pointer_cast<ConstNode>(node);
		if (to->category == Type::Category::DOUBLE) {
			cur->value->setDouble((double)cur->value->get.integer);
			node->token->text = cur->value->toString();
		}
		cur->type = Type::getSimpleType(to->category);
		return cur;
	}
	return node;
}

PSyntaxNode Parser::createOperationNode(PSyntaxNode left, PSyntaxNode right, PToken operation)
{
	PType operationType = getOperationType(left->type, right->type, operation);
	bool leftIsConst = instanceOfConstNode(left);
	bool rightIsConst = instanceOfConstNode(right);

	// result of logical operation is INTEGER
	// while operand types can be: INTEGER, DOUBLE, CHAR, STRING
	if (leftIsConst && rightIsConst && Operation::logicalTypes.count(operation->type)) {
		PIdentifierValue value = nullptr;
		auto lNode = std::static_pointer_cast<ConstNode>(left);
		auto rNode = std::static_pointer_cast<ConstNode>(right);

		Type::Category lcat = left->type->category;
		Type::Category rcat = right->type->category;
		
		std::set<Type::Category> strings = { Type::CHAR, Type::STRING };

		if (strings.count(lcat) && strings.count(rcat)) {
			value = std::make_shared<IdentifierValue>(Operation::evalLogicalOperation<std::string>(lNode->value->getString(), rNode->value->getString(), operation));
		}
		else if (lcat == Type::DOUBLE || rcat == Type::DOUBLE) {
			value = std::make_shared<IdentifierValue>(Operation::evalLogicalOperation<double>(lNode->value->toDouble(), rNode->value->toDouble(), operation));
		}
		else {
			value = std::make_shared<IdentifierValue>(Operation::evalLogicalOperation<int>(lNode->value->toInteger(), rNode->value->toInteger(), operation));
		}
		return std::make_shared<ConstNode>(operation, operationType, value);
	}

	// otherwise cast operands to operationType
	left = (leftIsConst ? castConstNode(left, operationType) : cast(left, operationType));
	right = (rightIsConst ? castConstNode(right, operationType) : cast(right, operationType));
	if (leftIsConst && rightIsConst) {
		return std::make_shared<ConstNode>(operation, operationType, evalOperation(left, right, operation, operationType));
	}
	return std::make_shared<BinaryOpNode>(operation, operationType, std::initializer_list<PSyntaxNode>({left, right}));
}

PIdentifierValue Parser::evalOperation(PSyntaxNode left, PSyntaxNode right, PToken operation, PType operationType)
{
	auto leftNode = std::static_pointer_cast<ConstNode>(left);
	auto rightNode = std::static_pointer_cast<ConstNode>(right);
	
	if (operationType->category == Type::Category::INTEGER) {
		return std::make_shared<IdentifierValue>(Operation::evalIntegers(leftNode->value->get.integer, rightNode->value->get.integer, operation));
	}
	else if (operationType->category == Type::Category::DOUBLE) {
		return std::make_shared<IdentifierValue>(Operation::evalDoubles(leftNode->value->get._double, rightNode->value->get._double, operation));
	}
	// char or string
	else {
		return std::make_shared<IdentifierValue>(Operation::evalStrings(leftNode->value->get.string, rightNode->value->get.string, operation));
	}
}

PType Parser::parse()
{
	tables.push_back(std::make_shared<SymbolTable>());
	parseProgram();
	declarationPart();
	std::shared_ptr<FunctionType> res(new FunctionType(tables.back(), Type::getSimpleType(Type::Category::NIL), compoundStatement(), programName));
	requireCurrent({ SEP_DOT });
	return res;
}

void Parser::parseProgram()
{
	requireNext({ KEYWORD_PROGRAM });
	requireNext({ IDENTIFIER });
	programName = currentToken()->text;
	requireNext({ SEP_SEMICOLON });
	goToNextToken();
}

void Parser::declarationPart()
{
	while (true) {
		if (currentTokenType() == KEYWORD_TYPE) {
			goToNextToken();
			typeDeclarationPart();
		}
		else if (currentTokenType() == KEYWORD_VAR) {
			goToNextToken();
			variableDeclarationPart();
		}
		else if (currentTokenType() == KEYWORD_CONST) {
			goToNextToken();
			constDeclarationPart();
		}
		else {
			return;
		}
	}
}

void Parser::typeDeclarationPart()
{
	do {
		requireCurrent({ IDENTIFIER });
		auto identifier = currentToken();
		requireNext({ OP_EQUAL });
		goToNextToken();
		PType type = parseType();
		tables.back()->addType(identifier, type);
		requireCurrent({ SEP_SEMICOLON });
		goToNextToken();
	} while (currentTokenType() == IDENTIFIER);
}

PType Parser::parseType()
{
	std::map<TokenType, Type::Category> simpleCategories = {
		{ TokenType::KEYWORD_INTEGER, Type::Category::INTEGER },
		{ TokenType::KEYWORD_DOUBLE, Type::Category::DOUBLE },
		{ TokenType::KEYWORD_CHARACTER, Type::Category::CHAR },
		{ TokenType::KEYWORD_STRING, Type::Category::STRING },
	};

	PToken token = currentToken();
	goToNextToken();

	if (token->type == IDENTIFIER) {
		return typeAlias(token);
	}
	else if (simpleCategories.count(token->type)) {
		return Type::getSimpleType(simpleCategories[token->type]);
	}
	else {
		throw LexicalException(token->row, token->col, "Expected type but found " + token->text);
	}
}

PType Parser::typeAlias(PToken token)
{
	return getSymbol(token)->type;
}

void Parser::variableDeclarationPart()
{
	do {
		requireCurrent({ IDENTIFIER });
		auto identifiers = identifierList();
		PType type = parseType();
		// TODO: initialization
		tables.back()->addVariables(identifiers, type);
		requireCurrent({ SEP_SEMICOLON });
		goToNextToken();
	} while (currentTokenType() == IDENTIFIER);
}

std::vector<PToken> Parser::identifierList()
{
	std::vector<PToken> res;
	while (true) {
		res.push_back(currentToken());
		requireNext({ SEP_COMMA, OP_COLON });
		if (currentTokenType() == OP_COLON) {
			goToNextToken();
			break;
		}
		goToNextToken();
	}
	return res;
}

bool Parser::instanceOfConstNode(PSyntaxNode node)
{
	return std::static_pointer_cast<ConstNode>(node) != nullptr;
	//return typeid(node) == typeid(ConstNode);
}

void Parser::constDeclarationPart()
{
	do {
		requireCurrent({ IDENTIFIER });
		PToken token = currentToken();
		requireNext({ OP_EQUAL, OP_COLON });

		// untyped constant
		if (currentTokenType() == OP_EQUAL) {
			goToNextToken();
			PSyntaxNode node = parseLogical();
			if (!instanceOfConstNode(node)) {
				throw LexicalException(node->token->row, node->token->col, "Illegal expression");
			}
			tables.back()->addConstant(token, node->type, node);
		}
		// typed constant
		else if (currentTokenType() == OP_COLON) {
			goToNextToken();
			PType type = parseType();
			requireCurrent({ OP_EQUAL });
			goToNextToken();
			PSyntaxNode value = typedConstant(type);
			tables.back()->addConstant(token, type, value);
		}
		requireCurrent({ SEP_SEMICOLON });
		goToNextToken();
	} while (currentTokenType() == IDENTIFIER);
}

void Parser::requireTypesCompatibility(PType left, PType right)
{
	std::vector<std::pair<Type::Category, Type::Category>> pairs = {
		{ Type::DOUBLE, Type::INTEGER },
		{ Type::STRING, Type::CHAR },
	};

	bool res = (left->category == right->category);
	for (auto it : pairs) {
		res = std::max(res, left->category == it.first && right->category == it.second);
	}

	if (!res) {
		throw LexicalException(currentToken()->row, currentToken()->col,
			"Incompatible types, expected " + Type::categoryName[right->category] + " but found " + Type::categoryName[left->category]);
	}
}

PSyntaxNode Parser::typedConstant(PType type)
{
	if (Type::simpleCategories.count(type->category)) {
		PSyntaxNode node = parseLogical();
		requireTypesCompatibility(type, node->type);
		if (instanceOfConstNode(node)) {
			return castConstNode(node, type);
		}
		if (node->type->category != type->category) {
			return std::make_shared<CastNode>(node, type);
		}
		return node;
	}
}

PSyntaxNode Parser::compoundStatement()
{
	requireCurrent({ KEYWORD_BEGIN });
	PSyntaxNode statement = statementList();
	requireCurrent({ KEYWORD_END });
	goToNextToken();
	return statement;
}

PSyntaxNode Parser::statementList()
{
	PSyntaxNode statement = std::make_shared<SyntaxNode>();// BodyFunction();
	do {
		goToNextToken();
		PSyntaxNode node = parseStatement();
		if (node != nullptr)
			statement->children.push_back(node);
	} while (currentTokenType() == SEP_SEMICOLON);
	return statement;
}

PSyntaxNode Parser::parseStatement()
{
	switch (currentTokenType()) {
		case IDENTIFIER:
			return nullptr;
		case KEYWORD_BEGIN:
			return compoundStatement();
		case SEP_SEMICOLON:
		case KEYWORD_END:
			return nullptr;
		default:
			throw LexicalException(currentToken()->row, currentToken()->col, "Unexpected token " + currentToken()->text);
	}
}
