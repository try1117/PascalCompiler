#include <codecvt>
#include "SyntaxObject.h"

SyntaxObject::SyntaxObject(std::shared_ptr<Token> token, std::shared_ptr<SyntaxObject> left, std::shared_ptr<SyntaxObject> right)
	: token(token), left(left), right(right)
{
}

void SyntaxObject::print(std::ofstream &output, std::string prefix, bool end)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	output << prefix + (end ? "--- " : "|-- ") + token->text << std::endl; //converter.from_bytes(token->text) << std::endl;

	if (left != nullptr) {
		left->print(output, prefix + (end ? "    " : "|   "), false);
	}
	if (right != nullptr) {
		right->print(output, prefix + (end ? "    " : "|   "), true);
	}
}
