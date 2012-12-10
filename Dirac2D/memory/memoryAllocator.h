/*
 *  memoryAllocator.h
 *  Dirac2D
 *
 *  Created by Prashant on 17/04/12.
 *
 */

#include "../definitions.h"
#include "../common.h"

#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class MemoryAllocator
{
	struct MemoryLinkNode
	{
		MemoryLinkNode* pNext;
		MemoryLinkNode* pPrev;
	};
	
public:
	MemoryAllocator(dint32 nObjects) : m_MaxObjects( nObjects )
	{
		m_NumObjects = 0;
		// Allocate contiguous memory for the entire pool.
		dint32 chunkSize = m_MaxObjects * ( sizeof(T) + sizeof(MemoryLinkNode) );
		m_pMemoryChunk = malloc( chunkSize );
		
		dAssert(m_pMemoryChunk);
		
		// Initialize the Memory Pool.
		m_pFreeMemoryBlock = (MemoryLinkNode*)m_pMemoryChunk;
		m_pFreeMemoryBlock->pPrev = 0;
		
		// Connect the doubly linked Memory Nodes.
		for( dint32 o=1; o<m_MaxObjects; o++ )
		{
			MemoryLinkNode* ptr = (MemoryLinkNode*)((dchar*)m_pMemoryChunk + o * ( sizeof(T) + sizeof(MemoryLinkNode) ) );
			m_pFreeMemoryBlock->pNext = ptr;
			ptr->pPrev = m_pFreeMemoryBlock;
			m_pFreeMemoryBlock = ptr;
			
			if( o == m_NumObjects-1 )
				ptr->pNext = 0;
		}
		m_pFreeMemoryBlock		= (MemoryLinkNode*)m_pMemoryChunk;
	}
	
	~MemoryAllocator()
	{
		free(m_pMemoryChunk);
		m_pMemoryChunk = 0;
	}
	
	T* Allocate()
	{
		dAssert( m_MaxObjects > ++m_NumObjects );
        
		MemoryLinkNode* pLinkNode = m_pFreeMemoryBlock;
		if( m_pFreeMemoryBlock->pNext )
			m_pFreeMemoryBlock = m_pFreeMemoryBlock->pNext;	

		return (T*)((dchar*)pLinkNode + sizeof(MemoryLinkNode));
	}
	
	
	void Free(T* ptr)
	{
		// Free the Node
		MemoryLinkNode* pLinkNode = (MemoryLinkNode*)( (dchar*)ptr - sizeof(MemoryLinkNode) );
		MemoryLinkNode* prevNode = pLinkNode->pPrev;
		MemoryLinkNode* nextNode = pLinkNode->pNext;
		
		if( prevNode )
			prevNode->pNext = nextNode;
		//else
		//	m_pFreeMemoryBlock->pNext = nextNode;
			
		if( nextNode )
			nextNode->pPrev = prevNode;
		
		// Insert the freed node after the freeMemoryBlock node.
		MemoryLinkNode* pNextFreeNode = m_pFreeMemoryBlock->pNext;
		
		m_pFreeMemoryBlock->pNext = pLinkNode;
		pLinkNode->pPrev = m_pFreeMemoryBlock;
		
		if( pNextFreeNode )
			pNextFreeNode->pPrev = pLinkNode;
			
		pLinkNode->pNext = pNextFreeNode;

		m_NumObjects--;
	}
	
	inline T* operator[]( dint32 index )
	{
		MemoryLinkNode* pLinkNode = (MemoryLinkNode*)m_pMemoryChunk;
		for(dint32 i=0; i<index; i++ )
		{
			pLinkNode = pLinkNode->pNext;
		}
		return (T*)((dchar*)pLinkNode + sizeof(MemoryLinkNode) );
	}
	
protected:
	void* m_pMemoryChunk; // Pointer to the Entire Memory Allocated for this pool.
	MemoryLinkNode* m_pFreeMemoryBlock; // Pointer to Free Memory.
	
	dint32 m_MaxObjects;
	dint32 m_NumObjects;
};

END_NAMESPACE_DIRAC2D

#endif


