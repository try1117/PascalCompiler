#include <algorithm>
#include "Utils.h"

std::string lowerString(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

int lastLineWidth(std::string s)
{
	return 0;
}
