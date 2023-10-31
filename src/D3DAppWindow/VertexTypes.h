#pragma once

#include "framework.h"



struct RGBA_COLOR
{
	FLOAT r;
	FLOAT g;
	FLOAT b;
	FLOAT a;
};


struct VERTEX_COLORED
{
	FLOAT X, Y, Z;
	RGBA_COLOR color;
};


extern D3D11_INPUT_ELEMENT_DESC VERTEX_COLORED_DESCRIPTION[];