#include "HttpResponse.h"

std::map<std::string, std::string> mimeMap;
bool mimeMapInitialized = false;


void initMimeMap()
{
	if (mimeMapInitialized)
	{
		return;
	}
	mimeMap[".html"] = "text/html";
	mimeMap[".htm"] = "text/html";
	mimeMap[".css"] = "text/css";
	mimeMap[".js"] = "text/javascript";
	mimeMap[".jpg"] = "image/jpeg";
	mimeMap[".jpeg"] = "image/jpeg";
	mimeMap[".png"] = "image/png";
	mimeMap[".gif"] = "image/gif";
	mimeMap[".bmp"] = "image/bmp";
	mimeMap[".ico"] = "image/x-icon";
	mimeMap[".svg"] = "image/svg+xml";
	mimeMap[".mp3"] = "audio/mpeg";
	mimeMap[".wav"] = "audio/wav";
	mimeMap[".mp4"] = "video/mp4";
	mimeMap[".webm"] = "video/webm";
	mimeMap[".ogv"] = "video/ogg";
	mimeMap[".pdf"] = "application/pdf";
	mimeMap[".zip"] = "application/zip";
	mimeMap[".tar"] = "application/x-tar";
	mimeMap[".gz"] = "application/x-gzip";
	mimeMap[".rar"] = "application/x-rar-compressed";
	mimeMap[".7z"] = "application/x-7z-compressed";
	mimeMap[".exe"] = "application/x-msdownload";
	mimeMap[".doc"] = "application/msword";
	mimeMap[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimeMap[".xls"] = "application/vnd.ms-excel";
	mimeMap[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mimeMap[".ppt"] = "application/vnd.ms-powerpoint";
	mimeMap[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimeMap[".odt"] = "application/vnd.oasis.opendocument.text";
	mimeMap[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mimeMap[".odp"] = "application/vnd.oasis.opendocument.presentation";
	mimeMap[".xml"] = "application/xml";
	mimeMap[".json"] = "application/json";
	mimeMap[".torrent"] = "application/x-bittorrent";
	mimeMap[".swf"] = "application/x-shockwave-flash";
	mimeMapInitialized = true;
}

std::string GetMimeType(std::wstring filePath)
{
	initMimeMap();
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::string shortExtension(extension.begin(), extension.end());
	for (char& c : shortExtension)
	{
		c = std::tolower(c);
	}
	auto it = mimeMap.find(shortExtension);
	if (it != mimeMap.end())
	{
		return it->second;
	}
	return "";
}



HttpResponse::HttpResponse(unsigned responseCode)
{
	this->responseCode = responseCode;
	this->contentLength = 0;
}

std::string HttpResponse::GetStatusLine()
{
	std::string statusLine = "HTTP/1.1 " + std::to_string(responseCode) + " ";
	switch (responseCode)
	{
	case 200:
		statusLine += "OK";
		break;
	case 404:
		statusLine += "Not Found";
		break;
	default:
		statusLine += "Unknown";
		break;
	}
	return statusLine + "\r\n";
}


std::string HttpResponse::GetHeaders()
{
	std::string headersString;
	for (auto header : headers)
	{
		headersString += header.first + ": " + header.second + "\r\n";
	}
	return headersString;
}

StringHttpResponse::StringHttpResponse(unsigned responseCode, std::string responseString) : HttpResponse(responseCode)
{
	this->responseString = responseString;
	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = std::to_string(responseString.size());
	contentLength = responseString.size();
}

void StringHttpResponse::Send(std::shared_ptr<TcpSocket> socket)
{
	std::string header = GetStatusLine() + GetHeaders() + "\r\n\r\n";
	socket->WriteString(header + responseString);
}

FileHttpResponse::FileHttpResponse(unsigned responseCode, std::wstring filePath) :HttpResponse(responseCode)
{
	this->filePath = filePath;
	std::ifstream file(filePath, std::ios::binary);
	if (file.is_open())
	{
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::end);
		fileSize = file.tellg() - fileSize;
		file.seekg(0, std::ios::beg);
		contentLength = fileSize;
	} 
	else
	{
		responseCode = 404;
	}
}

void FileHttpResponse::Send(std::shared_ptr<TcpSocket> socket)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.is_open())
	{
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::end);
		fileSize = file.tellg() - fileSize;
		file.seekg(0, std::ios::beg);
		contentLength = fileSize;
		std::string mimeType = GetMimeType(filePath);
		std::wcout << L"Sending file [" << filePath << L"] with mime type [";
		std::cout << mimeType;
		std::wcout << L"]" << std::endl;
		headers["Connection"] = "Keep-Alive: max=0";
		if (mimeType != "")
		{
			headers["Content-Type"] = mimeType;
		}
		std::string header = GetStatusLine() + GetHeaders() + "\r\n";
		socket->WriteString(header);
		std::vector<char> buffer(1024);
		while (!file.eof())
		{
			file.read(buffer.data(), buffer.size());
			size_t bytesRead = file.gcount();
			socket->Write(std::vector<char>(buffer.begin(), buffer.begin() + bytesRead));
		}
		std::wcout << L"File sent" << std::endl;
	}
	else
	{
		std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
		socket->WriteString(response);
	}
}