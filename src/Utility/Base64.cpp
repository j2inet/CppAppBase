#include "Base64.h"

//Add to  https://stackoverflow.com/questions/15595117/how-do-i-provide-custom-output-streams-in-c
// Add to https://codereview.stackexchange.com/questions/167321/custom-output-stream-that-acts-like-stdcout
//https://stackoverflow.com/questions/11618503/no-appropriate-default-constructor-for-stdbasic-ostream

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64_char(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

static inline bool is_whitespace(unsigned char c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

std::string base64_encode(const std::string& in) {
	std::string out;
	int val = 0, valb = -6;
	for (unsigned char c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}


Base64EncoderStreamBuf::Base64EncoderStreamBuf(std::ostream& targetStream) :m_outputStream(targetStream)
{
	bytesWritten = 0;
}

/*
int Base64EncoderStreamBuf::overflow(int c)
{
	if (c != EOF)
	{
		m_stream_fifo.push(c);
	}
	return c;
}
*/


std::streamsize  Base64EncoderStreamBuf::xsputn(const char* s, std::streamsize n)
{
	for (int i = 0; i < n; i++)
	{
		m_stream_fifo.push(s[i]);
	}
	encode(false);
	return n;

}


int Base64EncoderStreamBuf::sync()
{
	encode(true);

	return 0;
}






/*

std::streamsize  Base64EncoderStream::xsputn(const char* s, std::streamsize n)
{
	for (int i = 0; i < n; i++)
	{
		m_stream_fifo.push(s[i]);
	}
	encode(false);
	return n;
}
*/



Base64EncoderStream::Base64EncoderStream(std::ostream& stream):
	std::ostream(new Base64EncoderStreamBuf(stream))
{
}


void Base64EncoderStreamBuf::encode(bool flush)
{
	if (m_stream_fifo.size() == 0)
	{
		return;
	}
	/*
	0: take top 6 bits of first byte
	1: take bottom 2 bits of first byte and top 4 bits of second byte
	2: take bottom 4 bits of second byte and top 2 bits of third byte
	3: take bottom 6 bits of third byte
	*/

	while (m_stream_fifo.size() >= 3)
	{
		char firstByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		char secondByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		char thirdByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		m_outputStream << base64_chars[(firstByte & 0xFC) >> 2];
		m_outputStream << base64_chars[((firstByte & 0x03) << 4) + ((secondByte & 0xF0) >> 4)];
		m_outputStream << base64_chars[((secondByte & 0x0F) << 2) + ((thirdByte & 0xC0) >> 6)];
		m_outputStream << base64_chars[thirdByte & 0x3F];
		bytesWritten += 4;
		if (bytesWritten % 76 == 0)
		{
			m_outputStream << "\n";
		}
	}
	if (flush)
	{
		encodePartial();
	}
}


void Base64EncoderStreamBuf::encodePartial()
{
	if (m_stream_fifo.size() > 3)
	{
		encode(true);
	}
	if (m_stream_fifo.size() == 0)
	{
		return;
	}
	if (m_stream_fifo.size() == 1)
	{
		char firstByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		m_outputStream << base64_chars[(firstByte & 0xFC) >> 2];
		m_outputStream << base64_chars[(firstByte & 0x03) << 4];
		m_outputStream << "==";
		bytesWritten += 4;
	}
	else if (m_stream_fifo.size() == 2)
	{
		char firstByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		char secondByte = m_stream_fifo.front();
		m_stream_fifo.pop();
		m_outputStream << base64_chars[(firstByte & 0xFC) >> 2];
		m_outputStream << base64_chars[((firstByte & 0x03) << 4) + ((secondByte & 0xF0) >> 4)];
		m_outputStream << base64_chars[(secondByte & 0x0F) << 2];
		m_outputStream << "=";
		bytesWritten += 4;
	}
}





Base64DecoderStreamBuf::Base64DecoderStreamBuf(std::istream& targetStream) :m_inputStream(targetStream)
{
}

int Base64DecoderStreamBuf::underflow()
{
	if (m_stream_fifo.size())
	{
		auto retVal = m_stream_fifo.front();
		m_stream_fifo.pop();
		return std::char_traits<char>::to_int_type(retVal);
	}

	if(m_inputStream.eof())
	{
		std::char_traits<char>::eof();
	}


	std::vector<char> byteBuffer;
	std::vector<char> decodedBytes;
	while (byteBuffer.size() < 4)
	{
		char c = m_inputStream.get();
		if (c == EOF)
		{
			throw std::runtime_error("Invalid character in base64 stream");
		}
		if (is_base64_char(c))
		{
			byteBuffer.push_back(c);
		}
		else if (is_whitespace(c))
		{
			//do nothing with whitespace
		}
		else if (c == '=')
		{
			byteBuffer.push_back(c);
		}
		else
		{
			throw std::runtime_error("Invalid character in base64 stream");
		}
	}

	m_stream_fifo.push((base64_chars.find(byteBuffer[0]) << 2) + (base64_chars.find(byteBuffer[1]) >> 4));
	if (byteBuffer[2] != '=')
	{
		m_stream_fifo.push(((base64_chars.find(byteBuffer[1]) & 0x0F) << 4) + (base64_chars.find(byteBuffer[2]) >> 2));
	}
	if (byteBuffer[3] != '=')
	{
		m_stream_fifo.push(((base64_chars.find(byteBuffer[2]) & 0x03) << 6) + base64_chars.find(byteBuffer[3]));
	}

	auto retVal = m_stream_fifo.front();
	m_stream_fifo.pop();
	return std::char_traits<char>::to_int_type(retVal);
}


Base64DecoderStream::Base64DecoderStream(std::istream& stream ): std::istream(new Base64DecoderStreamBuf(stream))
{

}