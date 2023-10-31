#include "framework.h"
#include "D3DAppWindow.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>
#include <string>


#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 

using namespace std;


// for string delimiter
std::vector<std::wstring> split(std::wstring s, std::wstring delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::wstring token;
    std::vector<std::wstring> res;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

std::wstring Str2Wstr(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}


std::vector<std::wstring> loadFileLines(std::wstring sourceFileName)
{
    std::vector<std::wstring> retVal;
    HANDLE hFile = CreateFile(sourceFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return retVal;
    INT64 fileSize;
    DWORD fileSizeHigh;
    fileSize = GetFileSize(hFile, &fileSizeHigh);
    fileSize = fileSize | (fileSizeHigh << 32);
    if (fileSize != 0)
    {
        auto buffer = new char[fileSize + 1];
        ZeroMemory(buffer, fileSize + 1);
        DWORD bytesRead;
        ReadFile(hFile, buffer, fileSizeHigh, &bytesRead, FALSE);
        CloseHandle(hFile);
        std::wstring fileContents = Str2Wstr(buffer);
        auto  fileLines = split(fileContents, L"\n");
    }


    /*
    if(hFile == INVALID_HANDLE)

    
    std::wstring line;
    std::wifstream inputFile;    
    inputFile.open(sourceFileName.c_str());
    if (inputFile)
    {        

        std::locale utf8_locale(std::locale::empty(), new codecvt_utf8<wchar_t>);
        inputFile.imbue(utf8_locale);
        while (std::getline(inputFile, line))
        {
            retVal.push_back(line);
        }
    }
    
    */
    return retVal;
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    std::map<std::wstring, ComPtr<ID3D10Blob>> shaderMap;


    auto shaderFileList = loadFileLines(L"shaderList.txt");
    for (auto it = shaderFileList.begin(); it != shaderFileList.end(); ++it)
    {
        auto parts = split(*it, L",");
        if (parts.size() == 4)
        {
            std::wstring objName = parts[0];
            std::wstring fileName = parts[1];
            std::wstring entryPointName = parts[2];
            std::wstring shaderVersion = parts[3];

            ComPtr<ID3D10Blob> shaderBlob;
            ComPtr<ID3DBlob> errorBlob;
            D3DCompileFromFile(fileName.c_str(), 0, 0, (LPCSTR)entryPointName.c_str(), (LPCSTR)shaderVersion.c_str(), 0, 0, &shaderBlob, &errorBlob);
            shaderMap.emplace(objName, shaderBlob);


        }
    }

    if (FAILED(CoInitialize(NULL)))
        return -1;

    std::shared_ptr<AppWindow> appWindow = std::make_shared<D3DAppWindow>(hInstance);
    appWindow->Init();
    appWindow->RunMessageLoop();

    

    CoUninitialize();

    return 0;
}
