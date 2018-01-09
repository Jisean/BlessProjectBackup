#include "Shader_Base.hpp"

float		g_fTime;
vector		g_vLightPos;
vector		g_vCamPosition;
vector		g_vCamPos;

float		g_fSpecMapping = 0.f;

float		g_fSpecRange = 5.f;
float		g_fSpecPow = 1.f;


float4 wvK		= float4( 0.002F, 0.001F, 0.01F, 0.004F);
float4 wvSpeed	= float4( 4.0F, 3.5F,  1.4F, 2.8F);
float4 wvOmega	= float4( 0.35F, 0.2F,  0.4F, 0.3F);

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture		g_NormalTexture;
sampler NormalSampler = sampler_state
{
	Texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
};


vector SpecularMapping(float fCheckSpec, float3 vReflection, float3 vViewDir, float2 fTexUV)
{
	float3 vReflect = normalize(vReflection);
	float3 ViewDir = normalize(vViewDir);

	vector vSpecular = dot(vReflect, -ViewDir);

	if(fCheckSpec == 1.f)
	{
		vSpecular = pow(abs(vSpecular), g_fSpecRange);
		vSpecular.a = 1.f;
		vSpecular *= g_fSpecPow;
	}

	return vSpecular;
}


struct VS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;

	float3		vNormal		: NORMAL;
	float3		vTangent	: TANGENT;
	float3		vBinormal	: BINORMAL;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	//vector		vWorldNormal : TEXCOORD1;
	vector		vProjPos	: TEXCOORD2;

	float3		vT			: TEXCOORD3;
	float3		vB			: TEXCOORD4;
	float3		vN			: TEXCOORD1;


	//
	float3		vReflection	: COLOR0;
	float3		vViewDir	: COLOR1;
};

float3 WavePos(float3 Pos, float3 eps = float3(0, 0, 0))
{
	// Wave
	float3	vPos, vPower;		// vPower ÆÄ¿öÀÎÁö ¸ð¸§
	float	fr;					// ¹ºÁö ¸ð¸§

	float fTemp = 400;

	Pos += eps;
	vPos	= Pos;

	vPower	= Pos - float3(-fTemp, 0, 0);
	fr		= sqrt(vPower.x * vPower.x + vPower.z * vPower.z);
	vPos.y	+= exp(-wvK.x * fr) * sin(fr * wvOmega.x - g_fTime * wvSpeed.x);

	vPower	= Pos - float3(fTemp, 0, 0);
	fr		= sqrt(vPower.x * vPower.x + vPower.z * vPower.z);
	vPos.y	+= exp(-wvK.y * fr) * sin(fr * wvOmega.y - g_fTime * wvSpeed.y);

	vPower	= Pos - float3(0, 0, fTemp);
	fr		= sqrt(vPower.x * vPower.x + vPower.z * vPower.z);
	vPos.y	+= exp(-wvK.z * fr) * sin(fr * wvOmega.z - g_fTime * wvSpeed.z);

	vPower	= Pos - float3(0, 0, -fTemp);
	fr		= sqrt(vPower.x * vPower.x + vPower.z * vPower.z);
	vPos.y	+= exp(-wvK.w * fr) * sin(fr * wvOmega.w - g_fTime * wvSpeed.w);

	return vPos;
}

// Default
VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	//vector		vWorldNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	float3 vPos;
	vPos = WavePos((float3)In.vPosition.xyz);
	
	Out.vPosition = mul(vector(vPos.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	//Out.vWorldNormal = vWorldNormal;

	//vector vWorldPos = mul(vector(vPos.xyz, 1.f), g_matWorld);

	Out.vN = normalize(mul(In.vNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));


	// Specular Mapping
	float3 vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	vLightDir = normalize(vLightDir);

	Out.vReflection = reflect(vLightDir, Out.vN);
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);

	

	return Out;
}



struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	//vector			vWorldNormal : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;

	float3			vT			: TEXCOORD3;
	float3			vB			: TEXCOORD4;
	float3			vN			: TEXCOORD1;

	float3		vReflection	: COLOR0;
	float3		vViewDir	: COLOR1;
};	

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
	vector			vSpecular : COLOR3;
};

// Defulat
PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV * 3.f);
	vector		vNormal = tex2D(NormalSampler, In.vTexUV * 3.f);
	vNormal = normalize(vNormal * 2.f - 1.f);


	float3x3 TBN = float3x3(normalize(In.vT), normalize(In.vB), normalize(In.vN));
	TBN = transpose(TBN);
	
	float3 vWorldNormal = mul(TBN, vNormal);


	Out.vColor = vColor;
	Out.vColor *= 0.5f;
	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);

	// ½ºÆåÅ§·¯ ¸ÊÇÎ
	Out.vSpecular = 0.f;
	if(g_fSpecMapping == 1)
	{
		Out.vSpecular = SpecularMapping(g_fSpecMapping, In.vReflection, In.vViewDir, In.vTexUV);
		Out.vSpecular.a = In.vProjPos.z / In.vProjPos.w;
	}

	return Out;
}

technique Default
{
	pass Default
	{
		LIGHTING	= FALSE;

		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//FILLMODE	= WireFrame;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
