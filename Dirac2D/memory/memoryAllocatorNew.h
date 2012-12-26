/*
 *  memoryAllocatorNew.h
 *  Dirac2D
 *
 *  Created by Prashant on 17/04/12.
 *
 */

#include "../definitions.h"
#include "../common.h"
#include <stdio.h>

#ifndef _MEMORY_ALLOCATOR_NEW_H_
#define _MEMORY_ALLOCATOR_NEW_H_

BEGIN_NAMESPACE_DIRAC2D

#define MEMORY_CHUNK_SIZE (16*1024)  // 16 kB

/*
  --> This Allocator allocates initially 16 kB memory for the given object type.
  
  --> once this heap is full, new heap is allocated( same size 16 kB ), and so on.
 
  --> A free pointer keeps track of the memory available for allocation. 
 */

template< class T >
class MemoryAllocatorNew
{
private:
	
	struct MemoryBlock
	{
		MemoryBlock* m_pNext;
	};
	
	struct Heap
	{
		Heap()
		{
			m_pMemoryChunk = 0;
		}
		// Size of the Memory Block.
		dint32 m_MemoryBlockSize;
		// Pointer to the Entire Memory Chunk
		MemoryBlock* m_pMemoryChunk;
	};
	
public:
	MemoryAllocatorNew(dint32 capacity) : m_Capacity( capacity )
	{		
		m_HeapCount = 0;
		// Allocate the Heap pointer.
		m_pHeap = (Heap*)malloc( sizeof(Heap) );
		m_pFreeMemoryBlock = 0;
		m_HeapCount = 1;
		m_CurrentHeapCount = 0;
	}
	
	~MemoryAllocatorNew()
	{
		for( dint32 i=0; i<m_HeapCount; i++ )
		{
			Heap* pHeap = m_pHeap + i;
			free(pHeap->m_pMemoryChunk);
		}
		free(m_pHeap);
	}
	
	T* Allocate()
	{
		if( m_pFreeMemoryBlock )
		{
			// Allocate Memory from the Heap
			MemoryBlock* pBlock = m_pFreeMemoryBlock;
			// set the Free pointer to the Next Memory Block
			m_pFreeMemoryBlock = pBlock->m_pNext;
			return (T*)pBlock;
		}
		// Allocate memory.
		else 
		{
			// Heap is full. need to reallocate.
			if( m_HeapCount == m_CurrentHeapCount )
			{
				m_HeapCount++;
				Heap* newHeap = (Heap*)malloc(m_HeapCount * sizeof(Heap));
				memcpy(newHeap, m_pHeap, sizeof(Heap) * m_CurrentHeapCount);
				free(m_pHeap);
				m_pHeap = newHeap;
			}
			
			// Allocate contiguous memory for the new heap.
			Heap* pHeap = m_pHeap + m_CurrentHeapCount;
			pHeap->m_pMemoryChunk = (MemoryBlock*)malloc( MEMORY_CHUNK_SIZE );
			
			// Initialize the memory      
			dint32 blockSize = sizeof(T);
			pHeap->m_MemoryBlockSize = blockSize;
			dint32 blockCount = MEMORY_CHUNK_SIZE/blockSize;
			
			MemoryBlock* pBlock;
			for (dint32 i=0; i<blockCount-1; i++) 
			{
				pBlock = (MemoryBlock*)( (dchar*)pHeap->m_pMemoryChunk +  blockSize * i );
				pBlock->m_pNext = (MemoryBlock*)( (dchar*)pHeap->m_pMemoryChunk + blockSize * (i+1));
			}
			pBlock = (MemoryBlock*)( (dchar*)pHeap->m_pMemoryChunk +  blockSize * (blockCount-1) );
			pBlock->m_pNext = 0;
			
			m_pFreeMemoryBlock = pHeap->m_pMemoryChunk->m_pNext;
			m_CurrentHeapCount++;
			
			return (T*)pHeap->m_pMemoryChunk;
		}
		
	}
	
	
	void Free(T* ptr)
	{
		MemoryBlock* pBlock = m_pFreeMemoryBlock;
		m_pFreeMemoryBlock = (MemoryBlock*)ptr;
		((MemoryBlock*)ptr)->m_pNext = pBlock;		
	}
	
	void printMemoryStats()
	{
		printf("No of Heaps Allocated = %d\n", m_HeapCount);
		printf("Total Memory Used = %d kB\n", m_HeapCount * MEMORY_CHUNK_SIZE/ 1000 );
	}
protected:
	dint32 m_Capacity, m_CurrentHeapCount;
	Heap* m_pHeap;  // pointer to the heap. 
	dint32 m_HeapCount; // Number of heaps allocated. 
	MemoryBlock* m_pFreeMemoryBlock; // Free pointer pointing to freely available memory.	
};

END_NAMESPACE_DIRAC2D

#endif


