#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f); //初期色の設定(黒)
    
    outDiffuse = color;
}