


texture		g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	Texture = g_AlbedoTexture;
	minfilter = linear;
	magfilter = linear;
};

texture		g_LightTexture;

sampler LightSampler = sampler_state
{
	Texture = g_LightTexture;
	minfilter = linear;
	magfilter = linear;
};

texture		g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	Texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
};


struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;
	vector		vAlbedo, vLight, vSpecular; 

	vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vLight = tex2D(LightSampler, In.vTexUV);
	vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vAlbedo * vLight;

	return Out;
}


technique Default_Device
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
