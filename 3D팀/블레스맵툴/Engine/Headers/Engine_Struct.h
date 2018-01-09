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
		LPD3DXMESH				pOriMesh;
		D3DXMATRIX*				pFrameOffsetMatrix;
		D3DXMATRIX**			ppFrameCombinedMatrix;
		D3DXMATRIX*				pResultMatrix;
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
}

#endif // Engine_Struct_h__