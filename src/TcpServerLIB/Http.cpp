#include "Http.h"

HttpRequest::HttpRequest(std::string requestString)
{
	size_t pos = requestString.find("\r\n");
	while (pos != std::string::npos)
	{
		std::string line = requestString.substr(0, pos);
		requestString = requestString.substr(pos + 2);
		pos = requestString.find("\r\n");
		if (line.empty())
		{
			break;
		}
		if (method.empty())
		{
			size_t pos = line.find(' ');
			method = line.substr(0, pos);
			line = line.substr(pos + 1);
			pos = line.find(' ');
			path = line.substr(0, pos);
			line = line.substr(pos + 1);
			pos = line.find(' ');
			version = line.substr(0, pos);
		}
		else
		{
			size_t pos = line.find(':');
			std::string name = line.substr(0, pos);
			std::string value = line.substr(pos + 2);
			headers[name] = value;
		}
	}
}

std::string HttpRequest::GetMethod()
{
	return method;
}

std::string HttpRequest::GetPath()
{
	return path;
}

std::string HttpRequest::GetDecodedPath()
{
	return UrlDecode(path);
}

std::string HttpRequest::GetQuery()
{
	return query;
}

std::string HttpRequest::GetVersion()
{
	return version;
}

std::string HttpRequest::GetHeader(std::string name)
{
	return headers[name];
}
