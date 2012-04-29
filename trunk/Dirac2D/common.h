#include "Settings.h"
#include "definitions.h"
#include "maths/MathUtil.h"

#ifndef _COMMON_H_
#define _COMMON_H_

BEGIN_NAMESPACE_DIRAC2D

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