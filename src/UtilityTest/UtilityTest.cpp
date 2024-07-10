// UtilityTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Base64.h"

int main()
{
    Base64EncoderStream encoder(std::cout);
    encoder << "test" << std::flush;

}

