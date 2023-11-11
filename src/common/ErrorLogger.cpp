#include "ErrorLogger.h"
#include "StringConverter.h"
#include <comdef.h>


void ErrorLogger::Log(std::string message)
{
	ErrorLogger::Log(StringConverter::Str2Wstr(message));
}

void ErrorLogger::Log(std::wstring message)
{
	OutputDebugString(message.c_str());
}

void ErrorLogger::Log(std::string message, HRESULT result)
{
	Log(StringConverter::Str2Wstr(message), result);
}

void ErrorLogger::Log(std::wstring message, HRESULT result)
{
	_com_error error(result);
	std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
	ErrorLogger::Log(errorMessage);
}