#include "AppWindow.h"


CLASS_REGISTRATION_MAP	 AppWindow::classRegistrationMap;

LRESULT CALLBACK AppWindow_WndPRoc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	AppWindow* appWindow = (AppWindow*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (appWindow)
		return appWindow->WndProc(hWnd, message, wParam, lParam);
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

AppWindow::AppWindow(HINSTANCE hInstance)
{
	this->_hWnd = NULL;
	this->hInstance = hInstance;


}

AppWindow::~AppWindow()
{
	DestroyWindow(this->_hWnd);
}


WNDPROC AppWindow::GetClassWndProc()
{
	return AppWindow_WndPRoc;
}
void AppWindow::Init()
{
	this->CreateAppWindow();
}

HWND AppWindow::CreateButton(std::wstring caption, RECT position, HMENU command, DWORD style )
{	
	LONG& x = position.left;
	LONG& y = position.top;
	auto width = position.right - position.left;
	auto height = position.bottom - position.top;
	HWND hButton = CreateWindow(TEXT("BUTTON"), caption.c_str(), style, x, y, width, height, this->_hWnd, command, this->hInstance, 0);
	return hButton;
}

HWND AppWindow::CreateLabel(std::wstring caption, RECT position)
{
	DWORD style = WS_CHILD | WS_VISIBLE | ES_LEFT;
	LONG& x = position.left;
	LONG& y = position.top;
	auto width = position.right - position.left;
	auto height = position.bottom - position.top;
	HWND hTextbox = CreateWindow(TEXT("STATIC"), caption.c_str(), style, x, y, width, height, this->_hWnd, 0, this->hInstance, 0);
	//SetWindowText(hTextbox, caption.c_str());
	return hTextbox;
}

void AppWindow::InitFailure()
{
	return;
}

RECT AppWindow::ClientRect()
{
	RECT rect;
	GetClientRect(this->_hWnd, &rect);
	return rect;
}

void AppWindow::CreateAppWindow()
{
	this->RegisterClass(this->GetWindowClassName().c_str(), AppWindow_WndPRoc);
	std::wstring windowClass = this->GetWindowClassName();
	this->_hWnd = CreateWindowW(windowClass.c_str(), windowClass.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (this->_hWnd)
		ShowWindow(this->_hWnd, SW_SHOWDEFAULT);
	if (!this->_hWnd)
	{
		DWORD errorCode = GetLastError();
		std::wstringstream sstream;
		sstream << "Last error code " << errorCode << std::endl;
		OutputDebugString(sstream.str().c_str());
	}
	SetWindowLongPtr(this->_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	LONG_PTR l = GetWindowLongPtr(this->_hWnd, GWLP_USERDATA);
}


int AppWindow::GetClientWidth()
{
	auto rect = ClientRect();
	return rect.right - rect.left;

}
int AppWindow::GetClientHeight()
{
	auto rect = ClientRect();
	return rect.bottom - rect.top;
}

void AppWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->_hWnd, &ps);
	EndPaint(this->_hWnd, &ps);
}


LRESULT AppWindow::OnCommand(DWORD wParam, DWORD lParam)
{
	return DefWindowProc(_hWnd, WM_COMMAND, wParam, lParam);
}

void AppWindow::Destroy()
{
	PostQuitMessage(0);
}


int AppWindow::RunMessageLoop()
{
	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INFINITESCROLL));
	MSG msg;
	bool keepRunning = true;
	// Main message loop:

	do {

		if (PeekMessage(&msg, NULL, 0, 0, TRUE))
		{
			//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					keepRunning = false;
			}
		}
		else Idle();

	} while (keepRunning);
	return (int)msg.wParam;
}

void AppWindow::Idle()
{
	Sleep(0);
}

void AppWindow::CloseWindow()
{
	DestroyWindow(this->_hWnd);
	//Destroy();
}


void AppWindow::OnResize(UINT width, UINT height)
{

}


LRESULT CALLBACK AppWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		return this->OnCommand(wParam, lParam);
		break;

	case WM_PAINT:
		this->OnPaint();
		break;
	case WM_CLOSE:
		CloseWindow();
		break;

	case WM_DESTROY:
		this->Destroy();
	case WM_SIZE: {
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		this->OnResize(width, height);
	}
				break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}




std::wstring AppWindow::GetWindowClassName()
{
	std::wstring className = std::wstring(L"AppWindow");
	return className;
}



LRESULT  AppWindow::RegisterClass(std::wstring className, void*)
{
	CLASS_REGISTRATION_MAP::iterator it = classRegistrationMap.find(className);
	if (it == classRegistrationMap.end())
	{
		//This class isn't registered. We can move forward with refistering it. 
		WNDCLASSEXW wcex = { 0 };
		const std::wstring className = this->GetWindowClassName().c_str();

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (this->GetClassWndProc());
		//wcex.cbClsExtra = 0;
		//wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INFINITESCROLL));
		//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.lpszMenuName = 0;
		wcex.lpszClassName = className.c_str();;
		//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		ATOM classAtom = RegisterClassExW(&wcex);
		classRegistrationMap.emplace(GetWindowClassName(), classAtom);
		return classAtom;
	}
}