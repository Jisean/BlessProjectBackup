
// MapObjToolDoc.cpp : CMapObjToolDoc Ŭ������ ����
//

#include "stdafx.h"
#include "MapObjTool.h"

#include "MapObjToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapObjToolDoc

IMPLEMENT_DYNCREATE(CMapObjToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapObjToolDoc, CDocument)
END_MESSAGE_MAP()


// CMapObjToolDoc ����/�Ҹ�

CMapObjToolDoc::CMapObjToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMapObjToolDoc::~CMapObjToolDoc()
{
}

BOOL CMapObjToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CMapObjToolDoc serialization

void CMapObjToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CMapObjToolDoc ����

#ifdef _DEBUG
void CMapObjToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapObjToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapObjToolDoc ���
