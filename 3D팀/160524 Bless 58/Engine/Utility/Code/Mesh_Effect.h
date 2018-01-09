/*!
 * \file MeshEffect_h__.h
 * \date 2016/02/26 14:33
 *
 * \author 세웅 
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note	Mesh로 만들어지는 이펙트
*/

#ifndef Mesh_Effect_h__
#define Mesh_Effect_h__

#include "GameObject.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CTexture;
class CStaticMesh;
class CShader;
class CTransform;
class ENGINE_DLL CMesh_Effect : public CGameObject
{
	enum RESOURCETYPE {RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_STATIC, RESOURCE_UI, RESOURCE_REMAIN, RESOURCE_END};
	enum EFFECT_STATE {STATE_READY, STATE_START, STATE_IDLE};
	enum FX_TYPE {FX_TARGET, FX_END};

private:
	explicit CMesh_Effect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMesh_Effect(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void	TextureDelete(int iSelect);

	HRESULT	Add_Data(const TCHAR* szMeshKey, const TCHAR* szTextureKey, const TCHAR* szMaskKey, const TCHAR* szNormalKey, MESHEFFECTINFO* pEffectInfo);
	HRESULT	Add_Texture(const TCHAR* szTextureKey, int iSelect);

	vector<CStaticMesh*>*	GetMeshList();
	vector<MESHEFFECTINFO*>* GetMeshInfoList();
	vector<CTexture*>* GetTexList();
	map<const TCHAR*, CComponent*>*	GetCompomentList();
	int*	GetComMeshCount();
	int*	GetComTexCount();
	

public:		// 추가함수
	void	ActionUpdate(_int iCount, const _float& fTimeDelta, _float fGetTime);
	void	EffectReset();
	void	Off_FX();
	void	SingleReset(int iNum);
	void	Call_Effect(D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ, D3DXVECTOR3 vScale);
	void	Call_Effect(D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	void	ShieldCounting();
	void	SetShield();
	void	ShieldInit();
	bool	GetCalling();
	float	GetTime();
	void	SetNumber(int iNumber);
	int		GetNumber();
private:
	virtual HRESULT AddComponent(void);

public:
	static CMesh_Effect* Create(LPDIRECT3DDEVICE9 pDevice);

private:

private:	
	typedef	vector<CStaticMesh*>		VECMESHLIST;
	VECMESHLIST							m_vecMeshList;

	typedef vector<MESHEFFECTINFO*>		VECMESHINFOLIST;
	VECMESHINFOLIST						m_vecMeshInfoList;
	
	typedef vector<CTexture*>			VECTEXLIST;
	VECTEXLIST							m_vecTexList;
	VECTEXLIST							m_vecMaskTexList;
	VECTEXLIST							m_vecNormalTexList;
	D3DXVECTOR3*						m_vTargetPos;
	D3DXVECTOR3							m_vScale;

	CShader*							m_pShader;

	float								m_fTimeDelta;
	float								m_fUnLimitedTime;

	D3DXVECTOR3							m_vStartPos;

	int									m_iComMeshCount;
	int									m_iComTexCount;
	int									m_iComMaskTexCount;
	int									m_iComNormalTexCount;

	vector<const TCHAR*>				m_vecComKey;

	EFFECT_STATE						m_eState;	

	bool								m_bCalling;

	float*								m_pAngle[ANGLE_END];

	float								m_fGetTime;

	int									m_iNumber;

	bool								m_bShield;

	

	//typedef vector<LPDIRECT3DTEXTURE9*>		VECTEXLIST;
	//VECTEXLIST			m_vecTexList;
private:
	//bool		m_bCollision;

private:
	virtual void Free(void);
};

END


#endif // Mesh_Effect_h__