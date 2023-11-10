#pragma enable_d3d11_debug_symbols

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


VSOut main(VSIn input)
{
	VSOut output;
	output.position = float4(input.position, 0.0f);
	output.color = input.color;
	return output;
}

