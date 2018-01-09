/*!
 * \file Random_Number.h
 * \date 2016/07/01 0:05
 *
 * \author ±Ç¿ÀÈÆ
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Random_Number_h__
#define Random_Number_h__

class CRandom_Number
{
public:
	static unsigned int Get_RandomNum(unsigned int iMaxNumber = 0, unsigned int iNumCount = 3)
	{
		iMaxNumber = iMaxNumber == 0 ? rand() % 9 + 1 : iMaxNumber;

		iNumCount -= 1;
		iMaxNumber *= (int)pow((double)10, (double)iNumCount);
		for(unsigned int i = 0; i < iNumCount; ++i)
		{
			int iNum = rand() % 10;
			iNum *= (int)pow((double)10, (double)i);
			iMaxNumber += iNum;
		}
		return iMaxNumber;
	}
};

#endif // Random_Number_h__