#ifndef Frame_h__
#define Frame_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);
public:
	bool Pass_Limit(const _float& fTimeDelta);
public:
	HRESULT Ready_Frame(const _float& fLimitCall);
public:
	static CFrame* Create(const _float& fLimitCall);
private:
	float				m_fLimitCall;
	float				m_fAccTimes;
public:
	virtual void Free(void);


};

END


#endif // Frame_h__
