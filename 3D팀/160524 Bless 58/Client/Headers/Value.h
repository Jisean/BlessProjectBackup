#ifndef Value_h__
#define Value_h__

const WORD		WINCX = 1680;
const WORD		WINCY = 1050;

extern WORD		VTXCNTX;
extern WORD		VTXCNTZ;
extern WORD		VTXITV;
//const WORD		VTXCNTX = 257;
//const WORD		VTXCNTZ = 257;
//const WORD		VTXITV = 1;

const D3DXVECTOR3	SSHADOWEYE	= D3DXVECTOR3(-148.066f, 432.107f, -148.066f);	
const D3DXVECTOR3	SSHADOWAT	= D3DXVECTOR3(-142.399f, 424.358f, -142.399f);

const D3DXVECTOR3		g_vLook = D3DXVECTOR3(0.f, 0.f, -1.f);

extern bool		g_bBoundingBoxRender;

#endif // Value_h__
