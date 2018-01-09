

Engine::CBase::CBase(void)
: m_dwRefCnt(0)
{

}

Engine::CBase::~CBase(void)
{

}



unsigned long Engine::CBase::Release(void)
{
	Free();

	if(0 == m_dwRefCnt)	
	{
		delete this;	
		return 0;
	}
	else	
		return m_dwRefCnt--;	
}