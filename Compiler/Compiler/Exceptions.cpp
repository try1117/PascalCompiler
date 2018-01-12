#include <cstdio>
#include "Exceptions.h"

LexicalException::LexicalException(int row, int col, std::string mes)
{
	sprintf(message, "Lexical exception in row : %d column : %d - %s", row, col, mes.c_str());
}

const char * LexicalException::what() const
{
	return message;
}
