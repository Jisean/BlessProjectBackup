#ifndef Calculator_h__
#define Calculator_h__

#include "Include.h"
#include "Component.h"

class CCalculator : public Engine::CComponent
{
private:
	explicit CCalculator(void);
	virtual ~CCalculator(void);
public:
	_float Compute_HeightOnTerrain(const _vec3* pPosition, const TCHAR* pTerrainTag);
public:
	static CCalculator* Create(void);
public:
	virtual void Free(void);
};


#endif // Calculator_h__
