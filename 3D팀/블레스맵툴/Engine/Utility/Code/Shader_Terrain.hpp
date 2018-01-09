#include "Shader_Base.hpp"

float		g_fUVMount;

texture		g_BaseTexture; //배경 텍스쳐

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

///////////////////////////////////////////////////////////////////
texture		g_DestTexture;//첫 스플래팅 타일 텍스쳐

sampler DestSampler = sampler_state 
{
	Texture = g_DestTexture;
	minfilter = linear;
	magfilter = linear;
};

texture		g_DestTexture2;//두번째 스플래팅 타일 텍스쳐

sampler DestSampler2 = sampler_state 
{
	Texture = g_DestTexture2;
	minfilter = linear;
	magfilter = linear;
};



texture		g_DestTexture3;//세번째 스플래팅 타일 텍스쳐

sampler DestSampler3 = sampler_state 
{
	Texture = g_DestTexture3;
	minfilter = linear;
	magfilter = linear;
};

texture		g_DestTexture4;//네번째 스플래팅 타일 텍스쳐

sampler DestSampler4 = sampler_state 
{
	Texture = g_DestTexture4;
	minfilter = linear;
	magfilter = linear;
};

texture		g_DestTexture5;//다섯번째 스플래팅 타일 텍스쳐

sampler DestSampler5 = sampler_state 
{
	Texture = g_DestTexture5;
	minfilter = linear;
	magfilter = linear;
};

texture		g_DestTexture6;//여섯번째 스플래팅 타일 텍스쳐

sampler DestSampler6 = sampler_state 
{
	Texture = g_DestTexture6;
	minfilter = linear;
	magfilter = linear;
};

texture		g_DestTexture7;//일곱번째 스플래팅 타일 텍스쳐

sampler DestSampler7 = sampler_state 
{
	Texture = g_DestTexture7;
	minfilter = linear;
	magfilter = linear;
};

///////////////////////////////////////////////////////////////////


texture		g_FilterTexture;// 첫번째 스플래팅 팔렛트

sampler FilterSampler = sampler_state
{
	Texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture2;// 2번째 스플래팅 팔렛트

sampler FilterSampler2 = sampler_state
{
	Texture = g_FilterTexture2;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture3;// 3번째 스플래팅 팔렛트

sampler FilterSampler3 = sampler_state
{
	Texture = g_FilterTexture3;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture4;// 4번째 스플래팅 팔렛트

sampler FilterSampler4 = sampler_state
{
	Texture = g_FilterTexture4;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture5;// 5번째 스플래팅 팔렛트

sampler FilterSampler5 = sampler_state
{
	Texture = g_FilterTexture5;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture6;// 6번째 스플래팅 팔렛트

sampler FilterSampler6 = sampler_state
{
	Texture = g_FilterTexture6;
	minfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture7;// 7번째 스플래팅 팔렛트

sampler FilterSampler7 = sampler_state
{
	Texture = g_FilterTexture7;
	minfilter = linear;
	magfilter = linear;
};

/////////////////////////////////////////////////////////////////////

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
	vector		vWorldPos : TEXCOORD1;	
	vector		vWorldNormal : TEXCOORD2;
	vector		vProjPos : TEXCOORD3;
};

VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector		vWorldNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vTexUV = In.vTexUV / 8.f;
	Out.vWorldNormal = vWorldNormal;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldPos : TEXCOORD1;	
	vector			vWorldNormal : TEXCOORD2;
	vector			vProjPos : TEXCOORD3;
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

	vector		vFilter[7];

	vFilter[0] = tex2D(FilterSampler, In.vTexUV);
	vFilter[1] = tex2D(FilterSampler2, In.vTexUV);
	vFilter[2] = tex2D(FilterSampler3, In.vTexUV);
	vFilter[3] = tex2D(FilterSampler4, In.vTexUV);
	vFilter[4] = tex2D(FilterSampler5, In.vTexUV);
	vFilter[5] = tex2D(FilterSampler6, In.vTexUV);
	vFilter[6] = tex2D(FilterSampler7, In.vTexUV);

	vector		vColor[8];

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fUVMount);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fUVMount);
	vColor[2] = tex2D(DestSampler2, In.vTexUV * g_fUVMount);
	vColor[3] = tex2D(DestSampler3, In.vTexUV * g_fUVMount);
	vColor[4] = tex2D(DestSampler4, In.vTexUV * g_fUVMount);
	vColor[5] = tex2D(DestSampler5, In.vTexUV * g_fUVMount);
	vColor[6] = tex2D(DestSampler6, In.vTexUV * g_fUVMount);
	vColor[7] = tex2D(DestSampler7, In.vTexUV * g_fUVMount);

	vector		v1Color = vFilter[0] * vColor[0] + (1.f - vFilter[0]) * vColor[1];
	vector		v2Color, v3Color, v4Color, v5Color, v6Color, vResultColor;


	v2Color = vFilter[1] * v1Color + (1.f - vFilter[1]) * vColor[2];
	v3Color = vFilter[2] * v2Color + (1.f - vFilter[2]) * vColor[3];
	v4Color = vFilter[3] * v3Color + (1.f - vFilter[3]) * vColor[4];
	v5Color = vFilter[4] * v4Color + (1.f - vFilter[4]) * vColor[5];
	v6Color = vFilter[5] * v5Color + (1.f - vFilter[5]) * vColor[6];
	vResultColor = vFilter[6] * v6Color + (1.f - vFilter[6]) * vColor[7];

	Out.vColor = vector(vResultColor.xyz, 1.f);

	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);	

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

technique Default_Device
{
	pass Default
	{
		ALPHABLENDENABLE = FALSE;

		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
