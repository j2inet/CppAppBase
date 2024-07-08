#include "TcpListener.h"
#include <iostream>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

std::wstring getWSAErrorDescription(int error);
void initWSAErrorDescriptionMap();


int TcpListener::WSAStartupResult = -1;
std::map<int, std::wstring> WSAErrorDescriptionMap;
bool WSAErrorDescriptionMapInitialized = false;


void initWSAErrorDescriptionMap() {
	if (WSAErrorDescriptionMapInitialized) {
		return;
	}
	WSAErrorDescriptionMap[WSAEINTR] = L"A blocking operation was interrupted by a call to WSACancelBlockingCall.";
	WSAErrorDescriptionMap[WSAEBADF] = L"The file handle supplied is not valid.";
	WSAErrorDescriptionMap[WSAEACCES] = L"An attempt was made to access a socket in a way forbidden by its access permissions. ";
	WSAErrorDescriptionMap[WSAEFAULT] = L"The system detected an invalid pointer address in attempting to use a pointer argument of a call.";
	WSAErrorDescriptionMap[WSAEINVAL] = L"he requested service provider is invalid. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found. This error is also returned if the service provider returned a version number other than 2.0.";
	WSAErrorDescriptionMap[WSAEMFILE] = L"Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). ";
	WSAErrorDescriptionMap[WSAEWOULDBLOCK] = L"This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket.";
	WSAErrorDescriptionMap[WSAEINPROGRESS] = L"A blocking operation is currently executing. ";
	WSAErrorDescriptionMap[WSAEALREADY] = L"An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.";
	WSAErrorDescriptionMap[WSAENOTSOCK] = L"An operation was attempted on something that is not a socket. ";
	WSAErrorDescriptionMap[WSAEDESTADDRREQ] = L"A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.";
	WSAErrorDescriptionMap[WSAEMSGSIZE] = L"A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.";
	WSAErrorDescriptionMap[WSAEPROTOTYPE] = L"A protocol was specified in the socket function call that does not support the semantics of the socket type requested. F";
	WSAErrorDescriptionMap[WSAENOPROTOOPT] = L"An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.";
	WSAErrorDescriptionMap[WSAEPROTONOSUPPORT] = L"The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.";
	WSAErrorDescriptionMap[WSAESOCKTNOSUPPORT] = L"The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.";
	WSAErrorDescriptionMap[WSAEOPNOTSUPP] = L"The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.";
	WSAErrorDescriptionMap[WSAEPFNOSUPPORT] = L"The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.";
	WSAErrorDescriptionMap[WSAEAFNOSUPPORT] = L"An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). ";
	WSAErrorDescriptionMap[WSAEADDRINUSE] = L"Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. ";
	WSAErrorDescriptionMap[WSAEADDRNOTAVAIL] = L"The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. ";
	WSAErrorDescriptionMap[WSAENETDOWN] = L"A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.";
	WSAErrorDescriptionMap[WSAENETUNREACH] = L"A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.";
	WSAErrorDescriptionMap[WSAENETRESET] = L"The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.";
	WSAErrorDescriptionMap[WSAECONNABORTED] = L"An established connection was aborted by the software in your host machine.";
	WSAErrorDescriptionMap[WSAECONNRESET] = L"An existing connection was forcibly closed by the remote host.";
	WSAErrorDescriptionMap[WSAENOBUFS] = L"An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
	WSAErrorDescriptionMap[WSAEISCONN] = L"A connect request was made on an already connected socket.";
	WSAErrorDescriptionMap[WSAENOTCONN] = L"A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.";
	WSAErrorDescriptionMap[WSAESHUTDOWN] = L"A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.";
	WSAErrorDescriptionMap[WSAETIMEDOUT] = L"A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.";
	WSAErrorDescriptionMap[WSAECONNREFUSED] = L"No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.";
	WSAErrorDescriptionMap[WSAEHOSTDOWN] = L"A socket operation failed because the destination host was down. ";
	WSAErrorDescriptionMap[WSAEHOSTUNREACH] = L"A socket operation was attempted to an unreachable host. ";
	WSAErrorDescriptionMap[WSAEPROCLIM] = L"A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously.";
	WSAErrorDescriptionMap[WSASYSNOTREADY] = L"The underlying network subsystem is not ready for network communication.";
	WSAErrorDescriptionMap[WSAVERNOTSUPPORTED] = L"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.";
	WSAErrorDescriptionMap[WSANOTINITIALISED] = L"A successful WSAStartup call must occur before using this function.";
	WSAErrorDescriptionMap[WSAEDISCON] = L"The connection has been reset. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found.";
	WSAErrorDescriptionMap[WSAENOMORE] = L"No more results can be returned by WSALookupServiceNext.";
	WSAErrorDescriptionMap[WSAECANCELLED] = L"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.";
	WSAErrorDescriptionMap[WSAEINVALIDPROCTABLE] = L"The procedure call table is invalid.";
	WSAErrorDescriptionMap[WSAEINVALIDPROVIDER] = L"The requested service provider is invalid.";
	WSAErrorDescriptionMap[WSAEPROVIDERFAILEDINIT] = L"The requested service provider could not be loaded or initialized.";
	WSAErrorDescriptionMap[WSASYSCALLFAILURE] = L"A system call that should never fail has failed.";
	WSAErrorDescriptionMap[WSASERVICE_NOT_FOUND] = L"No such service is known. The service cannot be found in the specified name space.";
	WSAErrorDescriptionMap[WSATYPE_NOT_FOUND] = L"The specified class was not found.";
	WSAErrorDescriptionMap[WSA_E_NO_MORE] = L"No more results can be returned by WSALookupServiceNext.";
	WSAErrorDescriptionMap[WSA_E_CANCELLED] = L"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.";
	WSAErrorDescriptionMap[WSAEREFUSED] = L"A database query failed because it was actively refused.";
	WSAErrorDescriptionMap[WSAHOST_NOT_FOUND] = L"No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried.";
	WSAErrorDescriptionMap[WSATRY_AGAIN] = L"A nonauthoritative host was not found or a server failure occurred. This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server.";
	WSAErrorDescriptionMap[WSANO_RECOVERY] = L"A nonrecoverable error occurred. This is a generic error code, returned under various conditions.";
	WSAErrorDescriptionMap[WSANO_DATA] = L"The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for.";
	WSAErrorDescriptionMapInitialized = true;
}

std::wstring getWSAErrorDescription(int error) {
	initWSAErrorDescriptionMap();
	auto it = WSAErrorDescriptionMap.find(error);
	if (it != WSAErrorDescriptionMap.end()) {
		return it->second;
	}
	return L"Unknown error";
}

TcpListener::TcpListener(unsigned short port)
{
	InitializeWSA();

	struct std::shared_ptr<addrinfo> result = NULL;
	struct addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	std::string portStr = std::to_string(port);
	addrinfo* resultPtr = NULL;
	int iResult = getaddrinfo(NULL, portStr.c_str(), &hints, &resultPtr);
	result = std::shared_ptr<addrinfo>(resultPtr, [](addrinfo* ptr) { freeaddrinfo(ptr); });
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		CleanupWSA();
		return;
	}
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		CleanupWSA();
		return;
	}
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		CleanupWSA();
		return;
	}

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		CleanupWSA();
	}
}


TcpListener::~TcpListener()
{
	Stop();
	CleanupWSA();
}

void TcpListener::Start()
{
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		CleanupWSA();
		return;
	}
}

void TcpListener::Stop()
{
	if (listenSocket != INVALID_SOCKET) {
		closesocket(listenSocket);
		listenSocket = INVALID_SOCKET;
	}
}
void TcpListener::InitializeWSA()
{
	if (WSAStartupResult == -1) {
		WSADATA wsaData;
		WSAStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (WSAStartupResult != 0) {
			printf("WSAStartup failed: %d\n", WSAStartupResult);
			return;
		}
	}
}

void TcpListener::CleanupWSA()
{
	if (WSAStartupResult == 0) {
		WSACleanup();
		WSAStartupResult = -1;
	}
}

std::shared_ptr<TcpSocket> TcpListener::Accept()
{
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		auto error = WSAGetLastError();
		std::wcerr << L"accept failed: 0x" << std::hex << error << L"(" << std::dec << error << L")" << std::endl << getWSAErrorDescription(error) << std::endl << std::endl;
		return nullptr;
	}
	std::shared_ptr<SOCKET> clientSocketPtr = std::shared_ptr<SOCKET>(new SOCKET(clientSocket), [](SOCKET* ptr) { closesocket(*ptr); });
	std::shared_ptr<TcpSocket> tcpSocket = std::make_shared<TcpSocket>(clientSocketPtr);
	return tcpSocket;
}


TcpSocket::TcpSocket(std::shared_ptr<SOCKET> socket) : TcpSocket(socket, 1024)
{

}
TcpSocket::TcpSocket(std::shared_ptr<SOCKET> socket, size_t bufferSize)
{
	this->buffer.resize(bufferSize);
	this->socket = socket;
}


std::vector<char> TcpSocket::Read(size_t maxRequested)
{
	std::vector<char> result(maxRequested);
	int iResult = recv(*socket, result.data(), maxRequested, 0);
	if (iResult == SOCKET_ERROR) {
		auto error = WSAGetLastError();
		std::wcerr << L"recv failed: 0x" << std::hex << error << L"(" << std::dec << error << L")" << std::endl << getWSAErrorDescription(error) << std::endl << std::endl;
		return std::vector<char>();
	}
	result.resize(iResult);
	return result;
}

std::string TcpSocket::ReadString(size_t maxRequested)
{
	auto data = Read(maxRequested);
	std::string retVal = std::string(data.begin(), data.end());
	return retVal;
}

void TcpSocket::Write(std::vector<char> data)
{
	int iResult = send(*socket, data.data(), data.size(), 0);
	if (iResult == SOCKET_ERROR) {
		auto error = WSAGetLastError();
		std::wcerr << L"send failed: 0x" << std::hex << error << L"(" << std::dec << error << L")" << std::endl << getWSAErrorDescription(error) << std::endl << std::endl;
	}
}

void TcpSocket::WriteString(std::string data)
{
	Write(std::vector<char>(data.begin(), data.end()));
}

TcpSocket::~TcpSocket()
{
	if (socket) {
		closesocket(*socket);
	}
}
