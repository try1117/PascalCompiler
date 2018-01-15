#include "FileReader.h"

FileReader::FileReader(std::string fileName)
	: input(fileName), row(0), col(0)
{
}

char FileReader::nextSymbol()
{
	if (col + 1 > curLine.length()) {
		do {
			++row;
			if (!std::getline(input, curLine)) {
				col = curLine.length();
				return 0;
			}
		} while (curLine.empty());

		col = 0;
	}

	return curLine[col++];
}

void FileReader::symbolRollback()
{
	if (--col == 0) {
		throw std::exception("FileReader can't rollback the very first symbol of the line");
	}
}

void FileReader::nextLine()
{
	col = curLine.length();
}

bool FileReader::endOfLine()
{
	return col == curLine.length();
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
