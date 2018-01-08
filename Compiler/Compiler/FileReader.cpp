#include <cassert>
#include "FileReader.h"

FileReader::FileReader(std::string fileName)
	: input(fileName), row(-1), col(0)
{
}

char FileReader::nextSymbol()
{
	if (col + 1 >= curLine.length()) {
		if (!std::getline(input, curLine)) {
			return 0;
		}
		++row;
		col = -1;
	}

	return curLine[++col];
}

void FileReader::symbolRollback()
{
	assert(col--);
}

bool FileReader::endOfLine()
{
	return col + 1 == curLine.length();
}

bool FileReader::endOfFile()
{
	return endOfLine() && input.eof();
}

int FileReader::getRow()
{
	return row;
}

int FileReader::getCol()
{
	return col;
}
