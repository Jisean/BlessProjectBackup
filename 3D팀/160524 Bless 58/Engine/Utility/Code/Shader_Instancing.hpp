#include "Shader_Base.hpp"

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};


struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldNormal : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
};

VS_OUT InstancedVS(
				      vector		vPos : POSITION,
					  vector		vNormal : NORMAL,
					  float2		vTexUV : TEXCOORD0,
					  float4 vInstanceMatrix1 : TEXCOORD1,
					  float4 vInstanceMatrix2 : TEXCOORD2,
					  float4 vInstanceMatrix3 : TEXCOORD3)
{

	// We've encoded the 4x3 world matrix in a 3x4, so do a quick transpose so we can use it in DX
	float4 row1 = float4(vInstanceMatrix1.x,vInstanceMatrix2.x,vInstanceMatrix3.x,0);
	float4 row2 = float4(vInstanceMatrix1.y,vInstanceMatrix2.y,vInstanceMatrix3.y,0);
	float4 row3 = float4(vInstanceMatrix1.z,vInstanceMatrix2.z,vInstanceMatrix3.z,0);
	float4 row4 = float4(vInstanceMatrix1.w,vInstanceMatrix2.w,vInstanceMatrix3.w,1);
	float4x4 mInstanceMatrix = float4x4(row1,row2,row3,row4);

	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(mInstanceMatrix, g_matView);
	matWVP = mul(matWV, g_matProj);

	vector		vWorldNormal = normalize(mul(vector(vNormal.xyz, 0.f), mInstanceMatrix));

	Out.vPosition = mul(vector(vPos.xyz, 1.f), matWVP);
	Out.vTexUV = vTexUV;
	Out.vWorldNormal = vector(vNormal.xyz, 1.f);
	Out.vProjPos = Out.vPosition;

	return Out;

}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	vector			vWorldNormal : TEXCOORD1;
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

	Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}
//-----------------------------------------------------------------------------
// Name: PS
// Type: Pixel shader
// Desc: This shader outputs the pixel's color by modulating the texture's
//		 color with diffuse material color
//-----------------------------------------------------------------------------
technique RenderSceneInstance
{
	pass P0
	{    
		cullmode = none;      
		VertexShader = compile vs_3_0 InstancedVS();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}