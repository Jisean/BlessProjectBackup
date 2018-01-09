float	g_BlurStart;
float	g_BlurWidth;

float	g_CenterX;
float	g_CenterY;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;

	//AddressU = Border;
	//AddressV = Border;
	//Bordercolor = 0x00000000;

	AddressU = Clamp;
	AddressV = Clamp;
};

float Samples[10] = {
	-0.08f,
	-0.05f,
	-0.03f,
	-0.02f,
	-0.01f,
	 0.01f,
	 0.02f,
	 0.03f,
	 0.05f,
	 0.08f
};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	float fCenterX = 0.5f;
	// ���� ���� �߽���(ȭ�� �߽�)���� �ȼ������� �Ÿ� ���ϱ�
	float2 Dir;
	Dir.x = g_CenterX - In.vTexUV.x;
	Dir.y = g_CenterY - In.vTexUV.y;


	float Dist = sqrt(Dir.x * Dir.x + Dir.y * Dir.y);
	Dir /= Dist;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vAcc = vColor;

	for(int i = 0; i < 10; ++i)
		vAcc += tex2D(BaseSampler, In.vTexUV + Dir * Samples[i] * g_BlurStart);


	vAcc /= 11.f;
	float T = Dist * g_BlurWidth;
	T = clamp(T, 0.f, 1.f);

	Out.vColor = vColor * (1.f - T) + vAcc * T;

	return Out;
}

technique Default_Device
{
	pass Default
	{
		ZWriteEnable = false;
		ZEnable = false;

		// ���̴��ڵ带 �����ϱ����� ������ �Լ��� ����
		// ���� �� � �������� ������ �������� ����
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
