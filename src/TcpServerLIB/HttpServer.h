#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "TcpListener.h"
#include "Http.h"
#include <chrono>


using RequestTime = std::chrono::time_point<std::chrono::steady_clock>;

class ProcessThread
{
public:
	 ProcessThread()
	{
		startTime = std::chrono::steady_clock::now();
	}

	std::thread thread;
	std::shared_ptr<HttpRequest> request;
	RequestTime startTime;
};

class HttpServer
{
public:
	HttpServer(unsigned short port, std::wstring rootPath);
	void Run();
	void Stop();
	static std::string GetMimeType(std::wstring filePath);
protected:
	std::wstring RequestPathToFilePath(std::wstring requestPath);
	std::wstring RequestPathToFilePath(std::string requestPath);
	std::vector<std::wstring> defaultDocumentList;
	bool isMultithreaded = true;
private:
	std::vector<std::shared_ptr<ProcessThread>> activeRequestList;
	void ProcessRequest(std::shared_ptr<TcpSocket> socket);
	std::shared_ptr<TcpListener> listener;
	std::wstring rootPath;

};