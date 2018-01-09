#include "Shader_Base.hpp"

// 해의 스크린좌표
float2	g_ScreenSunPos = float2(0.5, 0.5);

// 샘플링 수(속도 저하, But 부드러워짐)
int		g_SampleCnt = 100;

// 빛의 밀도치
float	g_Density = 1.f;

// 빛의 감소치
float	g_IlluminationDecay = 1.f;

// 빛의 방향
float2	g_SunDir = float2(-3.f, -3.f);

// 빛의 세기
float	g_Exposure = 0.5f;

// 스크린 사이즈
float	g_fScreenW = 1024.f;
float	g_fScreenH = 768.f;

// ??
float	g_Decay = 0.9f;


texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_OccTexture;
sampler OccSampler = sampler_state
{
	Texture = g_OccTexture;

	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};


struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

// Defulat
PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	if(vColor.x == 0.f && vColor.y == 0.f && vColor.z == 0.f)
		Out.vColor = vector(1.f, 1.f, 1.f, 0.3f);

	return Out;
}


PS_OUT PS_SKYMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	if(vColor.x == 0.f && vColor.y == 0.f && vColor.z == 0.f)
		Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
	else
		Out.vColor = vector(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_LIGHTMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	// UV를 이동시킬 방향성..
	float2 DeltaTexCoord = In.vTexUV - g_ScreenSunPos;

	// 광선의 밀도치(빛들의 간격)
	DeltaTexCoord *= 1.f / (float)g_SampleCnt * g_Density;

	vector		vOcclusion = tex2D(OccSampler, In.vTexUV);
	

	// 광선의 감소치
	vector		vSample = vector(1.f, 1.f, 1.f, 1.f);

	float2	texCoord = In.vTexUV;
	float	IlluminationDecay = g_IlluminationDecay;

	for(int i = 0; i < g_SampleCnt; ++i)
	{
		texCoord -= DeltaTexCoord;

		// 광선의 방향
		texCoord.x += 1.f / g_fScreenW * g_SunDir.x;
		texCoord.y += 1.f / g_fScreenH * g_SunDir.y;

		vSample = tex2D(OccSampler, texCoord);

		// 감소치 곱해주기
		vSample *= IlluminationDecay;

		vOcclusion += vSample;

		IlluminationDecay *= g_Decay;
	}

	Out.vColor = vColor + (vOcclusion * g_Exposure);

	if(Out.vColor.r == 0.f && Out.vColor.g == 0.f && Out.vColor.b == 0.f)
		Out.vColor.a = 0.f;
	//Out.vColor = vOcclusion * 10.f;
	return Out;
}

technique Default
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};

technique Sky
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_SKYMAIN();
	}
};

technique Light
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_LIGHTMAIN();
	}
};