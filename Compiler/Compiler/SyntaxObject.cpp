#include <codecvt>
#include "SyntaxObject.h"
#include "Utils.h"
#include "Types.h"
#include "Operation.h"

SyntaxNode::SyntaxNode(PToken token, PType type, std::vector<PSyntaxNode> children, Category category)
	: token(token), type(type), children(children), category(category)
{
}

void SyntaxNode::print(std::string &output, std::string prefix, bool end)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	if (token) {
		output += prefix + (end ? "--- " : "|-- ") + token->text + "\n"; //converter.from_bytes(token->text) << std::endl;
	}

	if (children.empty()) {
		return;
	}

	prefix += (end ? "    " : "|   ");
	if (token) {
		increaseIndent(prefix, (int)token->text.length() - 1);
	}
	children[0]->print(output, prefix, false);

	for (int i = 1; i < children.size(); ++i) {
		children[i]->print(output, prefix, i == (int)children.size() - 1);
	}

	if (token) {
		decreaseIndent(prefix, token->text.length());
	}
}

std::string SyntaxNode::toString(std::string prefix)
{
	std::string res;
	print(res, prefix, false);
	return res;
}

void SyntaxNode::toAsmCode(AsmCode & code)
{
	for (auto child : children) {
		child->toAsmCode(code);
	}
}

void WriteNode::toAsmCode(AsmCode &code)
{
	if (children.size() != 1) {
		throw std::exception("Write can't contain more than one argument");
	}
	
	std::string res = "\"";
	auto child = children[0];
	AsmMemory::DataSize size;

	if (child->type->category == Type::Category::CHAR) {
		res += "%c";
		size = AsmMemory::DataSize::byte;
	}
	else if (child->type->category == Type::Category::INTEGER) {
		res += "%d";
		size = AsmMemory::DataSize::dword;
	}

	res += "\\n\"";
	child->toAsmCode(code);
	code.push_back({ AsmCommand::CommandType::pop, AsmRegister::eax });

	code.push_back({ AsmCommand::CommandType::printf });
	code.commands.back().push_back(std::make_shared<AsmValue>(res));
	code.commands.back().push_back(std::make_shared<AsmRegister>(AsmRegister::eax));
}

void ConstNode::toAsmCode(AsmCode & code)
{
	code.push_back({ AsmCommand::CommandType::push, value->toString() });
}

void VarNode::toAsmCode(AsmCode & code)
{
	code.push_back({ AsmCommand::CommandType::push, AsmMemory::DataSize::dword, code.offsets[lowerString(token->text)] });
}

void logicalOpAsmCode(AsmCode &code, PSyntaxNode node)
{
	std::map<TokenType, AsmCommand::CommandType> comTypes = {
		{ OP_GREATER, AsmCommand::setle },
		{ OP_LESS, AsmCommand::setge },
		{ OP_GREATER_OR_EQUAL, AsmCommand::setl },
		{ OP_LESS_OR_EQUAL, AsmCommand::setg },
		{ OP_EQUAL, AsmCommand::setne },
		{ OP_NOT_EQUAL, AsmCommand::sete },
	};

	auto tmp = comTypes[node->token->type];

	node->children[0]->toAsmCode(code);
	node->children[1]->toAsmCode(code);

	code.push_back({ AsmCommand::pop, AsmRegister::ebx });
	code.push_back({ AsmCommand::cmp, AsmMemory::DataSize::dword, AsmRegister::esp, AsmRegister::ebx });
	
	code.push_back({ comTypes[node->token->type], AsmRegister::al });
	code.push_back({ AsmCommand::sub, AsmRegister::al, "1" });
	code.push_back({ AsmCommand::movsx, AsmRegister::eax, AsmRegister::al });
	code.push_back({ AsmCommand::mov, AsmMemory::DataSize::dword, AsmRegister::esp, AsmRegister::eax });
}

void BinaryOpNode::toAsmCode(AsmCode &code)
{
	if (Operation::logicalTypes.count(token->type)) {
		logicalOpAsmCode(code, std::make_shared<SyntaxNode>(*this));
		return;
	}

	std::map<TokenType, AsmCommand::CommandType> comTypes = {
		{ OP_PLUS, AsmCommand::add },
		{ OP_MINUS, AsmCommand::sub },
		{ OP_MULT, AsmCommand::imul },
		{ KEYWORD_DIV, AsmCommand::idiv },
	};

	AsmCommand::CommandType comType = comTypes[token->type];
	auto left = children[0];
	auto right = children[1];
	auto reg1 = AsmRegister::eax;
	auto reg2 = AsmRegister::ebx;

	left->toAsmCode(code);
	right->toAsmCode(code);

	code.push_back({ AsmCommand::pop, reg2 });
	code.push_back({ AsmCommand::pop, reg1 });

	if (token->type == KEYWORD_DIV) {
		code.push_back({ AsmCommand::cdq });
		code.push_back({ comType, reg2 });
	}
	else {
		code.push_back({ comType, reg1, reg2 });
	}
	code.push_back({ AsmCommand::push, reg1 });
}

void AssignStatement::toAsmCode(AsmCode &code)
{
	auto left = children[0];
	auto right = children[1];

	right->toAsmCode(code);
	code.push_back({ AsmCommand::pop, AsmMemory::dword, code.offsets[lowerString(left->token->text)] });
}

void IfStatement::toAsmCode(AsmCode &code)
{
	std::string elseLabel = code.getLabel("IFFAIL");
	std::string endLabel = code.getLabel("IFEND");

	condition->toAsmCode(code);
	code.push_back({ AsmCommand::pop, AsmRegister::eax });
	code.push_back({ AsmCommand::test, AsmRegister::eax, AsmRegister::eax });
	code.push_back({ AsmCommand::jz, elseLabel });

	if (ifPart != nullptr) {
		ifPart->toAsmCode(code);
	}
	code.push_back({ AsmCommand::jmp, endLabel });
	
	code.push_back({ AsmCommand::label, elseLabel });
	if (elsePart != nullptr) {
		elsePart->toAsmCode(code);
	}
	code.push_back({ AsmCommand::label, endLabel });
}

void WhileNode::toAsmCode(AsmCode &code)
{
	std::string condLabel = code.getLabel("WHILE_COND");
	std::string bodyLabel = code.getLabel("WHILE_BODY");
	std::string endLabel = code.getLabel("WHILE_END");

	code.push_back({ AsmCommand::jmp, condLabel });

	code.push_back({ AsmCommand::label, bodyLabel });
	body->toAsmCode(code);

	code.push_back({ AsmCommand::label, condLabel });
	condition->toAsmCode(code);
	code.push_back({ AsmCommand::pop, AsmRegister::eax });
	code.push_back({ AsmCommand::test, AsmRegister::eax, AsmRegister::eax });
	code.push_back({ AsmCommand::jnz, bodyLabel });
	code.push_back({ AsmCommand::label, endLabel });
}
