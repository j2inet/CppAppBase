#pragma once
#include <Windows.h>
#include "StringConverter.h"


class ErrorLogger
{
public:
	static void Log(std::string message);
	static void Log(std::wstring message);

	static void Log(std::string message, HRESULT result);
	static void Log(std::wstring message, HRESULT result);
};