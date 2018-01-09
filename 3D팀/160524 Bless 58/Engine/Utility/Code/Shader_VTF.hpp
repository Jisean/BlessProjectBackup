#include "Shader_Base.hpp"

extern int g_iNumBoneInfluences;
vector		g_vLightPos;
vector		g_vCamPos;

matrix		g_matSLightWVP;
matrix		g_matDLightWVP;


float		g_fWeaponCheck = 0.f;
float		g_fSpecMapping = 0.f;

float		g_fSpecRange = 5.f;
float		g_fSpecPow = 1.f;

float		g_fFogRender = 0.f;
vector		g_vFog = vector(0.33f, 15.f, 0.f, 0.f);

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_NormalTexture;
sampler NormalSampler = sampler_state
{
	Texture = g_NormalTexture;
};


texture		g_SpecularTexture;
sampler SpecularSampler = sampler_state
{
	Texture = g_SpecularTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};


texture		g_MatrixTexture;

sampler MatrixSampler = sampler_state
{
	Texture = g_MatrixTexture;
};

//	Static Z Buffer
texture		g_SShadowTexture;
sampler SShadowSampler = sampler_state
{
	Texture = g_SShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};

// Dynamic Z Buffer
texture		g_DShadowTexture;
sampler DShadowSampler = sampler_state
{
	Texture = g_DShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};

// JiHyung's Hot Item
matrix GetSkinMatrix(int4 iIndex)
{
	int idx = iIndex;
	float4 uvCol = float4(((float)((idx % 8) * 4) + 0.5f) / 32.0f, ((float)((idx / 8)) + 0.5f) / 32.0f, 0.0f, 0.0f);

	matrix mat = 
	{
		tex2Dlod(MatrixSampler, uvCol),
		tex2Dlod(MatrixSampler, uvCol + float4(1.0f / 32.0f, 0, 0, 0)),
		tex2Dlod(MatrixSampler, uvCol + float4(2.0f / 32.0f, 0, 0, 0)),
		tex2Dlod(MatrixSampler, uvCol + float4(3.0f / 32.0f, 0, 0, 0))
	};

	return mat;
}
// JiHyung's Hot Item


vector SpecularMapping(float fCheckSpec, float3 vReflection, float3 vViewDir, float2 fTexUV)
{
	float3 vReflect = normalize(vReflection);
	float3 ViewDir = normalize(vViewDir);

	vector vSpecular = dot(vReflect, -ViewDir);

	if(fCheckSpec == 0.f)
	{
		vector SpecularIntensity = tex2D(SpecularSampler, fTexUV);
		vSpecular.rgb *= SpecularIntensity.rgb;
		vSpecular = vector(vSpecular.rgb, 1.f);
	}
	else
	{
		vSpecular = pow(abs(vSpecular), g_fSpecRange);
		vSpecular.a = 1.f;
		vSpecular *= g_fSpecPow;
	}

	return vSpecular;
}

vector NormalMapping(vector vNormalMap, float3 vT, float3 vB, float3 vN, float fProjPosW)
{
	//// 노말텍스쳐 없는거
	//if(vNormal.x == 1.f && vNormal.y == 1.f && vNormal.z == 1.f)
	//{
	//	vector		vWorldNormal = normalize(mul(In.vNormal, g_matWorld));	
	//	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	//}
	//else		//  노말텍스쳐 있는거
	//{
	vector vNormal = normalize(vNormalMap * 2.f - 1.f);

	float3x3 TBN = float3x3(normalize(vT), normalize(vB), normalize(vN));
	TBN = transpose(TBN);

	float3 vWorldNormal = mul(TBN, vNormal);
	
	return vector(vWorldNormal.xyz * 0.5f + 0.5f, fProjPosW / 1000.f);
	//}
}

vector Fog(vector vViewPos)
{
	// Fog
	float fDensity = g_vFog.x;
	float fFogEnd = g_vFog.y;

	float fDist = vViewPos.z / fFogEnd * 4.f;

	return	exp(-fDist * fDensity);
}




struct VS_IN
{
	vector vPosition : POSITION;
	float3 fNormal   : NORMAL;
	float2 vTexUV   : TEXCOORD0;

	vector vWeights  : BLENDWEIGHT0;
	int4   iBoneIndices : BLENDINDICES0;

	////// 노말매핑
	float3		vTangent	: TANGENT;
	float3		vBinormal	: BINORMAL;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV     : TEXCOORD0;
	//vector		vNormal : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
	vector		vLightProj : TEXCOORD3;
	vector		vSubLightProj : TEXCOORD4;

	//// 노말매핑
	float3		vT			: COLOR0;
	float3		vB			: COLOR1;
	float3		vN			: COLOR2;

	// 스펙큘러 매핑
	float3		vReflection	: COLOR3;
	float3		vViewDir	: COLOR4;

	vector		vFogValue	: COLOR5;
};

// Shadow None Dynamic
VS_OUT VS_MAIN_DYNAMIC( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;
	vector	vViewPos;

	matrix			matWV, matWVP;	
	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	vector vPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 fNorm = float3(0.0f, 0.0f, 0.0f);

	float	fLastWeight = 0.0f;
	int n = g_iNumBoneInfluences - 1;

	In.fNormal = normalize(In.fNormal);


	if(g_fWeaponCheck == 0.f)
	{
		matrix matSkinMatrix;

		// 정점의 포지션과 노말을 블렌딩한다.
		for(int i = 0; i < n; ++i)
		{
			fLastWeight += In.vWeights[i];

			matSkinMatrix = GetSkinMatrix(In.iBoneIndices[i]);

			vPos += In.vWeights[i] * mul(vector(In.vPosition.xyz, 1.f), matSkinMatrix);
			fNorm += In.vWeights[i] * mul(In.fNormal, matSkinMatrix);
		}
		fLastWeight = 1.0f - fLastWeight;

		matSkinMatrix = GetSkinMatrix(In.iBoneIndices[n]);

		vPos += fLastWeight * mul(vector(In.vPosition.xyz, 1.f), matSkinMatrix);
		fNorm += fLastWeight * mul(In.fNormal, matSkinMatrix);

		vPos.w = 1.0f;
		Out.vPosition =  mul(vPos, matWVP);
		vViewPos = mul(vPos, matWV);
	}
	else
	{
		Out.vPosition = mul(In.vPosition, matWVP);
		vViewPos = mul(In.vPosition, matWV);
	}
	


	// UV 좌표를 복사.
	Out.vTexUV = In.vTexUV;

	//fNorm = normalize(fNorm);
	// 노멀값  월드 좌표로 변환시 스키닝 적용이 안됨  이유는 모름..;;;
	//Out.vNormal = vector(fNorm.xyz, 0.f);
	Out.vProjPos = Out.vPosition;

	///// 노말매핑
	Out.vN = normalize(mul(In.fNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));


	// Specular Mapping
	float3 vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	vLightDir = normalize(vLightDir);

	Out.vReflection = reflect(vLightDir, Out.vN);
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);

	Out.vFogValue.x = 1.f;

	if(g_fFogRender == 1.f)
		Out.vFogValue = Fog(vViewPos);

	return Out;
}

// Shadow None Static
VS_OUT VS_MAIN_STATIC( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	In.vPosition.w = 1.f;
	Out.vPosition = mul(In.vPosition, g_matWorld);
	vector vViewPos = Out.vPosition = mul(Out.vPosition, g_matView);
	Out.vPosition = mul(Out.vPosition, g_matProj);

	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	// Normal Mapping
	Out.vN = normalize(mul(In.fNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));

	// Specular Mapping
	float3 vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	vLightDir = normalize(vLightDir);

	Out.vReflection = reflect(vLightDir, Out.vN);
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);


	Out.vFogValue.x = 1.f;

	if(g_fFogRender == 1.f)
		Out.vFogValue = Fog(vViewPos);

	return Out;
}

// Shadow	Dynamic Vertex Shader
VS_OUT VS_DMAIN( VS_IN In )
{
	//VS_OUT			Out = (VS_OUT)0;

	//matrix			matWV, matWVP;

	//matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);

	//// vnormal -> fnormal
	//vector		vWorldNormal = normalize(mul(vector(In.fNormal, 0.f), g_matWorld));

	//Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	//Out.vTexUV = In.vTexUV;
	//// vWorldNormal -> vNormal
	//Out.vNormal = vWorldNormal;
	//Out.vProjPos = Out.vPosition;

	//Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);
	//Out.vSubLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);

	//return Out;

	///////////////////////////////////////
	VS_OUT			Out = (VS_OUT)0;
	vector	vViewPos;

	matrix			matWV, matWVP;	
	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	vector vPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 fNorm = float3(0.0f, 0.0f, 0.0f);

	float	fLastWeight = 0.0f;
	int n = g_iNumBoneInfluences - 1;

	In.fNormal = normalize(In.fNormal);

	if(g_fWeaponCheck == 0.f)
	{
		matrix matSkinMatrix;

		// 정점의 포지션과 노말을 블렌딩한다.
		for(int i = 0; i < n; ++i)
		{
			fLastWeight += In.vWeights[i];

			matSkinMatrix = GetSkinMatrix(In.iBoneIndices[i]);

			vPos += In.vWeights[i] * mul(vector(In.vPosition.xyz, 1.f), matSkinMatrix);
			fNorm += In.vWeights[i] * mul(In.fNormal, matSkinMatrix);
		}
		fLastWeight = 1.0f - fLastWeight;

		matSkinMatrix = GetSkinMatrix(In.iBoneIndices[n]);

		vPos += fLastWeight * mul(vector(In.vPosition.xyz, 1.f), matSkinMatrix);
		fNorm += fLastWeight * mul(In.fNormal, matSkinMatrix);

		vPos.w = 1.0f;
		Out.vPosition =  mul(vPos, matWVP);
		vViewPos = mul(vPos, matWV);
	}
	else
	{
		Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
		vViewPos = mul(In.vPosition, matWV);
	}

	// UV 좌표를 복사.
	Out.vTexUV = In.vTexUV;

	//fNorm = normalize(fNorm);
	// 노멀값  월드 좌표로 변환시 스키닝 적용이 안됨  이유는 모름..;;;
	//Out.vNormal = vector(fNorm.xyz, 0.f);
	Out.vProjPos = Out.vPosition;

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);
	Out.vSubLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);


	///// 노말매핑
	Out.vN = normalize(mul(In.fNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));


	// Specular Mapping
	float3 vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	vLightDir = normalize(vLightDir);

	Out.vReflection = reflect(vLightDir, Out.vN);
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);





	Out.vFogValue.x = 1.f;

	if(g_fFogRender == 1.f)
		Out.vFogValue = Fog(vViewPos);


	return Out;
}

// Static Vertex Shader
VS_OUT VS_SMAIN( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;
	vector			vViewPos;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	Out.vN = normalize(mul(In.fNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));

	Out.vLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matSLightWVP);
	Out.vSubLightProj = mul(vector(In.vPosition.xyz, 1.f), g_matDLightWVP);


	// Specular Mapping
	float3 vLightDir = mul(In.vPosition, g_matWorld).xyz - g_vLightPos.xyz;
	vLightDir = normalize(vLightDir);

	Out.vReflection = reflect(vLightDir, Out.vN);
	Out.vViewDir = normalize(mul(In.vPosition, g_matWorld).xyz - g_vCamPos.xyz);


	vViewPos = mul(In.vPosition, matWV);
	Out.vFogValue.x = 1.f;

	if(g_fFogRender == 1.f)
		Out.vFogValue = Fog(vViewPos);


	return Out;
}



struct PS_IN
{
	float2			vTexUV : TEXCOORD0;		
	//vector			vNormal : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;
	vector			vLightProj : TEXCOORD3;
	vector			vSubLightProj : TEXCOORD4;
	/////////////////노말매핑
	float3			vT			: COLOR0;
	float3			vB			: COLOR1;
	float3			vN			: COLOR2;

	// 스펙큘러 매핑
	float3			vReflection	: COLOR3;
	float3			vViewDir	: COLOR4;

	vector			vFogValue	: COLOR5;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
	vector			vSpecular : COLOR3;
};


// Defulat
PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	vector		vColor = tex2D(BaseSampler, In.vTexUV);
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vColor = vColor;
	Out.vColor.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, In.vFogValue.x);

	// 노말 맵핑
	Out.vNormal = NormalMapping(vNormal, In.vT, In.vB, In.vN, In.vProjPos.w);

	// 스펙큘러 맵핑
	Out.vSpecular = SpecularMapping(g_fSpecMapping, In.vReflection, In.vViewDir, In.vTexUV);
	Out.vSpecular.a = In.vProjPos.z / In.vProjPos.w;

	return Out;
}

// Static Pixel Shader
PS_OUT PS_SMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vColor = vColor;
	Out.vColor.a = 1.f;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, In.vFogValue.x);

	
	// Dynamic Shadow
	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(SShadowSampler, fNewUV).r / 0.5f;


	// Static Shadow
	float fDCurrentDepth = In.vSubLightProj.z / In.vSubLightProj.w;

	float2 fDNewUV = (float2)0;
	fDNewUV.x = (In.vSubLightProj.x / In.vSubLightProj.w) * 0.5f + 0.5f;
	fDNewUV.y = (In.vSubLightProj.y / In.vSubLightProj.w) * -0.5f + 0.5f;

	float fDShadowDepth = tex2D(DShadowSampler, fDNewUV).r / 0.5f;

	if(fCurrentDepth > fShadowDepth + 0.0005f ||
		fDCurrentDepth > fDShadowDepth + 0.0000125f)
		Out.vDepth.z = 0.1f;

	// 노말 맵핑
	Out.vNormal = NormalMapping(vNormal, In.vT, In.vB, In.vN, In.vProjPos.w);

	// 스펙큘러 맵핑
	Out.vSpecular = SpecularMapping(g_fSpecMapping, In.vReflection, In.vViewDir, In.vTexUV);
	Out.vSpecular.a = In.vProjPos.z / In.vProjPos.w;

	return Out;
}

// Dynamic Pixel Shader
PS_OUT PS_DMAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV);
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vColor = vColor;
	Out.vColor.a = 1.f;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, In.vFogValue.x);


	// Dynamic Shadow
	float fCurrentDepth = In.vLightProj.z / In.vLightProj.w;

	float2 fNewUV = (float2)0;
	fNewUV.x = (In.vLightProj.x / In.vLightProj.w) * 0.5f + 0.5f;
	fNewUV.y = (In.vLightProj.y / In.vLightProj.w) * -0.5f + 0.5f;

	float fShadowDepth = tex2D(DShadowSampler, fNewUV).r / 0.5f;

	// Static Shadow
	float fSCurrentDepth = In.vSubLightProj.z / In.vSubLightProj.w;

	float2 fSNewUV = (float2)0;
	fSNewUV.x = (In.vSubLightProj.x / In.vSubLightProj.w) * 0.5f + 0.5f;
	fSNewUV.y = (In.vSubLightProj.y / In.vSubLightProj.w) * -0.5f + 0.5f;

	float fSShadowDepth = tex2D(SShadowSampler, fSNewUV).r / 0.5f;

	if(fCurrentDepth > fShadowDepth + 0.0015f ||
		fSCurrentDepth > fSShadowDepth + 0.0005f)
		Out.vDepth.z = 0.1f;


	// 노말 맵핑
	Out.vNormal = NormalMapping(vNormal, In.vT, In.vB, In.vN, In.vProjPos.w);

	// 스펙큘러 맵핑
	Out.vSpecular = SpecularMapping(g_fSpecMapping, In.vReflection, In.vViewDir, In.vTexUV);
	Out.vSpecular.a = In.vProjPos.z / In.vProjPos.w;

	return Out;
}

// Static Z Buffer 
VS_OUT VS_SShadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(In.vPosition, g_matSLightWVP);
	Out.vProjPos = Out.vPosition;


	return Out;
}

// Dynamic Z Buffer
VS_OUT VS_DShadow_Depth( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	vector vPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 fNorm = float3(0.0f, 0.0f, 0.0f);

	float	fLastWeight = 0.0f;
	int n = g_iNumBoneInfluences - 1;

	In.fNormal = normalize(In.fNormal);

	if(g_fWeaponCheck == 0.f)
	{
		matrix matSkinMatrix;
		// 정점의 포지션과 노말을 블렌딩한다.
		for(int i = 0; i < n; ++i)
		{
			fLastWeight += In.vWeights[i];

			matSkinMatrix = GetSkinMatrix(In.iBoneIndices[i]);

			vPos += In.vWeights[i] * mul(In.vPosition, matSkinMatrix);
			fNorm += In.vWeights[i] * mul(In.fNormal, matSkinMatrix);
		}
		fLastWeight = 1.0f - fLastWeight;

		matSkinMatrix = GetSkinMatrix(In.iBoneIndices[n]);

		vPos += fLastWeight * mul(In.vPosition, matSkinMatrix);
		fNorm += fLastWeight * mul(In.fNormal, matSkinMatrix);

		vPos.w = 1.0f;

		//Out.vPosition =  mul(vPos, matWVP);
		Out.vPosition = mul(vPos, g_matDLightWVP);
	}
	else
		Out.vPosition = mul(In.vPosition, g_matDLightWVP);
	

	// UV 좌표를 복사.
	Out.vTexUV = In.vTexUV;

	fNorm = normalize(fNorm);
	// 노멀값  월드 좌표로 변환시 스키닝 적용이 안됨  이유는 모름..;;;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_Shadow_OUT
{
	vector			vDepth : COLOR0;
};

PS_Shadow_OUT PS_Shadow_Depth( PS_IN In )
{
	PS_Shadow_OUT			Out = (PS_Shadow_OUT)0;	

	float vDepth = (In.vProjPos.z / In.vProjPos.w) * 0.5f;
	Out.vDepth = vDepth;

	return Out;
}




VS_OUT VS_SkyDome( VS_IN In )
{
	VS_OUT			Out = (VS_OUT)0;

	In.vPosition.w = 1.f;
	Out.vPosition = mul(In.vPosition, g_matWorld);
	Out.vPosition = mul(Out.vPosition, g_matView);
	Out.vPosition = mul(Out.vPosition, g_matProj);

	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	Out.vN = normalize(mul(In.fNormal, (float3x3)g_matWorld));
	Out.vT = normalize(mul(In.vTangent, (float3x3)g_matWorld));
	Out.vB = normalize(mul(In.vBinormal, (float3x3)g_matWorld));

	return Out;
}


PS_OUT PS_SkyDome( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;	

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = normalize(vNormal * 2.f - 1.f);

	float3x3 TBN = float3x3(normalize(In.vT), normalize(In.vB), normalize(In.vN));
	TBN = transpose(TBN);

	float3 vWorldNormal = mul(TBN, vNormal);
	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);
	//Out.vNormal = vector(In.vN.xyz * 0.5f + 0.5f, In.vProjPos.w / 1000.f);

	Out.vNormal = vector(0.8f, 1.f, 0.8f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.f, 1.f);

	return Out;
}


// Shaodw None Dynamic
technique Default_Device
{
	pass Default
	{	 
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN_DYNAMIC();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};

// Shaodw None Static
technique Default
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_MAIN_STATIC();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};

// Static BackBuffer Draw
technique S_Draw_Default
{
	pass Default
	{
		// 셰이더코드를 실행하기위한 진입점 함수를 결정
		// 번역 시 어떤 버젼으로 번역할 것인지를 결정
		VertexShader = compile vs_3_0 VS_SMAIN();
		PixelShader = compile ps_3_0 PS_SMAIN();
	}
};

// Dynamic BackBuffer Draw
technique D_Draw_Default
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_DMAIN();
		PixelShader = compile ps_3_0 PS_DMAIN();
	}
};

// Static Z Buffer Draw
technique S_Draw_Shadow
{
	pass Depth_Shadow
	{
		VertexShader = compile vs_3_0 VS_SShadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}

// Dynamic Z Buffer Draw
technique D_Draw_Shadow
{
	pass Depth_Shadow
	{
		VertexShader = compile vs_3_0 VS_DShadow_Depth();
		PixelShader = compile ps_3_0 PS_Shadow_Depth();
	}
}


// SkyDome
technique SkyDome
{
	pass SkyDome
	{
		LIGHTING	= FALSE;
		VertexShader = compile vs_3_0 VS_SkyDome();
		PixelShader = compile ps_3_0 PS_SkyDome();
	}
}


