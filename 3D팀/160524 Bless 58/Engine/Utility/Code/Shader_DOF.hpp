// 조절가능
float g_fFar;
float g_fNear;

// Origin
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

texture		g_OriginTexture;
sampler OriginSampler = sampler_state
{
	Texture = g_OriginTexture;

	minfilter = linear;
	magfilter = linear;
};

// Blur
texture		g_BlurTexture;
sampler BlurSampler = sampler_state
{
	Texture = g_BlurTexture;

	//AddressU = Border;
	//AddressV = Border;
	//Bordercolor = 0x00000000;
	AddressU = Clamp;
	AddressV = Clamp;
};



// Combine
texture		g_FirstBlurTexture;
sampler FirstBlurSampler = sampler_state
{
	Texture = g_FirstBlurTexture;

	//AddressU = Border;
	//AddressV = Border;
	//Bordercolor = 0x00000000;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_SecondBlurXTexture;
sampler SecondBlurSampler = sampler_state
{
	Texture = g_SecondBlurXTexture;

	//AddressU = Border;
	//AddressV = Border;
	//Bordercolor = 0x00000000;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_DepthTexture;
sampler DepthSampler = sampler_state
{
	Texture = g_DepthTexture;
};

float2 samples[12] = {
	-0.326212, -0.405805,
	-0.840144, -0.073580,
	-0.695914,  0.457137,
	-0.203345,  0.620716,
	0.962340, -0.194983,
	0.473434, -0.480026,
	0.519456,  0.767022,
	0.185461, -0.893124,
	0.507431,  0.064425,
	0.896420,  0.412458,
	-0.321940, -0.932615,
	-0.791559, -0.597705,
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_Blur(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;

	float4 sum = tex2D(BlurSampler, In.vTexUV);
	float fSampleDist = 0.00934f;

	for(int i=0; i< 12; i++)
	{
		sum += tex2D(BlurSampler, In.vTexUV + fSampleDist * samples[i]);
	}

	Out.vColor = sum/13;


	return Out;
}

PS_OUT PS_Combine(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;

	
	// 백 버퍼의 z 값 정보 
	vector vDepth = tex2D(DepthSampler, In.vTexUV);

	float z = saturate(vDepth.r) * 1000.f;

	// 원거리 
	if (g_fFar <= z) 
		Out.vColor = tex2D(FirstBlurSampler, In.vTexUV); 

	// 근거리 
	else //if (g_fNear >= z) 
		Out.vColor = tex2D(OriginSampler, In.vTexUV); 

	// 중간 
	//else 
	//{   
	//	// z 값이 0.0f ~ 1.0f의 범위 내에서 값으로 변환한다. 
	//	float ConvZ = (z - g_fNear) / (g_fFar - g_fNear); 

	//	if (ConvZ < 0.5f) 
	//	{ 
	//		ConvZ *= 2.0f; 
	//		Out.vColor = tex2D(OriginSampler, In.vTexUV) * (1.0f - ConvZ) + tex2D(FirstBlurSampler, In.vTexUV ) * ConvZ; 
	//	} 
	//	else 
	//	{ 
	//		ConvZ -= 0.5f; 
	//		ConvZ *= 2.0f; 
	//		Out.vColor = tex2D (FirstBlurSampler, In.vTexUV) * (1.0f - ConvZ) + tex2D(SecondBlurSampler, In.vTexUV) * ConvZ; 
	//	}       
	//} 

	return Out;
}

technique Default_Device
{
	pass Blur
	{
		ZWriteEnable = false;
		ZEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Blur();
	}

	pass Combine
	{
		ZWriteEnable = false;
		ZEnable = false;

		//ALPHABLENDENABLE = TRUE;
		//SRCBLEND = SRCALPHA;
		//DESTBLEND = INVSRCALPHA;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_Combine();
	}
};
