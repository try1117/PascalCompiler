#pragma once
#include <string>
#include <fstream>

class FileReader
{
private:
	int row, col;
	std::ifstream input;
	std::string curLine;

public:
	FileReader(std::string fileName);
	char nextSymbol();
	void symbolRollback();
	bool endOfLine();
	bool endOfFile();

	int getRow();
	int getCol();
};	
