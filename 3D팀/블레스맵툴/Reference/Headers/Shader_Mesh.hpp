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
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldNormal : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector		vWorldNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vWorldNormal = vWorldNormal;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldNormal : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	
	
	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;

	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

technique Default_Device
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Etc
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
