#pragma once
#include <stdint.h>
#include <Windows.h>
#include "ErrorLogger.h"



class Exception
{
public:
	virtual void WriteToLog() = 0;
};

class ArgumentNullException : public Exception
{
public:
	ArgumentNullException(const wchar_t* file, uint32_t line, const wchar_t* argument);
	void WriteToLog() override;

private:
	const wchar_t* _filename;
	uint32_t _line;
	const wchar_t* _argument;
};

#define ThrowArgumentNullException(argument) \
	throw ArgumentNullException(__FILEW__, __LINE__,  _STR2WSTR(#argument))


class ArgumentOutOfRangeException : public Exception
{
public:
	ArgumentOutOfRangeException(const wchar_t* file, uint32_t line, const wchar_t* argument);
	void WriteToLog() override;

private:
	const wchar_t* _filename;
	uint32_t _line;
	const wchar_t* _argument;
};

#define ThrowArgumentOutOfRangeException(argument) \
	throw ArgumentOutOfRangeException(__FILEW__, __LINE__,  _STR2WSTR(#argument))


class COMException : public Exception
{
public:
	COMException(const wchar_t* file, uint32_t line, HRESULT result);
	void WriteToLog() override;

private:
	const wchar_t* _filename;
	uint32_t _line;
	HRESULT _result;
};

#define ThrowCOMException(result) \
	throw COMException(__FILEW__, __LINE__, result)




class InvalidOperationException : public Exception
{
public:
	InvalidOperationException(const wchar_t* file, uint32_t line);
	void WriteToLog() override;

private:
	const wchar_t* _filename;
	uint32_t _line;
};

#define ThrowInvalidOperationException() \
	throw InvalidOperationException(__FILEW__, __LINE__)


class ContentValueRequiredException : public Exception
{
public:
	ContentValueRequiredException(const wchar_t* file, uint32_t line);
	void WriteToLog() override;

private:
	const wchar_t* _filename;
	uint32_t _line;
};

#define ThrowContentValueRequiredException() \
	throw ContentValueRequiredException(__FILEW__, __LINE__)


inline HRESULT TOF(HRESULT hr, std::wstring details = L"")
{
	if (FAILED(hr))
	{
		ErrorLogger::Log(L"COM exception occurred.", hr);
		if (details.length() > 0)
		{
			ErrorLogger::Log(details);
		}
		ThrowCOMException(hr);
	}
	return hr;
}