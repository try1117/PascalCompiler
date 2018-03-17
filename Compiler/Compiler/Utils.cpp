#include <algorithm>
#include "Utils.h"

std::string lowerString(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

void increaseIndent(std::string &s, int cnt)
{
	s += std::string(cnt, ' ');
}

void decreaseIndent(std::string &s, int cnt)
{
	if (s.length() >= cnt) {
		s.erase(s.length() - cnt, cnt);
	}
}
