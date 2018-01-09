#include "FontMgr.h"
#include "Font.h"

IMPLEMENT_SINGLETON(Engine::CFontMgr)

Engine::CFontMgr::CFontMgr(void)
{
	m_htFont.Ready_Table(19);
}

Engine::CFontMgr::~CFontMgr(void)
{

}

HRESULT Engine::CFontMgr::AddFont(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey , const UINT& iHeight, const UINT& iWidth, const UINT& iWeight)
{
	CFont*	pFont = (CFont*)m_htFont.Search_TableData(pFontKey);
	if(NULL != pFont)
		return E_FAIL;

	pFont = CFont::Create(pDevice, iHeight, iWidth, iWeight, pFontKey);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_htFont.Insert_Table(pFontKey, pFont);

	return S_OK;
}

void Engine::CFontMgr::Render(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString , D3DXCOLOR Color)
{
	CFont*	pFont = (CFont*)m_htFont.Search_TableData(pFontKey);
	if(NULL == pFont)
		return ;

	pFont->Render(pString, Color, vPos);
}

void Engine::CFontMgr::Free(void)
{
	size_t iTableSize =	m_htFont.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		CFont*		pFont = (CFont*)m_htFont.Erase_Slot(i);
		if(NULL != pFont)
			Safe_Release(pFont);
	}
	m_htFont.Erase_Table();
}

