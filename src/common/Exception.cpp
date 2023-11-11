#include "Exception.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <Windows.h>

ArgumentNullException::ArgumentNullException(const wchar_t* file, uint32_t line, const wchar_t* argument)
	: _filename(file), _line(line), _argument(argument)
{
}

void ArgumentNullException::WriteToLog()
{
	std::wofstream log;
	log.open(L"errors.log", std::ofstream::app);

	//std::wstringstream log;

	log << L"--ArugmentNullException--";
	log << L"\n\tfile='" << _filename << "'\n\tline=" << _line << "\n\targument='" << _argument << "'" << std::endl;
	log.flush();

	//EventLog::WriteError(15, log.str());
}


ArgumentOutOfRangeException::ArgumentOutOfRangeException(const wchar_t* file, uint32_t line, const wchar_t* argument)
	: _filename(file), _line(line), _argument(argument)
{
}

void ArgumentOutOfRangeException::WriteToLog()
{
	std::wofstream log;
	log.open(L"errors.log", std::ofstream::app);

	//std::wstringstream log;

	log << L"--ArgumentOutOfRangeException--";
	log << L"\n\tfile='" << _filename << "'\n\tline=" << _line << "\n\targument='" << _argument << "'" << std::endl;
	log.flush();

	//EventLog::WriteError(16, log.str());
}


COMException::COMException(const wchar_t* file, uint32_t line, HRESULT result)
	: _filename(file), _line(line), _result(result)
{
}

void COMException::WriteToLog()
{
	//std::wofstream log;
	//log.open(L"errors.log", std::ofstream::app);

	std::wstringstream log;

	log << L"--COMException--";
	log << L"\n\tfile='" << _filename << "'\n\tline=" << _line << "\n\result='" << _result << "'\n" << std::endl;
	log.flush();

	//EventLog::WriteError(16, log.str());
}


InvalidOperationException::InvalidOperationException(const wchar_t* file, uint32_t line)
	: _filename(file), _line(line)
{
}

void InvalidOperationException::WriteToLog()
{
	std::wofstream log;
	log.open(L"errors.log", std::ofstream::app);

	//std::wstringstream log;

	log << L"--InvalidOperationException--";
	log << L"\n\tfile='" << _filename << "'\n\tline=" << _line << "'" << std::endl;
	log.flush();

	//EventLog::WriteError(16, log.str());
}


ContentValueRequiredException::ContentValueRequiredException(const wchar_t* file, uint32_t line)
	: _filename(file), _line(line)
{
}

void ContentValueRequiredException::WriteToLog()
{
	std::wofstream log;
	log.open(L"errors.log", std::ofstream::app);

	//std::wstringstream log;

	log << L"--ContentValueRequiredException--";
	log << L"\n\tfile='" << _filename << "'\n\tline=" << _line << "'" << std::endl;
	log.flush();

	//EventLog::WriteError(16, log.str());
}
