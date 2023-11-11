#include "StringConverter.h"


std::wstring StringConverter::Str2Wstr(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

