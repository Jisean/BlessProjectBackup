#include "Shader_Base.hpp"

texture		g_BaseTexture;
texture		g_MaskTexture;
texture		g_NormalTexture;
vector		g_vColor;
vector		g_vSize;

vector		g_vSetColor;
vector		g_vLightDir;
vector		g_vCamPosition;

float		g_fU;
float		g_fV;
float		g_fMaskU;
float		g_fMaskV;
float		g_fFade;
float		g_fUSize;
float		g_fVSize;
int			g_iPass;

vector		g_vUVSprite;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};



sampler MaskSampler = sampler_state
{ 
	Texture = g_MaskTexture;
	minfilter = linear;
	magfilter = linear;
};

sampler NormalSampler = sampler_state
{ 
	Texture = g_NormalTexture;
	minfilter = linear;
	magfilter = linear;
};



struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

};

struct VS_NORMALIN
{
	vector		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float3		vTangent	: TANGENT;
	float3		vBiNormal	: BINORMAL;
	float2		vTexUV		: TEXCOORD0;

};


struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;	
	vector		vWorldNormal : TEXCOORD2;	
	float2		vTexUV2 : TEXCOORD3;
};

struct VS_NORMALOUT
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
	vector		vLightDir		: TEXCOORD1;
	vector		vViewDir		: TEXCOORD2;
	float3		vWorldTangent	: TEXCOORD3;
	float3		vWorldBinormal	: TEXCOORD4;
	float3		vWorldNormal	: TEXCOORD5;
	vector		vWorldPos		: TEXCOORD6;	
	
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
	//Out.vTexUV = In.vTexUV + float2( g_fU , g_fV);
	Out.vTexUV = In.vTexUV;

	//Out.vTexUV = In.vTexUV; 

	//if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	//{
	//	Out.vTexUV += 2.f;
	//}
	//else
	//{
	//	//Out.vTexUV.x = (Out.vTexUV.x * (1.f/ g_vUVSprite.x)) + ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
	//	//Out.vTexUV.y = (Out.vTexUV.y * (1.f/ g_vUVSprite.y)) + ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	//}

	Out.vWorldNormal = vWorldNormal;

	return Out;
}

VS_NORMALOUT VS_NORMALMAIN( VS_NORMALIN In )
{
	VS_NORMALOUT			Out = (VS_NORMALOUT)0;

	

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	float3		vWorldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	float3		vWorldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	float3		vWorldBiNormal = mul(In.vBiNormal, (float3x3)g_matWorld);


	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vTexUV = In.vTexUV + float2( g_fU , g_fV);

	Out.vLightDir = normalize(g_vLightDir);

	vector vViewDir = normalize(Out.vWorldPos - g_vCamPosition);
	Out.vViewDir = vViewDir;

	Out.vWorldNormal = vWorldNormal;
	Out.vWorldTangent = vWorldTangent;
	Out.vWorldBinormal = vWorldBiNormal;



	return Out;
}



struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldPos : TEXCOORD1;	
	vector			vWorldNormal : TEXCOORD2;
};

struct PS_NORMALIN
{
	float2		vTexUV			: TEXCOORD0;		
	vector		vLightDir		: TEXCOORD1;
	vector		vViewDir		: TEXCOORD2;
	float3		vWorldTangent	: TEXCOORD3;
	float3		vWorldBinormal	: TEXCOORD4;
	float3		vWorldNormal	: TEXCOORD5;
	vector		vWorldPos		: TEXCOORD6;	
};


struct PS_OUT
{
	vector			vColor : COLOR0;
};

struct PS_NORMALOUT
{
	vector			vColor : COLOR0;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}
	
	vector vColor = tex2D(BaseSampler,  float2(In.vTexUV.x * g_fUSize + g_fU, In.vTexUV.y * g_fVSize + g_fV ));
	
	//vColor = pow(vColor, 2.2f);

	//vector vAlphaColor = vColor;
	//
	//vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	//Out.vColor = vColor * vAlphaColor;

	
	Out.vColor = vColor;

	if(g_vSetColor.w == 1.f)
	{
		Out.vColor.rgb *= g_vSetColor.rgb;
	}


	//Out.vColor.r = 0.f;
	//Out.vColor.g = 0.f;

	Out.vColor *= g_fFade;
	
	return Out;
}

PS_OUT PS_MAIN_OPX( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}

	vector vColor = tex2D(BaseSampler,  float2(In.vTexUV.x * g_fUSize + g_fU, In.vTexUV.y * g_fVSize + g_fV ));	

//	+ float2( g_fU , g_fV);
	vector vAlphaColor = vColor;
	
	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	Out.vColor = vColor * vAlphaColor;	
 
	if(g_vSetColor.w == 1.f)
	{
		Out.vColor.rgb *= g_vSetColor.rgb;
	}	

	Out.vColor.a *= g_fFade;

	return Out;
}

PS_OUT PS_MASKMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}
	
	vector		vColor	= tex2D(BaseSampler, float2(In.vTexUV.x * g_fUSize + g_fU, In.vTexUV.y * g_fVSize + g_fV));
	
	vector		vMask	= tex2D(MaskSampler, float2(In.vTexUV.x * g_fUSize + g_fMaskU, In.vTexUV.y * g_fVSize + g_fMaskV));
	

	
	vector		vResultColor = vMask * vColor;	

	if(g_vSetColor.w == 1.f)
	{
		vResultColor.rgb *= g_vSetColor.rgb; 
	}

	Out.vColor = vResultColor;

	Out.vColor *= g_fFade;

	return Out;
}

PS_OUT PS_MASKMAIN_OPX( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}


	vector		vColor	= tex2D(BaseSampler, float2((In.vTexUV.x * g_fUSize) + g_fU, In.vTexUV.y * g_fVSize + g_fV ));

	vector		vMask	= tex2D(MaskSampler, float2((In.vTexUV.x * g_fUSize) + g_fMaskU, (In.vTexUV.y * g_fVSize) + g_fMaskV ));

	vMask.rgba = vector(1.f, 1.f, 1.f, vMask.r);

	vector vAlphaColor = vColor;

	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	vector vResultAlpha = vMask * vAlphaColor;

	vector		vResultColor = vResultAlpha * vColor;	

	if(g_vSetColor.w == 1.f)
	{
		vResultColor.rgb = g_vSetColor.rgb; 
	}


	Out.vColor = vResultColor;

	Out.vColor.a *= g_fFade;

	return Out;
}

//노말맵핑
PS_NORMALOUT PS_NORMALMAIN( PS_NORMALIN In )
{
	PS_OUT			Out = (PS_OUT)0;

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}
	

	float3 vTangentNormal = tex2D(NormalSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));

	vTangentNormal = normalize(vTangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.vWorldTangent), normalize(In.vWorldBinormal), normalize(In.vWorldNormal));

	TBN = transpose(TBN);

	float3 vWolrdNormal = mul(TBN, vTangentNormal);
	vector vLightDir = normalize(In.vLightDir);

	vector		vShade = saturate(dot(vWolrdNormal, -vLightDir));

	vector		vColor	= tex2D(BaseSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));
	vector		vMask	= tex2D(MaskSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));



	// + (1.f - vFilter) * vColor;

//	vMask.rgba = vector(1.f, 1.f, 1.f, vMask.r);

//	vector vAlphaColor = vColor;

//	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	//vector vResultAlpha = vMask * vAlphaColor;

	//vector		vResultColor = vResultAlpha * vColor * vShade;	

	vector vResultColor = vColor *  vMask * vShade;

	//vColor = vColor * vMask;

	//vector		vResultColor = vShade * vColor;


	Out.vColor = vResultColor;

	if(g_vSetColor.w == 1.f)
	{
		Out.vColor.rgb *= g_vSetColor.rgb; 
	}


	Out.vColor *= g_fFade;

	
	//Out.vColor *= g_fFade;
	return Out;
}

PS_NORMALOUT PS_NORMALMAIN_OPX( PS_NORMALIN In )
{
	PS_OUT			Out = (PS_OUT)0;

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
	}
	else
	{
		In.vTexUV.x = (In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z);
		In.vTexUV.y = (In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w);
	}


	float3 vTangentNormal = tex2D(NormalSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));
	vTangentNormal = normalize(vTangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.vWorldTangent), normalize(In.vWorldBinormal), normalize(In.vWorldNormal));

	TBN = transpose(TBN);

	float3 vWolrdNormal = mul(TBN, vTangentNormal);
	vector vLightDir = normalize(In.vLightDir);

	vector		vShade = saturate(dot(vWolrdNormal, -vLightDir));

	vector		vColor	= tex2D(BaseSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));
	vector		vMask	= tex2D(MaskSampler, float2(In.vTexUV.x * g_fUSize, In.vTexUV.y * g_fVSize ));



	vMask.rgba = vector(1.f, 1.f, 1.f, vMask.r);

	vector vAlphaColor = vColor;

	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	vector vResultAlpha = vMask * vAlphaColor;

	if(g_vSetColor.w == 1.f)
	{
		vColor.rgb *= g_vSetColor.rgb; 
	}

	vector		vResultColor = vResultAlpha * vColor * vShade;	

	// + (1.f - vFilter) * vColor;

	//	vMask.rgba = vector(1.f, 1.f, 1.f, vMask.r);

	//	vector vAlphaColor = vColor;

	//	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	//vector vResultAlpha = vMask * vAlphaColor;

	//vector		vResultColor = vResultAlpha * vColor * vShade;	



	//vector vResultColor = vColor *  vMask * vShade;

	//vColor = vColor * vMask;

	//vector		vResultColor = vShade * vColor;


	Out.vColor = vResultColor;

	Out.vColor.a *= g_fFade;

	//Out.vColor *= g_fFade;

	return Out;
}


PS_OUT PS_SPRITEMAIN_OPX( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vColor	= 0.f;

	vector		vMask	= 0.f;

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
		vColor	= tex2D(BaseSampler, float2((In.vTexUV.x * g_fUSize) + g_fU, In.vTexUV.y * g_fVSize + g_fV ));

		vMask	= tex2D(MaskSampler, float2((In.vTexUV.x * g_fUSize) + g_fMaskU, (In.vTexUV.y * g_fVSize) + g_fMaskV ));
	}
	else
	{
		vColor	= tex2D(BaseSampler, float2((((In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z)) * g_fUSize) + g_fU, ((In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w)) * g_fVSize + g_fV ));
		vMask	= tex2D(MaskSampler, float2((In.vTexUV.x * g_fUSize) + g_fMaskU, (In.vTexUV.y * g_fVSize) + g_fMaskV ));
	}

	vMask.rgba = vector(1.f, 1.f, 1.f, vMask.r);

	vector vAlphaColor = vColor;

	vAlphaColor.rgba = vector(1.f, 1.f, 1.f, vAlphaColor.r);

	vector vResultAlpha = vMask * vAlphaColor;

	if(g_vSetColor.w == 1.f)
	{
		vColor.rgb *= g_vSetColor.rgb; 
	}

	vector		vResultColor = vResultAlpha * vColor;	

	Out.vColor = vResultColor;

	Out.vColor.a *= g_fFade;

	return Out;
}

PS_OUT PS_SPRITEMASKMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vColor	= 0.f;

	vector		vMask	= 0.f;

	if(g_vUVSprite.x == 0.f && g_vUVSprite.y == 0.f && g_vUVSprite.z == 0.f && g_vUVSprite.w == 0.f )
	{
		vColor	= tex2D(BaseSampler, float2((In.vTexUV.x * g_fUSize) + g_fU, In.vTexUV.y * g_fVSize + g_fV ));

		vMask	= tex2D(MaskSampler, float2((In.vTexUV.x * g_fUSize) + g_fMaskU, (In.vTexUV.y * g_fVSize) + g_fMaskV ));
	}
	else
	{
		vColor	= tex2D(BaseSampler, float2((((In.vTexUV.x * (1.f/ g_vUVSprite.x))	+ ((1.f / g_vUVSprite.x) * g_vUVSprite.z)) * g_fUSize) + g_fU, ((In.vTexUV.y * (1.f/ g_vUVSprite.y))	+ ((1.f / g_vUVSprite.y) * g_vUVSprite.w)) * g_fVSize + g_fV ));
		vMask	= tex2D(MaskSampler, float2((In.vTexUV.x * g_fUSize) + g_fMaskU, (In.vTexUV.y * g_fVSize) + g_fMaskV ));
	}

	if(g_vSetColor.w == 1.f)
	{
		vColor.rgb *= g_vSetColor.rgb; 
	}

	vector		vResultColor = vMask * vColor;	

	Out.vColor = vResultColor;

	Out.vColor *= g_fFade;

	return Out;
}

technique Default_Device
{
	pass Default
	{
		CullMode = none;
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정

		//ALPHABLENDENABLE = TRUE;
		//SRCBLEND = SRCALPHA;
		//DESTBLEND = INVSRCALPHA;

		//ALPHATESTENABLE	= TRUE; 
		//ALPHAREF	= 0x00000088;
		//ALPHAFUNC = GREATER;

		VertexShader = compile vs_3_0 VS_MAIN();		
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Alpha
	{
		CullMode = none;		

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE; 
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Mask
	{
		CullMode = none;

		ZWriteEnable = false;
		//ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MASKMAIN();

	}	

	pass NormalOPX
	{
		CullMode = none;

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		/*BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;*/

		VertexShader = compile vs_3_0 VS_NORMALMAIN();
		PixelShader = compile ps_3_0 PS_NORMALMAIN_OPX();

	}

	pass AlphaOPX
	{
		CullMode = none;		

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE; 
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

	/*	BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;*/

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_OPX();
	}

	pass MaskOPX
	{
		CullMode = none;

		ZWriteEnable = false;
		//ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MASKMAIN_OPX();

	}	

	pass Texture
	{
		CullMode = none;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		ZWriteEnable = false;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();

	}

	pass Normal
	{
		CullMode = none;

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 VS_NORMALMAIN();
		PixelShader = compile ps_3_0 PS_NORMALMAIN();

	}

	pass SpriteOPX
	{
		CullMode = none;

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_SPRITEMAIN_OPX();

	}

	pass Sprite
	{
		CullMode = none;

		ZWriteEnable = false;
		ZEnable = true;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		/*	ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;*/

		BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;
		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_SPRITEMASKMAIN();
	}
};