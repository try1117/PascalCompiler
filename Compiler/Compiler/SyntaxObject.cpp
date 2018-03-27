#include <codecvt>
#include "SyntaxObject.h"
#include "Utils.h"
#include "Types.h"

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
	int offset = 0;
	std::string res = "\"";
	std::vector<std::shared_ptr<AsmParameter>> parameters;

	for (auto child : children) {
		auto category = child->type->category;
		AsmMemory::DataSize size;

		if (category == Type::Category::CHAR) {
			res += "%c";
			size = AsmMemory::DataSize::byte;
		}
		else if (category == Type::Category::INTEGER) {
			res += "%d";
			size = AsmMemory::DataSize::dword;
		}
		else if (category == Type::Category::DOUBLE) {
			res += "%f";
			size = AsmMemory::DataSize::qword;
		}

		child->toAsmCode(code);
		parameters.push_back(std::make_shared<AsmRegister>(AsmRegister::RegisterType::eax, size, offset));
		offset += child->type->size;
	}
	res += "\\n\"";

	code.push_back({ AsmCommand::CommandType::mov, AsmRegister::RegisterType::eax, AsmRegister::RegisterType::esp });
	parameters.push_back(std::make_shared<AsmValue>(res));
	reverse(parameters.begin(), parameters.end());
	code.push_back({ AsmCommand::CommandType::printf, parameters });
	code.push_back({ AsmCommand::CommandType::add, AsmRegister::RegisterType::esp, std::to_string(offset) });
}

void ConstNode::toAsmCode(AsmCode & code)
{
	code.push_back({ AsmCommand::CommandType::push, value->toString() });
}
