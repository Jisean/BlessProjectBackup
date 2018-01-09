#include "Shader_Base.hpp"

extern float4x4 g_matMatrixPalette[60];
extern int g_iNumBoneInfluences = 3;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector vPosition : POSITION;
	float3 fNormal   : NORMAL;
	float2 vTexUV   : TEXCOORD0;
	vector vWeights  : BLENDWEIGHT0;
	int4   iBoneIndices : BLENDINDICES0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV     : TEXCOORD0;
	vector		vNormal : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;	
	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	vector vPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 fNorm = float3(0.0f, 0.0f, 0.0f);
	
	float	fLastWeight = 0.0f;
	int n = g_iNumBoneInfluences - 1;

	In.fNormal = normalize(In.fNormal);
 
 
	// ������ �����ǰ� �븻�� �����Ѵ�.
	for(int i = 0; i < n; ++i)
	{
		fLastWeight += In.vWeights[i];
		vPos += In.vWeights[i] * mul(In.vPosition, g_matMatrixPalette[In.iBoneIndices[i]]);
		fNorm += In.vWeights[i] * mul(In.fNormal, g_matMatrixPalette[In.iBoneIndices[i]]);
	}
	fLastWeight = 1.0f - fLastWeight;

	vPos += fLastWeight * mul(In.vPosition, g_matMatrixPalette[In.iBoneIndices[n]]);
	fNorm += fLastWeight * mul(In.fNormal, g_matMatrixPalette[In.iBoneIndices[n]]);

	vPos.w = 1.0f;


	Out.vPosition =  mul(vPos, matWVP);

	// UV ��ǥ�� ����.
	Out.vTexUV = In.vTexUV;
	
	//fNorm = normalize(fNorm);
	// ��ְ�  ���� ��ǥ�� ��ȯ�� ��Ű�� ������ �ȵ�  ������ ��..;;;
	//Out.vNormal = vector(fNorm.xyz, 0.f);
	Out.vProjPos = Out.vPosition;
 
	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vNormal : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
};



PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;
	Out.vColor.a = 1.f;

	vector		vWorldNormal = normalize(mul(In.vNormal, g_matWorld));
	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

technique Default_Device
{
	pass Default
	{	 
		// ���̴��ڵ带 �����ϱ����� ������ �Լ��� ����
		// ���� �� � �������� ������ �������� ����
		VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	}
};
