/*!
 * \file Engine_Struct.h
 * \date 2015/04/04 23:03
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagMeshContainer_Derived
		: public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNormTexture;
		LPDIRECT3DTEXTURE9*     ppSpecTexture;
		LPD3DXMESH				pOriMesh;
		D3DXMATRIX*				pFrameOffsetMatrix;
		D3DXMATRIX**			ppFrameCombinedMatrix;
		D3DXMATRIX*				pResultMatrix;

		DWORD					NumAttributeGroups;              // 속성 그룹의 수
		LPD3DXBUFFER			pBoneCombinationBuf;

		INT						iBoneInfluences;
	}DERIVED_MESHCONTAINER;

	typedef struct tagFrame_Derived
		: public D3DXFRAME
	{
		D3DXMATRIX		CombinedMatrix;
	}DERIVED_FRAME;
	

	typedef struct tagVertexScreen
	{
		D3DXVECTOR4				vPos;
		D3DXVECTOR2				vTexUV;
	}VTXSCREEN;
	const DWORD VTXFVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTexUV;
	}VTXTEX;
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexColor
	{
		D3DXVECTOR3			vPos;
		DWORD				dwColor;
	}VTXCOL;
	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vTex;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;

	// MATRIX
	typedef struct MYMATRIX : public D3DXMATRIX
	{
	public:
		MYMATRIX(void){};
		MYMATRIX( const MYMATRIX& );
		MYMATRIX( const D3DXMATRIX matInit ){ memcpy(this, &matInit, sizeof(D3DXMATRIX)); };
	public:
		// 초기화
		void		SetMat( D3DXMATRIX matInit){ memcpy(this, &matInit, sizeof(D3DXMATRIX)); }
		void		SetMat( D3DXVECTOR3 vRight, D3DXVECTOR3 vUp, D3DXVECTOR3 vLook, D3DXVECTOR3 vPos )
		{
			D3DXMatrixIdentity(this);
			memcpy_s(this->m[0], sizeof(D3DXVECTOR3), &vRight, 1);
			memcpy_s(this->m[1], sizeof(D3DXVECTOR3), &vUp, 1);
			memcpy_s(this->m[2], sizeof(D3DXVECTOR3), &vLook, 1);
			memcpy_s(this->m[3], sizeof(D3DXVECTOR3), &vPos, 1);
		}
		// 포인터 겟
		const D3DXMATRIX*	GetPointer(void){ return this; }
		// 멤버함수
		D3DXMATRIX*	Identity(void){ return D3DXMatrixIdentity(this); }
		D3DXMATRIX*	Inverse(D3DXMATRIX* pM){ return D3DXMatrixInverse(this, 0, pM); }
		D3DXMATRIX*	InverseThis(D3DXMATRIX* pOut){ return D3DXMatrixInverse(pOut, 0, this); }
		// Scaling
		D3DXMATRIX* Scale( _float fX, _float fY, _float fZ ){ return D3DXMatrixScaling(this, fX, fY, fZ); }
		D3DXMATRIX* Scale( D3DXVECTOR3 vScale ){ return D3DXMatrixScaling( this, vScale.x, vScale.y, vScale.z ); }
		// Rotation
		D3DXMATRIX* RotX(_float fAngle){ return D3DXMatrixRotationX(this, fAngle); }
		D3DXMATRIX* RotY(_float fAngle){ return D3DXMatrixRotationY(this, fAngle); }
		D3DXMATRIX* RotZ(_float fAngle){ return D3DXMatrixRotationZ(this, fAngle); }
		D3DXMATRIX* RotAxis(const D3DXVECTOR3* pV, _float fAngle){ return D3DXMatrixRotationAxis(this, pV, fAngle); }
		// Translation
		D3DXMATRIX* Trans( _float fX, _float fY, _float fZ ){ return D3DXMatrixTranslation(this, fX, fY, fZ); }
		D3DXMATRIX* Trans( D3DXVECTOR3 vTrans ){ return D3DXMatrixTranslation( this, vTrans.x, vTrans.y, vTrans.z ); }
	}_mymat;

	// VECTOR
	typedef struct MYVECTOR : public D3DXVECTOR3
	{
	public:
		MYVECTOR(void){};
		MYVECTOR( const MYVECTOR& );
		MYVECTOR( const D3DXVECTOR3 vInit ){ this->x = vInit.x; this->y = vInit.y; this->z = vInit.z; }
		MYVECTOR( _float fX, _float fY, _float fZ ){ this->x = fX; this->y = fY; this->z = fZ; }
	public:
		// 초기화
		void	SetVec( D3DXVECTOR3 vInit ){ this->x = vInit.x; this->y = vInit.y; this->z = vInit.z; }
		void	SetVec( _float fX, _float fY, _float fZ ){ this->x = fX; this->y = fY; this->z = fZ; }
		void	SetVec( _float* pf ){ memcpy(this, pf, sizeof(D3DXVECTOR3)); }
		// 포인터 겟
		const D3DXVECTOR3*	GetPointer(void){ return this; }
		// 멤버함수
		D3DXVECTOR3*	Normalize(void){ return D3DXVec3Normalize(this, this); }
		D3DXVECTOR3*	CrossFront( D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1 ){	return D3DXVec3Cross( pOut, this, pV1 ); }
		D3DXVECTOR3*	CrossBack( D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1 ){	return D3DXVec3Cross( pOut, pV1, this ); }
		_float	Dot( const D3DXVECTOR3* pV1 ){ return D3DXVec3Dot(this, pV1); }
		_float	Length(void){ return D3DXVec3Length(this); }
		D3DXVECTOR3*	TransformCoord( D3DXVECTOR3* pOut, D3DXMATRIX* pM ){ return D3DXVec3TransformCoord(pOut, this, pM); }
		D3DXVECTOR3*	TransformNormal( D3DXVECTOR3* pOut, D3DXMATRIX* pM ){ return D3DXVec3TransformNormal(pOut, this, pM); }
	}_myvec;

	/////////////////애니////////////////////////////////////
	typedef struct tagAniInfo
	{
		TCHAR			  tName[15];
		WORD			  wIndex;
		WORD			  wIdleIndex;

		float			  fSpeed;	 
		float			  fWeight;
		float			  fDelay;
		float			  fPeriod;

		float			  fEffectStart;

		D3DXVECTOR3		  vPos;
		D3DXVECTOR3		  vScale;

	}ANI_INFO;

	typedef struct tagBoxInfo
	{
		TCHAR			tKeyName[15];
		TCHAR			tBoneName[30];
		D3DXVECTOR3		vMin;
		D3DXVECTOR3		vMax;
		D3DXVECTOR3		vSpherePos;
		float			fRadius;
	}BOX_INFO;

	//===========================파티클=============================//

	typedef struct tagParticle
	{
		D3DXVECTOR3	vPos;
		D3DXVECTOR4	vColor;
	}PARTICLE;

	const DWORD	PARTICLE_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tageAttribute
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vDir;
		D3DXVECTOR3	vAccel;
		float		fSpeed;
		float		fAccel;
		float		fLifeTime;
		float		fAge;
		float		fSize;
		bool		bCreate;
		D3DXVECTOR4	vColor;
		DWORD		dwColorFade;
		bool		bAlive;
		float		fViewZ;
	}ATTRIBUTE;


	typedef struct tagBox
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;

		tagBox()
		{			
			// infinite small
			//FLT_MAX
			vMin.x = FLT_MAX;
			vMin.y = FLT_MAX;
			vMin.z = FLT_MAX;

			vMax.x = -FLT_MAX;
			vMax.y = -FLT_MAX;
			vMax.z = -FLT_MAX;
		}
		bool isPointInside(D3DXVECTOR3& p)
		{
			if( p.x >= vMin.x && p.y >= vMin.y && p.z >= vMin.z &&
				p.x <= vMax.x && p.y <= vMax.y && p.z <= vMax.z )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	}BOX;


	typedef struct tagCircle
	{
		ANGLE		eDirAngle;
		D3DXVECTOR3	vPos;		
		int			iRadius;
		int			iAngle;

	}CIRCLE;


	typedef struct tagParticleInfo
	{
		wstring			wstrTexturePath;
		float			fSize;
		float			fSpeed;
		float			fLifeTime;
		int				iMaxCount;
		PARTICLE_SHAPE	eShape;
		BOX*			pBox;
		CIRCLE*			pCircle;
		D3DXVECTOR4		vColor;

	}PARTICLEINFO;

	// 메쉬 이펙트

	typedef struct tagPosAction
	{
		D3DXVECTOR3	vPosValue;
		float		fStart_Time;
		float		fEnd_Time;
		float		fSpeed;
		float		fAcc;

	}POSACTION;

	typedef struct tagRotAction
	{
		D3DXVECTOR3	vRotValue;
		float		fStart_Time;
		float		fEnd_Time;
		float		fSpeed;
		float		fAcc;		
	}ROTACTION;

	typedef struct tagScaleAction
	{
		D3DXVECTOR3	vScaleValue;
		float		fStart_Time;
		float		fEnd_Time;
		float		fSpeed;
		float		fAcc;		
	}SCALEACTION;

	typedef struct tagFadeAction
	{
		float		fFadeValue;
		float		fStart_Time;
		float		fEnd_Time;
		float		fSpeed;
		float		fAcc;
	}FADEACTION;

	typedef struct tagUVAction
	{
		float		fU;
		float		fV;	
		float		fStart_Time;
		float		fEnd_Time;
		float		fSpeedU;
		float		fAccU;
		float		fSpeedV;
		float		fAccV;
	}UVACTION;

	typedef struct tagUVSizeAction
	{
		float		fUSize;
		float		fVSize;		
		float		fStart_Time;
		float		fEnd_Time;	
	}UVSIZEACTION;

	typedef struct tagColorAction
	{
		D3DXVECTOR3	vRGB;		
		float		fNext_Time;
		float		fChange_Time;
	}COLORACTION;


	typedef struct tagMeshEffectInfo
	{		
		_mymat	matWorld;

		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vOriPos;
		D3DXVECTOR3 vRot;
		D3DXVECTOR3 vOriRot;
		D3DXVECTOR3 vScale;
		D3DXVECTOR3 vOriScale;		

		D3DXVECTOR3 vRGB;
		D3DXVECTOR3	vRGBDifference;

		vector<POSACTION>	vecPosAction;
		vector<ROTACTION>	vecRotAction;
		vector<SCALEACTION> vecScaleAction;
		vector<FADEACTION>	vecFadeAction;
		vector<UVACTION>	vecUVAction;
		vector<UVSIZEACTION>vecUVSizeAction;
		vector<COLORACTION>	vecColorAction;

		int		iPass;

		int		iActionIndex[ACT_END];
		int		iActionMaxIndex[ACT_END];

		int		iSpriteCount;
		int		iSpriteMaxCount;
		int		iSpriteX;
		int		iSpriteY;

		int		iCycle;
		int		iOriCycle;

		float	fGetTime;

		float	fAliveStart_Time;
		float	fAliveEnd_Time;

		float	fUnLimitedTime[ACT_END];

		float	fSpriteTotalTime;		
		float	fSpriteTime;

		float	fColorTime;
		float	fColorChangeTime;	

		float	fFade;
		float	fOriFade;

		float	fUPos;
		float	fVPos;

		float	fOriUPos;
		float	fOriVPos;

		float	fUSize;
		float	fVSize;
		float	fOriUSize;
		float	fOriVSize;

		float	fViewZ;

		bool	bShield;
		bool	bParRot;
		bool	bAliveOn;	
		bool	bColorOn;
		bool	bBillOn;
		bool	bSingleBillOn;
		bool	bHoldOn;
		bool	bCustomOn;
		bool	bEnd;
		bool	bUnLimited[ACT_END];

	}MESHEFFECTINFO;

	typedef	struct	tagSaveData
	{
		TCHAR  szMeshKey[128];
		TCHAR  szMeshPath[128];
		TCHAR  szTextureKey[128];
		TCHAR  szTexturePath[128];
		TCHAR  szMaskKey[128];
		TCHAR  szMaskPath[128];
		TCHAR  szNormalKey[128];
		TCHAR  szNormalPath[128];

		int		iActionMaxIndex[ACT_END];

		vector<POSACTION>	vecPosAction;
		vector<ROTACTION>	vecRotAction;
		vector<SCALEACTION> vecScaleAction;
		vector<FADEACTION>	vecFadeAction;
		vector<UVACTION>	vecUVAction;
		vector<UVSIZEACTION>vecUVSizeAction;
		vector<COLORACTION>	vecColorAction;

		D3DXVECTOR3 vOriPos;		
		D3DXVECTOR3 vOriRot;		
		D3DXVECTOR3 vOriScale;

		int			iOriCycle;		

		int			iPass;

		int			iSpriteMaxCount;
		int			iSpriteX;
		int			iSpriteY;

		float		fSpriteTotalTime;	

		float		fFade;
		float		fOriFade;		

		float		fOriUPos;
		float		fOriVPos;

		float		fOriUSize;
		float		fOriVSize;

		float		fAliveStart_Time;
		float		fAliveEnd_Time;

		bool		bColorOn;
		bool		bBillOn;
		bool		bSingleBillOn;
		bool		bHoldOn;		
		bool		bUnLimited[ACT_END];	

	}SAVEDATA;

	//================== Static Instancing ===================//
	class CStaticMesh;
	typedef struct tagInstanceData
	{
		vector<D3DXMATRIX*>					vecWorldMat;

		LPDIRECT3DVERTEXBUFFER9 			pVB;
		LPDIRECT3DVERTEXBUFFER9				pInsVB;
		LPDIRECT3DINDEXBUFFER9  			pIB;

		D3DXATTRIBUTERANGE*					pAttributeTable;
		DWORD								dTexIndex;

		IDirect3DVertexDeclaration9*		pMeshVertexDecl;
		int									iMaxIndex;
		CStaticMesh*						pStaticMesh;

	}INSTANSDATA;

	const D3DVERTEXELEMENT9 g_MeshVertDecl[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},	
		{0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{1, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{1, 16, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{1, 32, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		D3DDECL_END()
	};

	struct MatrixVertex
	{
	public:
		MatrixVertex(D3DXVECTOR4& i1,D3DXVECTOR4& i2,D3DXVECTOR4& i3)
			: r1(i1),r2(i2),r3(i3) {}
		D3DXVECTOR4 r1;     // row 1
		D3DXVECTOR4 r2;     // row 2
		D3DXVECTOR4 r3;		// row 3
	};

	//==================  ===================//

	////////////////////////////////////////////////////////////////////////// ji hyeong Add
	typedef	struct	tagAttackPosition
	{
		D3DXVECTOR3	vPos;			// 자리
		bool		bPossession;	// 소유
		float       fAttDist;
	}ATTPOSITION;
}

#endif // Engine_Struct_h__