//
//  memoryBlockAllocator.h
//  Dirac2D
//
//  Created by Prashant Sharma on 18/11/12.
//
#include "../definitions.h"
#include "../common.h"

#ifndef _MEMORY_BLOCK_ALLOCATOR_H_
#define _MEMORY_BLOCK_ALLOCATOR_H_

/*
 This is a Memory Block Allocator, and is inspired from the following Link::
 http://www.codeguru.com/cpp/misc/misc/memory/article.php/c13101/Fast-Efficient-Allocator-for-Small-Blocks-of-Memory.htm

 Here are some notes::
 
 1. This Allocator manages a Number of Heaps.
 
 2. Each Heap is pointing to a Memory Chunk. Memory Chunk is a kind of collection of Memory
    Blocks. 
 
 3. Size of a Memory Block is fixed for each Heap.
 
 3. Given a size of the Object to allocate, we need to determine the Heap from which the Memory 
    Chunk/Block will be used for the allocation of this object. this is done through a simple lookup 
    table.
 
 4. To access an available memory block given a Heap, an array of Free Pointers( for each Heap)
    are used, which keeps track of the available free Memory Block.
 
 5. Memory Blocks are connected via a singly linked list to navigate through, when searching for
    the next free Block.
 
 6. Objects of size <= 1024 bytes are allocated by the allocator. size > 1024 bytes are handeled
    by malloc/free.
 
 */

BEGIN_NAMESPACE_DIRAC2D

#define NUM_HEAPS 36
#define NUM_MEMORY_BLOCKS 14
#define MEMORY_CHUNK_SIZE (16*1024)
#define MAX_OBJECT_SIZE 1024

struct MemoryBlock
{
    MemoryBlock* m_pNext;
};

struct Heap
{
    // Size of the Memory Block.
    dint32 m_MemoryBlockSize;
    // Pointer to the Entire Memory Chunk
    MemoryBlock* m_pMemoryChunk;
};

class MemoryBlockAllocator
{
  public:
    MemoryBlockAllocator();
    
    ~MemoryBlockAllocator();
    
    void* Allocate(dint32 size);
    
    void Free(void* ptr, dint32 size);
    
  private:
    Heap* m_pHeapArray;  // pointer to the heap array.
    dint32 m_HeapCount;  // number of heaps.
    dint32 m_CurrentHeapCount;
    
    MemoryBlock* m_pFreeMemoryBlock[NUM_HEAPS];
    
    static dint32 m_HeapLookUpTable[MAX_OBJECT_SIZE+1];
};



END_NAMESPACE_DIRAC2D

#endif
