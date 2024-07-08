#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include "TcpListener.h"

 class HttpResponse
{
public:
	HttpResponse(unsigned responseCode);
	virtual void Send(std::shared_ptr<TcpSocket> socket) = 0;
	std::string GetStatusLine();
	std::string GetHeaders();
protected:
	unsigned responseCode;
	std::map<std::string, std::string> headers;
	size_t contentLength;
private:	
};


 class StringHttpResponse : public HttpResponse
 {
 public:
	 StringHttpResponse(unsigned responseCode, std::string responseString);
	 void Send(std::shared_ptr<TcpSocket> socket) override;
 private:
	 std::string responseString;
 };




 class FileHttpResponse : public HttpResponse
 {
 public:
	 FileHttpResponse(unsigned responseCode, std::wstring filePath);
	 void Send(std::shared_ptr<TcpSocket> socket) override;
 private:
	 std::wstring filePath;
 };