#include "common.hlsl"

cbuffer Gauge : register(b6)
{
    float currentST;  //���݂̃X�^�~�i
    float MaxST;      //�ő�X�^�~�i
    float4 baseColor;  //�x�[�X�J���[(��)
    float4 lostColor;  //�X�^�~�i���Ȃ��Ȃ����Ƃ��̐F
}

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f); //�����F�̐ݒ�(��)
    float2 uv = In.TexCoord; //uv���W�̒��S�𒲐�
    float angle = uv.x; //����̊p�x�����肷��

	//�Q�[�W�̊p�x���v�Z(0.0�`1.0)
    float gauge = frac(angle);

    color.rgb += baseColor * step(angle,currentST);
    
	////�Q�[�W��gauge2�𒴂�����lostColor���v�Z
 //   color.rgb += lostColor.rgb * step(gauge2, gauge);

	////�Q�[�W��gauge1��gauge2�̊ԂȂ�diffColor�����Z
 //   color.rgb += diffColor.rgb * (1.0f - step(gauge2, gauge)) * step(gauge1, gauge);

	////�Q�[�W��gauge1�����Ȃ�baseColor�����Z
 //   color.rgb += baseColor.rgb * (1.0f - step(gauge1, gauge));

	////uv���W�̌��_����̋������v�Z(0.0�`1.0)
 //   float dist = length(uv) / 0.5f;

	////�`�悷��͈�(inner�`outer)�ȊO�ł͓����ɂ���
 //   color.a = (1 - step(outer, dist)) * step(inner, dist);
    
    outDiffuse = color;
}