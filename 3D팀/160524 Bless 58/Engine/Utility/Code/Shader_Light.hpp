
vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange;


vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);

vector		g_vCamPosition;
float		g_fPower;

float		g_fLightPower;

matrix		g_matViewInv;
matrix		g_matProjInv;

texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	Texture = g_NormalTexture;	

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	Texture = g_DepthTexture;
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	//vector		vSpecular : COLOR1;
	vector		vShadow : COLOR1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;
	vector		vNormal, vColor, vDepth; 

	float		Power = g_fLightPower;

	vColor = tex2D(NormalSampler, In.vTexUV);
	vColor = pow(abs(vColor), Power);

	vNormal = vector(vColor.xyz * 2.f - 1.f, 0.f);

	vDepth = tex2D(DepthSampler, In.vTexUV);
	Out.vShadow = vector(vDepth.zzz, 19.f);

	vector		vWorldLight = normalize(g_vLightDir) * -1.f;

	Out.vColor = (max(dot(vWorldLight, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) * vDepth.z) + (g_vLightAmbient * g_vMtrlAmbient);

	if(vDepth.w != 1.f)	
		Out.vColor = lerp(vector(0.f, 0.f, 0.f, 1.f), Out.vColor, vDepth.w);
	//Out.vColor = pow(abs(Out.vColor), 1/Power);
	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.f;
	vector		vNormal, vColor, vDepth; 

	float		Power = g_fLightPower;

	vColor = tex2D(NormalSampler, In.vTexUV);
	vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vColor.w * 1000.f;

	vNormal = vector(vColor.xyz * 2.f - 1.f, 0.f);
	vNormal = pow(abs(vNormal), Power);

	vector		vWorldLight = (vector)0.f;

	// 픽셀의 위치를 구한다.
	vector		vPosition;

	// In.ViewSpace * matProj
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.r * fViewZ;
	vPosition.w = fViewZ;

	// In.ViewSpace
	vPosition = mul(vPosition, g_matProjInv);

	// In.WorldSpace
	vPosition = mul(vPosition, g_matViewInv);

	vWorldLight = (vPosition - g_vLightPos);

	float		fDistance = length(vWorldLight);
	
	Out.vColor = max(dot(normalize(vWorldLight) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vColor = pow(abs(Out.vColor), 1/Power);

	float		fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	Out.vColor = Out.vColor * fAtt;
	Out.vColor.a = 1.f;


	return Out;		
}


technique Default_Device
{
	pass Directional
	{
		ZWriteEnable = false;
		ZEnable = false;
		
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Point
	{
		ZWriteEnable = false;
		ZEnable = false;

		AlphaBlendenable = true;	

		Blendop = Add;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
};
