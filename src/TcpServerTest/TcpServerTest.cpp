// TcpServerTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>


#include "HttpServer.h"

std::shared_ptr<HttpServer> server;

BOOL ControlCHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		std::cout << "Ctrl-C event\n\n";
		server->Stop();
		return(TRUE);

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
		std::cout << "Ctrl-Close event\n\n";
		return(TRUE);

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		std::cout << "Ctrl-Break event\n\n";
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		std::cout << "Ctrl-Logoff event\n\n";
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		std::cout << "Ctrl-Shutdown event\n\n";
		return FALSE;

	default:
		return FALSE;
	}
}

int main()
{



	SetConsoleCtrlHandler(ControlCHandler, TRUE);
	server = std::make_shared<HttpServer>(8090, L"");
	server->Run();
}