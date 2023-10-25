#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

cbuffer PlayerColor : register(b8)
{
    float4 color;
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse.xyz = In.Diffuse.xyz;
        outDiffuse = In.Diffuse;
        outDiffuse.a = color.a;
    }
}
