#include <codecvt>
#include "SyntaxObject.h"

SyntaxNode::SyntaxNode(PToken token, PType type, std::initializer_list<PSyntaxNode> children)
	: token(token), type(type), children(children)
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

	children[0]->print(output, prefix + (end ? "    " : "|   "), false);

	for (int i = 1; i < children.size(); ++i) {
		children[i]->print(output, prefix + (end ? "    " : "|   "), i == (int)children.size() - 1);
	}
}

std::string SyntaxNode::toString(std::string prefix)
{
	std::string res;
	print(res, prefix, false);
	return res;
}
