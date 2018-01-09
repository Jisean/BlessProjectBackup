#ifndef Struct_h__
#define Struct_h__

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
	// Identity 검사
	BOOL	IsIdentity( const D3DXMATRIX* pM ){ return D3DXMatrixIsIdentity(pM); }
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

typedef struct tagAttack_DMG_Timimg
{
	float fInterval_1;
	float fInterval_2;
}ATTACK_DMG_TIMIMG;


#endif // Struct_h__
