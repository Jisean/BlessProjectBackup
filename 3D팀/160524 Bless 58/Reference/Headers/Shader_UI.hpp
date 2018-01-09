#include "Shader_Base.hpp"

float		g_fAlpha;
float		g_fRGB;
float		g_fPointPower;

float		g_fColor;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
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
	
	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	
	//Out.vColor.gb *= g_fRGB; 
	Out.vColor.r *= g_fPointPower;//pow(Out.vColor.r,g_fPointPower);
	Out.vColor.a *= g_fAlpha;

	/*if(Out.vColor.r == 0.f && Out.vColor.g == 0.f && Out.vColor.b == 0.f)
		Out.vColor.a = 0.f;*/

	/*if(Out.vColor.r == 1.f && Out.vColor.g == 1.f && Out.vColor.b == 1.f)
		Out.vColor.a = 0.f;*/

	return Out;
}

PS_OUT PS_SKILL_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;
	
	vector vFilter = tex2D(FilterSampler, In.vTexUV);

	if(vFilter.x < g_fColor)
	{
		Out.vColor.xyz = 0.0f;
		Out.vColor.a = 0.7f;
	}


	return Out;
}

PS_OUT PS_Quest_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.rgb *= 0.5f;
	Out.vColor.a *= 0.7f;
	
	return Out;
}

PS_OUT PS_TXT_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vFilter = tex2D(FilterSampler, In.vTexUV);

	if(vFilter.x > g_fColor)
	{
		Out.vColor = 0.f;
	}


	return Out;
}

technique Default_Device
{
	pass Default
	{
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};

technique SkillRange
{
	pass Default
	{
		//cullmode = none;

		zwriteenable = false;
		zenable = true;

		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		ALPHATESTENABLE   = TRUE; 
		ALPHAREF   = 0x05;
		ALPHAFUNC = GREATER;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};


technique Skill
{
	// 기본 밑판 그리기
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Time
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_SKILL_MAIN();
	}
};


technique Txt
{
	// 기본 밑판 그리기
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_TXT_MAIN();
	}
}

technique Quest
{
	// 기본 밑판 그리기
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Quest_MAIN();
	}
}