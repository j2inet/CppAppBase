#include "TcpListener.h"


std::string hexChars = "0123456789ABCDEF";
bool  isSafeChar(char c)
{
	return isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~';
}

std::string UrlDecode(const std::string str)
{
	std::vector<char> buffer;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '%')
		{
			if (i + 2 < str.size())
			{
				char c = (char)std::stoi(str.substr(i + 1, 2), nullptr, 16);
				buffer.push_back(c);
				i += 2;
			}
			else
			{
				buffer.push_back(str[i]);
			}
		}
		else if (str[i] == '+')
		{
			buffer.push_back(' ');
		}
		else
		{
			buffer.push_back(str[i]);
		}
	}
	return std::string(buffer.begin(), buffer.end());
}

std::string UrlEncode(const std::string str)
{
	std::vector<char> buffer;
	for (auto c : str)
	{
		if (isSafeChar(c))
		{
			buffer.push_back(c);
		}
		else
		{
			buffer.push_back('%');
			buffer.push_back(hexChars[c >> 4]);
			buffer.push_back(hexChars[c & 0x0F]);
		}
	}
	return std::string(buffer.begin(), buffer.end());
}
