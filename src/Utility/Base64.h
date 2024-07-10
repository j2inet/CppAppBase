#pragma once
#include <iostream>
#include <string>
#include <streambuf>
#include <vector>
#include <queue>



class Base64EncoderStreamBuf : public std::streambuf
{
public:
Base64EncoderStreamBuf(std::ostream& targetStream);
	//int overflow(int c);
	std::streamsize  xsputn(const char* s, std::streamsize n);
	int sync() override;
private:
	
	
	std::ostream& m_outputStream;
	std::queue<char> m_stream_fifo;
	size_t bytesWritten;
	void encode(bool flush = false);
	void encodePartial();

};



class Base64EncoderStream : public std::ostream
{
public:
	Base64EncoderStream(std::ostream& stream = std::cout);
	//int overflow(int c) ;
	//std::streamsize  xsputn(const char* s, std::streamsize n) ;
	
private:	
	size_t streamSize = 0;
	

};
