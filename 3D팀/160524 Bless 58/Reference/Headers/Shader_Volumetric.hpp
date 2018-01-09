#include "Shader_Base.hpp"

// ���� ��ũ����ǥ
float2	g_ScreenSunPos = float2(0.5, 0.5);

// ���ø� ��(�ӵ� ����, But �ε巯����)
int		g_SampleCnt = 100;

// ���� �е�ġ
float	g_Density = 1.f;

// ���� ����ġ
float	g_IlluminationDecay = 1.f;

// ���� ����
float2	g_SunDir = float2(-3.f, -3.f);

// ���� ����
float	g_Exposure = 0.5f;

// ��ũ�� ������
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

	// UV�� �̵���ų ���⼺..
	float2 DeltaTexCoord = In.vTexUV - g_ScreenSunPos;

	// ������ �е�ġ(������ ����)
	DeltaTexCoord *= 1.f / (float)g_SampleCnt * g_Density;

	vector		vOcclusion = tex2D(OccSampler, In.vTexUV);
	

	// ������ ����ġ
	vector		vSample = vector(1.f, 1.f, 1.f, 1.f);

	float2	texCoord = In.vTexUV;
	float	IlluminationDecay = g_IlluminationDecay;

	for(int i = 0; i < g_SampleCnt; ++i)
	{
		texCoord -= DeltaTexCoord;

		// ������ ����
		texCoord.x += 1.f / g_fScreenW * g_SunDir.x;
		texCoord.y += 1.f / g_fScreenH * g_SunDir.y;

		vSample = tex2D(OccSampler, texCoord);

		// ����ġ �����ֱ�
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
		// ���̴��ڵ带 �����ϱ����� ������ �Լ��� ����
		// ���� �� � �������� ������ �������� ����
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

		// ���̴��ڵ带 �����ϱ����� ������ �Լ��� ����
		// ���� �� � �������� ������ �������� ����
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