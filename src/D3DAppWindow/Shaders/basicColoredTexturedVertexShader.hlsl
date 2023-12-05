#pragma enable_d3d11_debug_symbols

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD; // texture coordinates  
    
};


VOut main(
    float3 position : POSITION, 
    float4 color:COLOR,
    float2 texcoord : TEXCOORD)
{
    VOut output;
    output.texcoord = texcoord;
    output.position = float4(position, 1.0);
    output.color = color;
    return output;
 }