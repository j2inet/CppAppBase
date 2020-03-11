#pragma once
#include "AppWindow.h"

class DerivedWindow : public AppWindow {
public:
	DerivedWindow(HINSTANCE hInstance) : AppWindow(hInstance) {

	}

	void Init() override {
		AppWindow::Init();
		CreateButton(TEXT("Test Button"), { 20,20,200,60 });
		CreateLabel(TEXT("Test Label"), { 20,90,200,120 });
	}

	std::wstring GetWindowClassName() override
	{
		return TEXT("DerivedWindow");

	}


};