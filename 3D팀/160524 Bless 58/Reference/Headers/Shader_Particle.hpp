#include "Shader_Base.hpp"

texture		g_BaseTexture;
vector		g_vColor;
vector		g_vSize;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
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
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;	
	vector		vWorldNormal : TEXCOORD2;	
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
	Out.vTexUV = In.vTexUV;
	Out.vWorldNormal = vWorldNormal;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldPos : TEXCOORD1;	
	vector			vWorldNormal : TEXCOORD2;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	
	
	vector vColor = tex2D(BaseSampler, In.vTexUV);	

	vector vChangeColor = g_vColor;

	vChangeColor /= 255.f;

	

	/*vColor.rgb *= 0.5f;
	vColor.b *= 2.f;*/

	


	//vColor.rg = 0.f;
	//vColor *= 1.5f;

	
	/*vColor.r *= 0.f;
	vColor.g *= 0.f;
	vColor.b *= 1.f;*/

	Out.vColor = vColor * vChangeColor;

	//Out.vColor.a = 1.f;
	
	



	
	

	//if(vColor.r <= 0.25f  && vColor.g <= 0.25f && vColor.b <= 0.25f)
	//{
	//	Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
	//}
	//else if(vColor.r <= 0.4f  && vColor.g <= 0.4f && vColor.b <= 0.4f)
	//{
	//	Out.vColor.a = 0.8f;
	//}
	//else
	//{
	//	Out.vColor.r = vColor.r * 2.f; 
	//}

	//Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, 0.f);	

	return Out;
}

technique Default_Device
{

	pass Default
	{
		/*ZWriteEnable = false;
		ZEnable = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;*/

		/*Lighting = false;
		CullMode = cw;
		Zwriteenable = false;
		ZEnable = false;*/

		ZWriteEnable = false;
		ZEnable = false;

		//LIGHTING			= FALSE;

		POINTSPRITEENABLE	= TRUE;
		//POINTSCALEENABLE	= TRUE;
		POINTSIZE			= g_vSize.r;
		POINTSIZE_MIN		= 0.f;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		ALPHATESTENABLE	= TRUE;
		ALPHAREF	= 0x00000088;
		ALPHAFUNC = GREATER;


		BLENDOP = ADD;
		SRCBLEND =	ONE;
		DESTBLEND = ONE;	


	

	/*	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		

		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	*/

		//m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		


		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN();
		
		PixelShader = compile ps_3_0 PS_MAIN();
	}

};
