#include "common.hlsl"

//πの定義
static const float pi = 3.1415926535f;

cbuffer Polar : register(b5)
{
    float gauge1; //ゲージの下限値
    float gauge2; //ゲージの上限値
    float inner; //描画する内側の円の半径
    float outer; //描画する外側の円の半径
    float4 baseColor; //ベースカラー
    float4 diffColor; //ゲージがgauge1とgauge2の間の時の色
    float4 lostColor; //ゲージがgauge2を超えた時の色

}

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f); //初期色の設定(黒)
    float2 uv = In.TexCoord - 0.5f; //uv座標の中心を調整
    float angle = atan2(uv.y, uv.x); //極座標系での角度を計算

	//角度をπ/2だけずらす
    angle += pi / 2.0f;

	//ゲージの角度を計算(0.0～1.0)
    float gauge = frac(angle * 0.5f / -pi);

	//ゲージがgauge2を超えたらlostColorを計算
    color.rgb += lostColor.rgb * step(gauge2, gauge);

	//ゲージがgauge1とgauge2の間ならdiffColorを加算
    color.rgb += diffColor.rgb * (1.0f - step(gauge2, gauge)) * step(gauge1, gauge);

	//ゲージがgauge1未満ならbaseColorを加算
    color.rgb += baseColor.rgb * (1.0f - step(gauge1, gauge));

	//uv座標の原点からの距離を計算(0.0～1.0)
    float dist = length(uv) / 0.5f;

	//描画する範囲(inner～outer)以外では透明にする
    color.a = (1 - step(outer, dist)) * step(inner, dist);
    
    outDiffuse = color;
}