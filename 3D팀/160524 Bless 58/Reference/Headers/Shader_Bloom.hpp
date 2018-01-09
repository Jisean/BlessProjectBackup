#include "Shader_Base.hpp"

float		g_fBloomPower;
float		g_fBloomRange;
float		g_fLightPower;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};

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

	vector vColor = 0.f;

	vColor = abs(tex2D(BaseSampler, In.vTexUV));

	vColor.rbg -= g_fBloomRange;

	Out.vColor = g_fBloomPower * max(vColor, 0.f);
	Out.vColor = vector(Out.vColor.xxx, Out.vColor.w);

	return Out;
}

PS_OUT PS_Blend( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector vColor = 0.f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();

		MinFilter[0] = Point;
		MagFilter[0] = Point;
	}

	pass Blend
	{
		ZWriteEnable = false;
		ZEnable = false;

		AlphaBlendenable = true;	

		Blendop = Add;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Blend();
	}
};

