struct VSIn
{
	float4 position : POSITION;
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
	output.position = input.position;
	output.color = input.color;
	return output;
}

