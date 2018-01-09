#include "Shader_Base.hpp"

//vector		g_vBrushPos;
//float		g_fRange;
matrix		g_matSLightWVP;
matrix		g_matDLightWVP;
float		g_fUVMount;
vector		g_vLightPos;
vector		g_vCamPos;

/////////////////////////////////////////////////////////
texture		g_BaseTexture;//배경 텍스쳐
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
/////////////////////////////////////////////////////////
texture		g_DestTexture;//첫번째 스플래팅 타일 텍스쳐
sampler DestSampler = sampler_state
{
	Texture = g_DestTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


texture		g_DestTexture2;//두번째 스플래팅 타일 텍스쳐
sampler DestSampler2 = sampler_state 
{
	Texture = g_DestTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_DestTexture3;//세번째 스플래팅 타일 텍스쳐
sampler DestSampler3 = sampler_state 
{
	Texture = g_DestTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_DestTexture4;//네번째 스플래팅 타일 텍스쳐
sampler DestSampler4 = sampler_state 
{
	Texture = g_DestTexture4;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
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

texture		g_FilterTexture4;// 4번째 스플래팅 팔렛트
sampler FilterSampler4 = sampler_state
{
	Texture = g_FilterTexture4;
	minfilter = linear;
	magfilter = linear;
};
////////////////////////////////////////
texture		g_BaseNormTexture;//배경 노말 텍스쳐
sampler BaseNormSampler = sampler_state
{
	Texture = g_BaseNormTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
texture		g_DestNormTexture;//첫번째 스플래팅 타일 텍스쳐
sampler DestNormSampler = sampler_state
{
	Texture = g_DestNormTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


texture		g_DestNormTexture2;//두번째 스플래팅 타일 텍스쳐
sampler DestNormSampler2 = sampler_state 
{
	Texture = g_DestNormTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_DestNormTexture3;//세번째 스플래팅 타일 텍스쳐
sampler DestNormSampler3 = sampler_state 
{
	Texture = g_DestNormTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_DestNormTexture4;//네번째 스플래팅 타일 텍스쳐
sampler DestNormSampler4 = sampler_state 
{
	Texture = g_DestNormTexture4;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


/////////////////////////////////////////////////////////
texture		g_SShadowTexture;// Static 그림자 샘플러
sampler SShadowSampler = sampler_state
{
	Texture = g_SShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_DShadowTexture;// Dynamic 그림자 샘플러
sampler DShadowSampler = sampler_state
{
	Texture = g_DShadowTexture;

	AddressU = Border;
	AddressV = Border;
	Bordercolor = 0xffffffff;
};

/////////////////////////////////////////////////////////

vector NormalMapping(vector vNormalMap, float3 vT, float3 vB, float3 vN, float fProjPosW)
{
	//// 노말텍스쳐 없는거
	//if(vNormal.x == 1.f && vNormal.y == 1.f && vNormal.z == 1.f)
	//{
	//	vector		vWorldNormal = normalize(mul(In.vNormal, g_matWorld));	
	//	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	//}
	//else		//  노말텍스쳐 있는거
	//{
	vector vNormal = normalize(vNormalMap * 2.f - 1.f);

	float3x3 TBN = float3x3(normalize(vT), normalize(vB), normalize(vN));
	TBN = transpose(TBN);

	float3 vWorldNormal = mul(TBN, vNormal);

	return vector(vWorldNormal.xyz * 0.5f + 0.5f, fProjPosW / 1000.f);
	//}
}



struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	////// 노말매핑
	float3		vTangent	: TANGENT;
	float3		vBinormal	: BINORMAL;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;	
	vector		vWorldNormal : TEXCOORD2;
	vector		vProjPos : TEXCOORD3;
	vector		vLightProj : TEXCOORD4;
	vector		vDLightProj : TEXCOORD5;

	//// 노말매핑
	float3		vT			: COLOR0;
	float3		vB			: COLOR1;
	//float3		vN			: COLOR2;
	float3		vLightDir	: COLOR3;
	float3		vViewDir	: COLOR4;
};

// Shadow None
VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector		vWorldNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);
	Out.vTexUV = In.vTexUV / 8.f;
	Out.vWorldNormal = vWorldNormal;
	Out.vProjPos = Out.vPosition;
	//Out.vN = normalize(mul(In.vNormal.xyz, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));

	return Out;
}

// Shadow
VS_OUT VS_Shadow_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector		vWorldNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);
	Out.vTexUV = In.vTexUV / 8.f;
	Out.vWorldNormal = vWorldNormal;
	Out.vProjPos = Out.vPosition;
	//Out.vN = normalize(mul(In.vNormal.xyz, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);
	Out.vDLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldPos : TEXCOORD1;	
	vector			vWorldNormal : TEXCOORD2;
	vector			vProjPos : TEXCOORD3;
	vector			vLightProj : TEXCOORD4;
	vector			vDLightProj : TEXCOORD5;

	//// 노말매핑
	float3		vT			: COLOR0;
	float3		vB			: COLOR1;
	//float3		vN			: COLOR2;
	float3		vLightDir	: COLOR3;
	float3		vViewDir	: COLOR4;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
	vector			vSpecular : COLOR3;
};

// Shaodw None
PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vFilter[4];

	vFilter[0] = tex2D(FilterSampler, In.vTexUV);
	vFilter[1] = tex2D(FilterSampler2, In.vTexUV);
	vFilter[2] = tex2D(FilterSampler3, In.vTexUV);
	vFilter[3] = tex2D(FilterSampler4, In.vTexUV);

	vector		vColor[5];
	float fDivision = 15.f;

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fUVMount / fDivision);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fUVMount / fDivision);
	vColor[2] = tex2D(DestSampler2, In.vTexUV * g_fUVMount / fDivision);
	vColor[3] = tex2D(DestSampler3, In.vTexUV * g_fUVMount / fDivision);
	vColor[4] = tex2D(DestSampler4, In.vTexUV * g_fUVMount / 2.f);

	vector vNormColor[5];
	vNormColor[0] = tex2D(BaseNormSampler, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[1] = tex2D(DestNormSampler, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[2] = tex2D(DestNormSampler2, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[3] = tex2D(DestNormSampler3, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[4] = tex2D(DestNormSampler4, In.vTexUV * g_fUVMount / 2.f);


	vector		v1Color = vFilter[0] * vColor[0] + (1.f - vFilter[0]) * vColor[1];
	vector		v2Color, v3Color, vResultColor;


	v2Color = vFilter[1] * v1Color + (1.f - vFilter[1]) * vColor[2];
	v3Color = vFilter[2] * v2Color + (1.f - vFilter[2]) * vColor[3];
	vResultColor = vFilter[3] * v3Color + (1.f - vFilter[3]) * vColor[4];

	vector		v1NormColor = vFilter[0] * vNormColor[0] + (1.f - vFilter[0]) * vNormColor[1];
	vector		v2NormColor, v3NormColor, vResultNormColor;

	v2NormColor = vFilter[1] * v1NormColor + (1.f - vFilter[1]) * vNormColor[2];
	v3NormColor = vFilter[2] * v2NormColor + (1.f - vFilter[2]) * vNormColor[3];
	vResultNormColor = vFilter[3] * v3NormColor + (1.f - vFilter[3]) * vNormColor[4];


	Out.vColor = vector(vResultColor.xyz, 1.f);
	Out.vNormal = NormalMapping(vResultNormColor, In.vT, In.vB, In.vWorldNormal, In.vProjPos.w);	
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);
	Out.vSpecular = vector(0.f, 0.f, 0.f, 1.f);
	return Out;
}

// Shadow
PS_OUT PS_Shadow_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vFilter[4];

	vFilter[0] = tex2D(FilterSampler, In.vTexUV);
	vFilter[1] = tex2D(FilterSampler2, In.vTexUV);
	vFilter[2] = tex2D(FilterSampler3, In.vTexUV);
	vFilter[3] = tex2D(FilterSampler4, In.vTexUV);

	vector		vColor[5];
	float fDivision = 15.f;

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fUVMount / fDivision);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fUVMount / fDivision);
	vColor[2] = tex2D(DestSampler2, In.vTexUV * g_fUVMount / fDivision);
	vColor[3] = tex2D(DestSampler3, In.vTexUV * g_fUVMount / fDivision);
	vColor[4] = tex2D(DestSampler4, In.vTexUV * g_fUVMount / 2.f);

	vector vNormColor[5];
	vNormColor[0] = tex2D(BaseNormSampler, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[1] = tex2D(DestNormSampler, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[2] = tex2D(DestNormSampler2, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[3] = tex2D(DestNormSampler3, In.vTexUV * g_fUVMount / fDivision);
	vNormColor[4] = tex2D(DestNormSampler4, In.vTexUV * g_fUVMount / 2.f);


	vector		v1Color = vFilter[0] * vColor[0] + (1.f - vFilter[0]) * vColor[1];
	vector		v2Color, v3Color, vResultColor;


	v2Color = vFilter[1] * v1Color + (1.f - vFilter[1]) * vColor[2];
	v3Color = vFilter[2] * v2Color + (1.f - vFilter[2]) * vColor[3];
	vResultColor = vFilter[3] * v3Color + (1.f - vFilter[3]) * vColor[4];

	vector		v1NormColor = vFilter[0] * vNormColor[0] + (1.f - vFilter[0]) * vNormColor[1];
	vector		v2NormColor, v3NormColor, vResultNormColor;

	v2NormColor = vFilter[1] * v1NormColor + (1.f - vFilter[1]) * vNormColor[2];
	v3NormColor = vFilter[2] * v2NormColor + (1.f - vFilter[2]) * vNormColor[3];
	vResultNormColor = vFilter[3] * v3NormColor + (1.f - vFilter[3]) * vNormColor[4];


	Out.vColor = vector(vResultColor.xyz, 1.f);
	Out.vNormal = NormalMapping(vResultNormColor, In.vT, In.vB, In.vWorldNormal, In.vProjPos.w);	
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);


	// Static Check
	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(SShadowSampler, fNewUV).r / 0.5f;



	// Dynamic Check
	float fDCurrentDepth = In.vDLightProj.z / In.vDLightProj.w;

	float2 fDNewUV = (float2)0;
	fDNewUV.x = (In.vDLightProj.x / In.vDLightProj.w) * 0.5f + 0.5f;
	fDNewUV.y = (In.vDLightProj.y / In.vDLightProj.w) * -0.5f + 0.5f;

	float fDShadowDepth = tex2D(DShadowSampler, fDNewUV).r / 0.5f;

	if(fCurrentDepth > fShadowDepth + 0.0000125f ||
		fDCurrentDepth > fDShadowDepth + 0.0000125f)
		Out.vDepth.z = 0.1f;

	Out.vSpecular = vector(0.f, 0.f, 0.f, 1.f);

	return Out;
}

struct PS_Shadow_OUT
{
	vector			vDepth : COLOR0;
};

VS_OUT VS_Shadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);

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

technique Default
{
	pass Depth_Shadow
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}


technique S_Draw_Default
{
	pass Default
	{
		ALPHABLENDENABLE = FALSE;

		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_Shadow_MAIN();
		PixelShader = compile ps_3_0 PS_Shadow_MAIN();
	}
};

technique S_Draw_Shadow
{
	pass Depth_Shadow
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_Shadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}
