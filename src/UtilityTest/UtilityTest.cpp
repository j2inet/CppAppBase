// UtilityTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "Base64.h"

int main()
{

    std::string result = "";
    
    {
        std::stringstream ss;
        Base64EncoderStream encoder(ss);
        encoder << "test";// << std::flush;
        encoder.flush();
        result = ss.str();
        //std::cout << result << std::endl;
    }

    std::stringstream ss(result);
    Base64DecoderStream decoder(ss);
    while (decoder.eof() == false)
    {
		char c;
		c = decoder.get();
        std::cout.put(c);
        std::cout.flush();
		
	}

}

