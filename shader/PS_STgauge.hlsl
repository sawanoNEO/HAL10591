#include "common.hlsl"

cbuffer Gauge : register(b6)
{
    float currentST;  //現在のスタミナ
    float MaxST;      //最大スタミナ
    float4 baseColor;  //ベースカラー(緑)
    float4 lostColor;  //スタミナがなくなったとこの色
}

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f); //初期色の設定(黒)
    float2 uv = In.TexCoord; //uv座標の中心を調整
    float angle = uv.x; //消費の角度を決定する

	//ゲージの角度を計算(0.0～1.0)
    float gauge = frac(angle);

    color.rgb += baseColor * step(angle,currentST);
    
	////ゲージがgauge2を超えたらlostColorを計算
 //   color.rgb += lostColor.rgb * step(gauge2, gauge);

	////ゲージがgauge1とgauge2の間ならdiffColorを加算
 //   color.rgb += diffColor.rgb * (1.0f - step(gauge2, gauge)) * step(gauge1, gauge);

	////ゲージがgauge1未満ならbaseColorを加算
 //   color.rgb += baseColor.rgb * (1.0f - step(gauge1, gauge));

	////uv座標の原点からの距離を計算(0.0～1.0)
 //   float dist = length(uv) / 0.5f;

	////描画する範囲(inner～outer)以外では透明にする
 //   color.a = (1 - step(outer, dist)) * step(inner, dist);
    
    outDiffuse = color;
}