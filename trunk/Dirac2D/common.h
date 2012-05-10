#include "Settings.h"
#include "definitions.h"
#include "maths/MathUtil.h"

#include <cassert>

#ifndef _COMMON_H_
#define _COMMON_H_

BEGIN_NAMESPACE_DIRAC2D

#define dAssert(EXP) assert(EXP)

template<class T>
struct dList
{
	dList()
	{
		next = 0;
	}
	dList* next;
	T* data;
	dint32 size;
};

END_NAMESPACE_DIRAC2D

#endif