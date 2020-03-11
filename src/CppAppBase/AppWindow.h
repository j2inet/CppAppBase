#pragma once
#include "framework.h"
#include <sstream>
#include <iostream>
#include <wrl/client.h>
#include <memory>

static LRESULT CALLBACK AppWindow_WndPRoc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef LRESULT(*ClassWndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef std::map<std::wstring, ATOM> CLASS_REGISTRATION_MAP;


class AppWindow : public std::enable_shared_from_this<AppWindow> {
public:
	AppWindow(HINSTANCE hInstance);
	~AppWindow();
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Init();
	RECT ClientRect();
	virtual int RunMessageLoop();
protected:
	HWND _hWnd;
	HINSTANCE hInstance;
	LRESULT  RegisterClass(std::wstring, void* windowProcedure);
	void CreateAppWindow();
	HWND CreateButton(std::wstring caption, RECT position, HMENU command=0, DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
	HWND CreateLabel(std::wstring caption, RECT position);
	virtual void Idle();
	virtual void InitFailure();
	virtual void OnPaint();
	virtual LRESULT OnCommand(DWORD wParam, DWORD lParam);
	virtual void Destroy();
	virtual void CloseWindow();
	virtual WNDPROC GetClassWndProc();
	virtual std::wstring GetWindowClassName() = 0;
	virtual void OnResize(UINT width, UINT height);
private:
	static CLASS_REGISTRATION_MAP	 classRegistrationMap;
	std::wstring windowClass;
};
