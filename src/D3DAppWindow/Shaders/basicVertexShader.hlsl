cbuffer PositioningConstantsBuffer
{
    float scale;
    float2 offset;
    float padding;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut main(float3 position : POSITION, float4 color : COLOR)
{
    VOut output;
    float3 scaledPosition = position * scale;

    output.position = float4(scaledPosition.x + offset.x, scaledPosition.y + offset.y, scaledPosition.z, 1.0);
    output.color = 1 - color;

    return output;
}