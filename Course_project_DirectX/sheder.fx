//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// ����������� ������
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);		// ����� ��������
SamplerState samLinear : register(s0);	// ����� �������

cbuffer ConstantBuffer : register(b0)
{
	matrix World;			// ������� ����
	matrix View;			// ������� ����
	matrix Projection;		// ������� ��������
	float4 vLightDir[6];	// ����������� ��������� �����
	float4 vLightColor[6];	// ���� ��������� �����
	float4 vOutputColor;	// �������� ����
}


//--------------------------------------------------------------------------------------
struct VS_INPUT					// �������� ������ ���������� �������
{
	float4 Pos : POSITION;		// ������� �� X, Y, Z
	float2 Tex : TEXCOORD0;		// ���������� �������� �� tu, tv
	float3 Norm : NORMAL;		// ������� �� X, Y, Z
};

struct PS_INPUT					// �������� ������ ����������� �������
{
	float4 Pos : SV_POSITION;	// ������� ������� � �������� (��������)
	float2 Tex : TEXCOORD0;		// ���������� �������� �� tu, tv
	float3 Norm : TEXCOORD1;	// ������������� ������� ������� �� tu, tv
};


//--------------------------------------------------------------------------------------
// ��������� ������
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(input.Norm, World);
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// ���������� ������ ��� ����
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;

	// ���������� ������������ ������� �� ���� ���������� �����
	for (int i = 0; i<4; i++)
	{
		finalColor += saturate(dot((float3)vLightDir[i], input.Norm) * vLightColor[i]);
	}
	finalColor.a = 1;
	finalColor *= txDiffuse.Sample(samLinear, input.Tex);

	return finalColor;
}


//--------------------------------------------------------------------------------------
// ���������� ������ ��� ���������� �����
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}
