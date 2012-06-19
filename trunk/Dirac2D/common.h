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

template< class T >
class dStack
{
public:
	dStack(dint32 capacity ) : m_Capacity( capacity )
	{
		m_Data = (T*)malloc(m_Capacity * sizeof(T));
		m_CurrentCount = 0;
	}
	
	void push(T& value)
	{
		if( m_CurrentCount > m_Capacity )
		{
			m_Capacity += m_Capacity;
			T* newData = (T*)malloc(m_Capacity * sizeof(T));
			dAssert( newData );
			free(m_Data);
			memcpy(newData, m_Data, m_CurrentCount * sizeof(T) );
			m_Data = newData;
		}
		m_Data[m_CurrentCount++] = value;
	}
	
	T pop()
	{
		m_CurrentCount--;
		return m_Data[m_CurrentCount];
	}
	
	inline dint32 getSize()
	{
		return m_CurrentCount;
	}
	
protected:
	dint32 m_Capacity;
	dint32 m_CurrentCount;
	T* m_Data;
};

END_NAMESPACE_DIRAC2D

#endif