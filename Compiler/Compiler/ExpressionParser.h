#pragma once
#include <memory>
#include "Token.h"

class ExpressionParser {
	
	class Node {
	public:
		std::shared_ptr<Node> left, right;
		Token token;
	};

};
