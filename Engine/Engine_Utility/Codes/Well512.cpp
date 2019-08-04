#include "Well512.h"

USING(ENGINE)

IMPLEMENT_SINGLETON(CWell512)

CWell512::CWell512()
{
	iIndex = 0;
	unsigned int s = static_cast<unsigned int>(time(NULL));

	for (int i = 0; i < 16; ++i)
	{
		state[i] = s;
		s += s + DIFFER_VALUE;
	}


}

CWell512::CWell512(unsigned int nSeed)
{
	iIndex = 0;
	unsigned int s = nSeed;

	for (int i = 0; i < 16; ++i)
	{
		state[i] = s;
		s += s + DIFFER_VALUE;
	}

}


CWell512::~CWell512()
{
}

unsigned int CWell512::Get_Value()
{
	unsigned int a, b, c, d;

	a = state[iIndex];
	c = state[iIndex + 13 & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = state[(iIndex + 9) & 15];
	c ^= (c >> 11);
	a = state[iIndex] = b ^ c;
	d = a ^ ((a << 5) & 0xda442d24U);
	iIndex = (iIndex + 15) & 15;
	a = state[iIndex];
	state[iIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

	return state[iIndex];
}

unsigned int CWell512::Get_Value(unsigned int min, unsigned int max)
{
	return min + (Get_Value() % (max - min));
}

unsigned int CWell512::Get_Value(unsigned int max)
{
	return Get_Value() % max;
}

float CWell512::Get_floatValues()
{
	unsigned int nRandValude = Get_Value();

	union { unsigned long uL; float f; } p;

	p.uL = (((nRandValude *= 16807) & 0x007fffff) - 1) | 0x3f800000;
	
	return p.f - 1.f;
}

float CWell512::Get_floatValues(float fMinValue, float fMaxValue)
{
	return fMinValue + Get_floatValues() * (fMaxValue - fMinValue);
}

float CWell512::Get_floatValues(float fMaxValue)
{
	return Get_floatValues() * fMaxValue;
}



