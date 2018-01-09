#include "Shader_Base.hpp"

//vector		g_vBrushPos;
//float		g_fRange;
matrix		g_matLightWVP;
float		g_fUVMount;

/////////////////////////////////////////////////////////
texture		g_BaseTexture;//배경 텍스쳐
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};
/////////////////////////////////////////////////////////
texture		g_DestTexture;//첫번째 스플래팅 타일 텍스쳐
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

/////////////////////////////////////////////////////////
texture		g_FilterTexture;//1번째 스플래팅 팔렛트
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

/////////////////////////////////////////////////////////
texture		g_ShadowTexture;// 그림자 샘플러
sampler ShadowSampler = sampler_state
{
	Texture = g_ShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};

/////////////////////////////////////////////////////////

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
	vector		vLightProj : TEXCOORD4;
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

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matLightWVP);

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldPos : TEXCOORD1;	
	vector			vWorldNormal : TEXCOORD2;
	vector			vProjPos : TEXCOORD3;
	vector			vLightProj : TEXCOORD4;
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

	vector		vFilter[3];

	vFilter[0] = tex2D(FilterSampler, In.vTexUV);
	vFilter[1] = tex2D(FilterSampler2, In.vTexUV);
	vFilter[2] = tex2D(FilterSampler3, In.vTexUV);

	vector		vColor[4];

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fUVMount);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fUVMount);
	vColor[2] = tex2D(DestSampler2, In.vTexUV * g_fUVMount);
	vColor[3] = tex2D(DestSampler3, In.vTexUV * g_fUVMount);

	vector		vFirstColor = vFilter[0] * vColor[0] + (1.f - vFilter[0]) * vColor[1];
	vector		vSecondColor, vResultColor;


	vSecondColor = vFilter[1] * vFirstColor + (1.f - vFilter[1]) * vColor[2];
	vResultColor = vFilter[2] * vSecondColor + (1.f - vFilter[2]) * vColor[3];

	Out.vColor = vector(vResultColor.xyz, 1.f);

	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);	

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(ShadowSampler, fNewUV).r / 0.5f;

	if(fCurrentDepth > fShadowDepth + 0.0000125f/*- 0.00125f*/)
		Out.vDepth.z = 0.1f;

	return Out;
}

struct PS_Shadow_OUT
{
	vector			vDepth : COLOR0;
};

VS_OUT VS_Shadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matLightWVP);

	Out.vProjPos = Out.vPosition;

	return Out;
}


PS_Shadow_OUT PS_Shadow_Depth( PS_IN In )
{
	PS_Shadow_OUT			Out = (PS_Shadow_OUT)0;	

	float vDepth = (In.vProjPos.z / In.vProjPos.w) * 0.5f;

	Out.vDepth = vDepth;

	return Out;
}


technique Draw_Default
{
	pass Default
	{
		ALPHABLENDENABLE = FALSE;

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

technique Draw_Shadow
{
	pass Depth_Shadow
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_Shadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}
