#ifndef CWY_Struct_h__
#define CWY_Struct_h__

typedef struct tagUIInfo
{
	TCHAR	TextureKey[30];
	TCHAR	TextureName[30];
	WORD	ContainerNumber;
	WORD	wSortNumber;

	float fX;
	float fY;
	float fSizeX;
	float fSizeY;
	float fStateIndex;
	float fMaxIndex;
	bool  bPressed;
	bool  bPush;
	bool  bCheck;
	bool  bCrush;
	bool  bStop;

	D3DXMATRIX matView;
	D3DXMATRIX matProj;

}UI_INFO;


#endif // CWY_Struct_h__
