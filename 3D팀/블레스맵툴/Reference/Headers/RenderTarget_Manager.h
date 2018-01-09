#ifndef RenderTarget_Manager_h__
#define RenderTarget_Manager_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CRenderTarget_Manager : public CBase
{
	DECLARE_SINGLETON(CRenderTarget_Manager)
private:
	explicit CRenderTarget_Manager(void);
	virtual ~CRenderTarget_Manager(void);
public:
	void SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, const char* pConstantName);
public:
	HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag);
	HRESULT Ready_DebugBuffer(const TCHAR* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void Begin_MRT(const TCHAR* pMRTTag);
	void End_MRT(const TCHAR* pMRTTag);

	void Render_DebugBuffer(const TCHAR* pMRTTag);
private:
	map<const TCHAR*, CRenderTarget*>				m_mapTargets;
	typedef map<const TCHAR*, CRenderTarget*>		MAPTARGET;

	map<const TCHAR*, list<CRenderTarget*>>			m_mapMRT;
	typedef map<const TCHAR*, list<CRenderTarget*>>	MAPMRT;
private:
	CRenderTarget* Find_RenderTarget(const TCHAR* pTargetTag);
	list<CRenderTarget*>* Find_MRT(const TCHAR* pMRTTag);
public:
	virtual void Free(void);
};

END

#endif // RenderTarget_Manager_h__
