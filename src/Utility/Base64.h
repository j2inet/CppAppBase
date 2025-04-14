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
	int sync();
	virtual char* egptr() { 
		if (bufferSize == 0) return nullptr;
		if (bufferIndex >= bufferSize) return nullptr;
		return &bufferedCharacters[bufferIndex];
	}
protected:
	///Returns a pointer to the first element of the array with the portion of the controlled input sequence that is currently buffered.
	virtual char* eback() const;
	/// <summary>
	/// Returns a pointer to the current element of the controlled input sequence (i.e., the "get pointer").
	/// </summary>
	/// <returns>A pointer to the current element in the controlled input sequence.</returns>
	virtual char* gptr() const;
	/// <summary>
	/// Returns a pointer to the element just past the last element of the array with the portion of the controlled input sequence that is currently buffered.
	//  Member functions can access this array directly; It is described by the pointers returned by the following protected member functions :
	/// </summary>
	/// <returns></returns>
	virtual char* egptr() const;


	void gbump(int n);

private:
	int bufferIndex = 0;;
	void decode();
     std::istream& m_inputStream;
	 std::shared_ptr<char[]> bufferedCharacters;
	 int bufferSize = 0;

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