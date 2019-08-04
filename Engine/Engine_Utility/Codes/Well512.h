#pragma once

#ifndef __WELL512_H__
#define __WELL512_H__

#include "Engine_Define.h"
#include "Base.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CWell512
{
	DECLARE_SINGLETON(CWell512)

public:
	enum {
		DIFFER_VALUE = 100,
	};

public:
	explicit CWell512();
	explicit CWell512(unsigned int nSeed);
	~CWell512();

public:
	//int Values
	unsigned int Get_Value();
	unsigned int Get_Value(unsigned int min, unsigned int max);
	unsigned int Get_Value(unsigned int max);

	//float Values
	float Get_floatValues();
	float Get_floatValues(float fMinValue, float fMaxValue);
	float Get_floatValues(float fMaxValue);

protected:
	unsigned long state[16];
	unsigned int iIndex;

};

NS_END

#endif // !__WELL512_H__
