// CppAppBase.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AppWindow.h"
#include "DerivedWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if(FAILED(CoInitialize(NULL)))
        return -1;

    std::shared_ptr<AppWindow> appWindow = std::make_shared<DerivedWindow>(hInstance);
    appWindow->Init();
    appWindow->RunMessageLoop();

    CoUninitialize();

    return 0;
}
