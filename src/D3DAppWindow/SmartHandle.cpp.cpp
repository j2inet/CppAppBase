#pragma once
#include <string>
#include "SmartHandle.h"

HANDLE_unique_ptr CreateFileHandle(std::wstring fileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	HANDLE handle = CreateFile(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
	{
		return nullptr;
	}
	return HANDLE_unique_ptr(handle);
}