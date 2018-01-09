#include "Shader_Base.hpp"


matrix	g_matSLightWVP;
matrix	g_matDLightWVP;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

//	Static Z Buffer
texture		g_SShadowTexture;
sampler SShadowSampler = sampler_state
{
	Texture = g_SShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};

// Dynamic Z Buffer
texture		g_DShadowTexture;
sampler DShadowSampler = sampler_state
{
	Texture = g_DShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
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
	vector		vLightProj : TEXCOORD3;
	vector		vSubLightProj : TEXCOORD4;
};

// Default
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


// Static Vertex Shader
VS_OUT VS_SMAIN( VS_IN In )
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

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);
	Out.vSubLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);

	return Out;
}

// Dynamic Vertex Shader
VS_OUT VS_DMAIN( VS_IN In )
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

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);
	Out.vSubLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldNormal : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;
	vector			vLightProj : TEXCOORD3;
	vector			vSubLightProj : TEXCOORD4;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
};

// Defulat
PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;
	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);

	return Out;
}


// Static Pixel Shader
PS_OUT PS_SMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;
	
	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;
	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);


	// Dynamic Shadow
	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(SShadowSampler, fNewUV).r / 0.5f;


	// Static Shadow
	float fDCurrentDepth = In.vSubLightProj.z / In.vSubLightProj.w;

	float2 fDNewUV = (float2)0;
	fDNewUV.x = (In.vSubLightProj.x / In.vSubLightProj.w) * 0.5f + 0.5f;
	fDNewUV.y = (In.vSubLightProj.y / In.vSubLightProj.w) * -0.5f + 0.5f;

	float fDShadowDepth = tex2D(DShadowSampler, fDNewUV).r / 0.5f;


	if(fCurrentDepth > fShadowDepth + 0.0005f ||
		fDCurrentDepth > fDShadowDepth + 0.0000125f)
		Out.vDepth.z = 0.1f;

	return Out;
}

// Dynamic Pixel Shader
PS_OUT PS_DMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;
	//Out.vColor.a = 1.f;
	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);


	// Dynamic Shadow
	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(DShadowSampler, fNewUV).r / 0.5f;

	// Static Shadow
	float fSCurrentDepth = In.vSubLightProj.z / In.vSubLightProj.w;

	float2 fSNewUV = (float2)0;
	fSNewUV.x = (In.vSubLightProj.x / In.vSubLightProj.w) * 0.5f + 0.5f;
	fSNewUV.y = (In.vSubLightProj.y / In.vSubLightProj.w) * -0.5f + 0.5f;

	float fSShadowDepth = tex2D(SShadowSampler, fSNewUV).r / 0.5f;

	if(fCurrentDepth > fShadowDepth + 0.0000125f ||
		fSCurrentDepth > fSShadowDepth + 0.0005f)
		Out.vDepth.z = 0.1f;

	return Out;
}



// Static Z Buffer 
VS_OUT VS_SShadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(In.vPosition, g_matSLightWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

// Dynamic Z Buffer
VS_OUT VS_DShadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(In.vPosition, g_matDLightWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_Shadow_OUT
{
	vector			vDepth : COLOR0;
};

PS_Shadow_OUT PS_Shadow_Depth( PS_IN In )
{
	PS_Shadow_OUT			Out = (PS_Shadow_OUT)0;	

	float vDepth = (In.vProjPos.z / In.vProjPos.w) * 0.5f;
	Out.vDepth = vDepth;

	return Out;
}



// Shaodw None
technique Default
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};


// Static BackBuffer Draw
technique S_Draw_Default
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_SMAIN();
		PixelShader = compile ps_3_0 PS_SMAIN();
	}
};

// Dynamic BackBuffer Draw
technique D_Draw_Default
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_DMAIN();
		PixelShader = compile ps_3_0 PS_DMAIN();
	}
};


// Static Z Buffer Draw
technique S_Draw_Shadow
{
	pass Depth_Shadow
	{
		VertexShader = compile vs_3_0 VS_SShadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}

// Dynamic Z Buffer Draw
technique D_Draw_Shadow
{
	pass Depth_Shadow
	{
		VertexShader = compile vs_3_0 VS_DShadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}
