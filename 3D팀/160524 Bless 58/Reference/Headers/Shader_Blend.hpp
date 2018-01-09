// DOF = false
texture		g_AlbedoTexture;
sampler AlbedoSampler = sampler_state
{
	Texture = g_AlbedoTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_LightTexture;
sampler LightSampler = sampler_state
{
	Texture = g_LightTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_SpecularTexture;
sampler SpecularSampler = sampler_state
{
	Texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_DepthTexture;
sampler DepthSampler = sampler_state
{
	Texture = g_DepthTexture;
	minfilter = linear;
	magfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};



// DOF = true
texture		g_DOFTexture;
sampler DOFSampler = sampler_state
{
	Texture = g_DOFTexture;
	/*minfilter = linear;
	magfilter = linear;*/

	AddressU = Clamp;
	AddressV = Clamp;
};


struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_Draw_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;

	Out.vColor = tex2D(DOFSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_Blend_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;
	vector		vAlbedo, vLight, vSpecular, vDepth; 

	vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vLight = tex2D(LightSampler, In.vTexUV);
	vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vDepth = tex2D(DepthSampler, In.vTexUV);

	if(vSpecular.x > 0.f)
	{
		Out.vColor = vAlbedo * vLight;
		Out.vColor.rgb += vSpecular.rgb;
	}
	else
		Out.vColor = vAlbedo * vLight;

	return Out;
}


technique Blend_DrawBuffer
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Draw_MAIN();

	}
};

technique Blend_BlendBuffer
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Blend_MAIN();
	}

	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Blend_MAIN();
	}
};
