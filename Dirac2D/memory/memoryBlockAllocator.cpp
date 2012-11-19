
#include "memoryBlockAllocator.h"
#include <string>

BEGIN_NAMESPACE_DIRAC2D

// Memory Block size for each Heap
dint32 memoryBlockSize[] = 
{
    8,
    16,
    32,
    48,
    64,
    96,
    128,
    144,
    160,
    176,
    192,
    224,
    256,
    288,
    320,
    352,
    384,
    416,
    480,
    512,
    544,
    576,
    608,
    640,
    672,
    704,
    736,
    768,
    800,
    832,
    864,
    896,
    928,
    960,
    992,
    1024
};


MemoryBlockAllocator::MemoryBlockAllocator()
{
    m_HeapCount = NUM_HEAPS;
    m_pHeapArray = (Heap*)malloc(m_HeapCount * sizeof(Heap));
    m_CurrentHeapCount = 0;
    
    memset(m_pFreeMemoryBlock, 0, NUM_HEAPS * sizeof(MemoryBlock));
    // initialize the heap lookup table.
    dint32 n = 0;
    for (dint32 i=0; i<=MAX_OBJECT_SIZE; i++) 
    {
        if( i  <= memoryBlockSize[n] )
        {
            m_HeapLookUpTable[i] = n;
        }
        else
        {
            n++;
            m_HeapLookUpTable[i] = n;
        }
    }
}

void* MemoryBlockAllocator::Allocate(dint32 size)
{
    if( size == 0 )
        return 0;
    
    if( size > MAX_OBJECT_SIZE )
    {
        return malloc(size);
    }
    // Find out the Heap from which Memory Block will be returned.
    dint32 heapIndex = m_HeapLookUpTable[size];
    
    // We have Free Memory Block available in this Heap
    if( m_pFreeMemoryBlock[heapIndex] )
    {
        MemoryBlock* pBlock = m_pFreeMemoryBlock[heapIndex];
        // set the Free pointer to the Next Memory Block
        m_pFreeMemoryBlock[heapIndex] = pBlock->m_pNext;
        return pBlock;
    }
    // We don't have Free memory block in this Heap, so we need to create a new Heap space
    else
    {
        // All the Heaps are Full. need to create new ones.
        if( m_CurrentHeapCount == m_HeapCount )
        {
            m_HeapCount *= 2;
            Heap* newHeap = (Heap*)malloc(m_HeapCount * sizeof(Heap));
            memcpy(newHeap, m_pHeapArray, sizeof(Heap) * m_CurrentHeapCount);
            free(m_pHeapArray);
            m_pHeapArray = newHeap;
        }
        Heap* pHeap = m_pHeapArray + m_CurrentHeapCount;
        dint32 blockSize = memoryBlockSize[heapIndex];
        
        pHeap->m_pMemoryChunk = (MemoryBlock*)malloc( MEMORY_CHUNK_SIZE );
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
        
        m_pFreeMemoryBlock[heapIndex] = pHeap->m_pMemoryChunk->m_pNext;
        
        m_CurrentHeapCount++;
        
        return pHeap->m_pMemoryChunk;
    }
    
    return 0;
}

void MemoryBlockAllocator::Free(void* ptr, dint32 size)
{
    if( size == 0 )
        return;
    if( size > 1024 )
    {
        free(ptr);
        return;
    }
    // Find out the Heap from which Memory Block will be returned.
    dint32 heapIndex = m_HeapLookUpTable[size];
    
    MemoryBlock* pBlock = m_pFreeMemoryBlock[heapIndex];
    m_pFreeMemoryBlock[heapIndex] = (MemoryBlock*)ptr;
    ((MemoryBlock*)ptr)->m_pNext = pBlock;
}

MemoryBlockAllocator::~MemoryBlockAllocator()
{
    for( dint32 i=0; i<m_HeapCount; i++ )
    {
        Heap* pHeap = m_pHeapArray + m_CurrentHeapCount;
        free(pHeap->m_pMemoryChunk);
    }
    free(m_pHeapArray);
}

END_NAMESPACE_DIRAC2D
