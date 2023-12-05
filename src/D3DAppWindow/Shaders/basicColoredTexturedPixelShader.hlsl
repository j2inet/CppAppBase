#pragma enable_d3d11_debug_symbols

SamplerState ss;
Texture2D Texture;
    

float4 main(
    float4 position : SV_POSITION,
    float4 color : COLOR,
    float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 textureColor = Texture.Sample(ss, texcoord);
    float4 endColor = mul(color, textureColor);
    float cz = color[2] * textureColor[2];
    float cw = color[3] * textureColor[3];
    float4 returnColor;
    returnColor.r = color.r * textureColor.r;
    returnColor.g = color.g * textureColor.g;
    returnColor.b = color.b * textureColor.b;
    returnColor.a = color.a * textureColor.a;
    return returnColor;
}