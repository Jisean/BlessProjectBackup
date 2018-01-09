#include "Shader_Base.hpp"

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	vector		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float3			vTexUV : TEXCOORD0;

};

struct PS_OUT
{
	vector			vColor : COLOR0;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	
	pass Default
	{
		Lighting = false;
		CullMode = cw;
		Zwriteenable = false;
		ZEnable = false;

		// ���̴��ڵ带 �����ϱ����� ������ �Լ��� ����
		// ���� �� � �������� ������ �������� ����
		VertexShader = compile vs_3_0 VS_MAIN();

		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
