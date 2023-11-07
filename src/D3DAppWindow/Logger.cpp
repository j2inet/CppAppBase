#include "ErrorLogger.h"
#include "StringConverter.h"
#include <comdef.h>


void Logger::Log(std::string message)
{
	Logger::Log(StringConverter::Str2Wstr(message));
}

void Logger::Log(std::wstring message)
{
	OutputDebugString(message.c_str());
}

void Logger::Log(std::string message, HRESULT result)
{
	Log(StringConverter::Str2Wstr(message), result);
}

void Logger::Log(std::wstring message, HRESULT result)
{
	_com_error error(result);
	std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
	Logger::Log(errorMessage);
}