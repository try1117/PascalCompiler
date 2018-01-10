#include <fstream>
#include <iostream>

#include "Tokenizer.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		std::cout << "Pascal compiler" << std::endl;
		std::cout << "Copyright (c) 2018 by Danilov Roman FEFU b8303a" << std::endl;
		std::cout << "[options] <file name>" << std::endl;
		std::cout << "-l option to show table of tokens" << std::endl;
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "-l") == 0) {
			Tokenizer tokenizer(argv[2]);
			std::ofstream output("output.txt");

			try {
				while (tokenizer.next()) {
					//std::cout << tokenizer.toString() << std::endl;
					output << tokenizer.toString() << std::endl;
				}
			}
			catch (LexicalException e) {
				output << e.what() << std::endl;
			}
			catch (std::exception e) {
				output << e.what() << std::endl;
			}
			//std::system("pause");
		}
	}

	return 0;
}
