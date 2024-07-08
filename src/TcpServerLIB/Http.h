#pragma once
#include "UrlConverter.h"
#include <string>
#include <map>

class HttpRequest
{
public:
	HttpRequest(std::string request);
	std::string GetMethod();
	std::string GetPath();
	std::string GetDecodedPath();
	std::string GetQuery();
	std::string GetVersion();
	std::string GetHeader(std::string name);
	std::string GetBody();
private:
	std::string method;
	std::string path;
	std::string query;
	std::string version;
	std::map<std::string, std::string> headers;
	std::string body;
};
