#include <codecvt>
#include "SyntaxObject.h"

SyntaxObject::SyntaxObject(std::shared_ptr<Token> token, std::initializer_list<std::shared_ptr<SyntaxObject>> children)
	: token(token), children(children)
{
}

void SyntaxObject::print(std::ofstream &output, std::string prefix, bool end)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	output << prefix + (end ? "--- " : "|-- ") + token->text << std::endl; //converter.from_bytes(token->text) << std::endl;

	if (children.empty()) {
		return;
	}

	children[0]->print(output, prefix + (end ? "    " : "|   "), false);

	for (int i = 1; i < children.size(); ++i) {
		children[i]->print(output, prefix + (end ? "    " : "|   "), i == (int)children.size() - 1);
	}
}
