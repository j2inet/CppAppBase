#include "HttpServer.h"
#include "HttpResponse.h"
#include <thread>




std::wstring GetCurrentPath()
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetModuleFileName(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
	std::wstring fullPath = std::wstring(buffer.data());
	std::wstring::size_type pos = fullPath.find_last_of(L"\\/");
	return fullPath.substr(0, pos);
}

std::wstring HttpServer::RequestPathToFilePath(std::string requestPath)
{
	std::wstring wrequestPath(requestPath.begin(), requestPath.end());
	return RequestPathToFilePath(wrequestPath);
}




std::wstring HttpServer::RequestPathToFilePath( std::wstring requestPath)
{
	auto replacedPath = requestPath;
	size_t pos = 0;
	while ((pos = replacedPath.find(L'/', pos)) != std::string::npos) {
		replacedPath.replace(pos, 1, L"\\");
		pos += 1;
	}
	std::wstring fullPath = rootPath + replacedPath;
	auto lastChar = fullPath[fullPath.size() - 1];
	if (fullPath[fullPath.size() - 1] == L'\\')
	{
		for (auto document : defaultDocumentList)
		{
			std::wstring documentPath = fullPath + document;
			std::ifstream file(documentPath);
			if (file.is_open())
			{
				fullPath = documentPath;
				break;
			}
		}
	}
	return fullPath;
}

HttpServer::HttpServer(unsigned short port, std::wstring rootPath)
	: rootPath(rootPath)
{
	defaultDocumentList = { L"index.html", L"index.htm" };
	if (rootPath.empty())
	{
		this->rootPath = GetCurrentPath();
	}
	listener = std::make_shared<TcpListener>(port);
}

void HttpServer::ProcessRequest(std::shared_ptr<TcpSocket> socket)
{
	std::string stringBuffer = "";

	do {
		auto requestString = socket->ReadString(1024);
		stringBuffer += requestString;
	} while (stringBuffer.find("\r\n\r\n") == std::string::npos);


	auto request = std::make_shared<HttpRequest>(stringBuffer);
	auto path = request->GetDecodedPath();
	if (path.find("..") != std::string::npos)
	{
		std::string response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
		socket->WriteString(response);
		return ;
	}
	std::wcout << L"Incoming request for [";
	std::cout << path;
	std::wcout << L"]" << std::endl;
	std::wstring wpath = std::wstring(path.begin(), path.end());
	std::wstring fullPath = RequestPathToFilePath(path);
	std::wcout << L"Full path is [" << fullPath << L"]" << std::endl;


	std::ifstream file(fullPath, std::ios::binary);

	std::shared_ptr<HttpResponse> httpResponse = std::make_shared<FileHttpResponse>(200, fullPath);
	httpResponse->Send(socket);
	return ;
	/*
	if (file.is_open())
	{
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::end);
		fileSize = file.tellg() - fileSize;
		file.seekg(0, std::ios::beg);
		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(fileSize) + "\r\n\r\n";
		socket->WriteString(response);
		while (!file.eof())
		{
			std::vector<char> buffer(4096);
			file.read(buffer.data(), buffer.size());
			size_t bytesRead = file.gcount();
			socket->Write(std::vector<char>(buffer.begin(), buffer.begin() + bytesRead));
		}
		return ;
	}
	std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
	socket->WriteString(response);
	*/
}

void HttpServer::Stop()
{
	listener->Stop();
}

void HttpServer::Run()
{
	listener->Start();
	while (true)
	{
		std::shared_ptr<TcpSocket> socket = listener->Accept();
		if (isMultithreaded)
		{
			std::shared_ptr<ProcessThread> thread = std::make_shared<ProcessThread>();
			std::thread t([this, socket]() { ProcessRequest(socket); });
			thread->thread = std::move(t);
			activeRequestList.push_back(thread);
		}
		else
		{
			ProcessRequest(socket);
		}
	}
}

