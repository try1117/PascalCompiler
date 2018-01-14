#pragma once
#include <string>
#include <fstream>

class FileReader
{
public:
	FileReader(std::string fileName);
	char nextSymbol();
	void symbolRollback();
	void nextLine();
	bool endOfLine();
	bool endOfFile();

	int getRow();
	int getCol();

private:
	int row, col;
	std::ifstream input;
	std::string curLine;
};	
