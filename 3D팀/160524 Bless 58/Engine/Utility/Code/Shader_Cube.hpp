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

		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();

		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
