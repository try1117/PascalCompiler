#include <codecvt>
#include "SyntaxObject.h"

SyntaxNode::SyntaxNode(PToken token, std::initializer_list<PSyntaxNode> children)
	: token(token), children(children)
{
}

void SyntaxNode::print(std::stringstream &output, std::string prefix, bool end)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	if (token) {
		output << prefix + (end ? "--- " : "|-- ") + token->text << std::endl; //converter.from_bytes(token->text) << std::endl;
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
	std::stringstream tmp;
	print(tmp, prefix, false);
	std::string res;
	tmp >> res;
	return res;
}
