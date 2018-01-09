#include "Shader_Base.hpp"

float		g_fTime;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture;
sampler FilterSampler = sampler_state
{
	Texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
};



struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;

};

VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = In.vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
		
	vector vFilter = tex2D(FilterSampler, In.vTexUV);

	if(vFilter.x > g_fTime)
		Out.vColor = tex2D(BaseSampler, In.vTexUV);


	return Out;
}

PS_OUT PS_BackMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.rgb = Out.vColor.x;

	return Out;
}

technique Default_Device
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_BackMAIN();
	}

	pass Default
	{
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;


		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
