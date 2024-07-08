#pragma once
#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>


std::wstring getWSAErrorDescription(int error);
class TcpSocket;
class TcpListener;



class TcpListener
{
	
public:
	friend class TcpSocket;
	TcpListener(unsigned short port);
	~TcpListener();
	std::shared_ptr<TcpSocket> Accept();
	void Start();
	void Stop();
private:
	SOCKET listenSocket = INVALID_SOCKET;
	static int WSAStartupResult;
	static void InitializeWSA();
	static void CleanupWSA();
};

class TcpSocket
{
	friend class TcpListener;
public:
	TcpSocket(std::shared_ptr<SOCKET> socket);
	TcpSocket(std::shared_ptr<SOCKET> socket, size_t bufferSize);
	std::vector<char> Read(size_t maxRequested);
	std::string ReadString(size_t maxRequested);
	void Write(std::vector<char> data);
	void WriteString(std::string data);
	~TcpSocket();
private:



	std::shared_ptr<SOCKET> socket;
	std::vector<char> buffer;
};