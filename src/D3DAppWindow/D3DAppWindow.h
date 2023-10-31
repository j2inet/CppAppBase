#pragma once

#include "resource.h"
#include "../CppAppBase/AppWindow.h"


class D3DAppWindow : public AppWindow {
public:
	D3DAppWindow(HINSTANCE hInstance);
protected:
	std::wstring GetWindowClassName() { return L"D3DAppWindow"; };
};