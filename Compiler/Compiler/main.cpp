#include <fstream>
#include <iostream>
#include <locale>

#include "Tokenizer.h"
#include "ExpressionParser.h"
#include "Exceptions.h"

int main(int argc, char *argv[])
{
	std::locale::global(std::locale(""));

	if (argc == 1) {
		std::cout << "Pascal compiler" << std::endl;
		std::cout << "Copyright (c) 2018 by Danilov Roman FEFU b8303a" << std::endl;
		std::cout << "[options] <file name>" << std::endl;
		std::cout << "-l option to show a table of tokens" << std::endl;
		std::cout << "-exp option to show a syntax-tree of an arithmetic expression" << std::endl;
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "-l") == 0) {
			Tokenizer tokenizer(argv[2]);
			std::ofstream output("output.txt");

			try {
				while (tokenizer.next()) {
					//std::cout << tokenizer.getCurrentToken()->toString() << std::endl;
					output << tokenizer.getCurrentToken()->toString() << std::endl;
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
		else if (strcmp(argv[1], "-exp") == 0) {
			ExpressionParser exprParser(std::shared_ptr<Tokenizer>(new Tokenizer(argv[2])));
			std::ofstream output("output.txt");

			try {
				auto e = exprParser.parse();
				e->print(output);
			}
			catch (LexicalException e) {
				output << e.what() << std::endl;
			}
			catch (SyntaxException e) {
				output << e.what() << std::endl;
			}
			catch (std::exception e) {
				output << e.what() << std::endl;
			}
		}
	}

	return 0;
}
