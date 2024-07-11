#pragma once
#include <iostream>
#include <string>
#include <streambuf>
#include <vector>
#include <queue>


#pragma region Base64 Encoder
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
#pragma endregion

#pragma region Base64 Decoder
class Base64DecoderStreamBuf : public std::streambuf
{
public:
	Base64DecoderStreamBuf(std::istream& targetStream);	
	int underflow() override;
	
private:
	void decode();
     std::istream& m_inputStream;
	 std::queue<char> m_stream_fifo;

	 size_t bytesRead = 0;
	 bool EOFReached = false;
 };

class Base64DecoderStream : public std::istream
{
public:
	Base64DecoderStream(std::istream& stream = std::cin);
private:

 };


#pragma endregion