
// MapObjToolDoc.h : CMapObjToolDoc Ŭ������ �������̽�
//


#pragma once


class CMapObjToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMapObjToolDoc();
	DECLARE_DYNCREATE(CMapObjToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMapObjToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


