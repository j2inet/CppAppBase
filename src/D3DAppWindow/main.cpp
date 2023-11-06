#include "framework.h"
#include "D3DAppWindow.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>
#include <string>


#include "SmartHandle.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 

using namespace std;


// for string delimiter




/*
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
        ReadFile(hFile, buffer, fileSize, &bytesRead, FALSE);
        CloseHandle(hFile);
        std::wstring fileContents = Str2Wstr(buffer);
        delete buffer;
        retVal = split(fileContents, L"\n");
    }
    return retVal;
}
*/


vector<unsigned char> LoadFileContents(std::wstring sourceFileName)
{
    vector<unsigned char> retVal;
    auto hFile = CreateFileHandle(sourceFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile)
    {
        DWORD fileSize = GetFileSize(hFile.get(), NULL);

        retVal.resize(fileSize);
        DWORD bytesRead;
        HRESULT result = ReadFile(hFile.get(), retVal.data(), fileSize, &bytesRead, FALSE);    
    }
    return retVal;
}

/*
vector<unsigned char> LoadFileContents(std::wstring sourceFileName)
{
    vector<unsigned char> retVal;
    auto hFile = CreateFile(sourceFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD fileSize = GetFileSize(hFile, NULL);

        retVal.resize(fileSize);
        DWORD bytesRead;
        HRESULT result = ReadFile(hFile, retVal.data(), fileSize, &bytesRead, FALSE);
        CloseHandle(hFile);
    }
    return retVal;
}
*/



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    std::map<std::wstring, ComPtr<ID3D10Blob>> shaderMap;

    D3DAppWindow::GoToExeDirectory();
    
    

    if (FAILED(CoInitialize(NULL)))
        return -1;

    std::shared_ptr<D3DAppWindow> appWindow = std::make_shared<D3DAppWindow>(hInstance);
    appWindow->SetShaderSource(L"shaderList.txt");
    appWindow->Init();
    appWindow->RunMessageLoop();

    

    CoUninitialize();

    return 0;
}
