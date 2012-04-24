/*
 *  memoryAllocator.h
 *  Dirac2D
 *
 *  Created by Prashant on 17/04/12.
 *
 */

#include "../../Dirac2D/definitions.h"

#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class MemoryAllocator
{
public:
	MemoryAllocator()
	{
		
	}
	
	T* Allocate()
	{
	}
	
	void Free(T* ptr)
	{
	}
	
protected:
	
};

END_NAMESPACE_DIRAC2D

#endif

