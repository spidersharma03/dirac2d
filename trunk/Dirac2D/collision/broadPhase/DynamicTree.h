/*
 *  DynamicTree.h
 *  Dirac2D
 *
 *  Created by Prashant on 13/06/12.
 *
 */

#include "../../definitions.h"
#include "../../maths/MathUtil.h"

#ifndef _DYNAMIC_TREE_H_
#define _DYNAMIC_TREE_H_

BEGIN_NAMESPACE_DIRAC2D

#define Null_Node -1

/*
 Dynamic Tree is a strucure which is used to accelerate BroadPhase collisions, Ray cast and overlap/collision queries.
 the implementation here is based upon the paper by GoldSmith & Salmon::Automatic Creation of Object Hierarchies for Ray Tracing.
 Box2D's dynamic tree was a great reference for the implementation.
 
 Basically the Construction of the tree is incremental. whenever a new node needs to be inserted in the tree, a cost function as described in the above paper is calculated.
 this cost function decides the way in which the new node is inserted in the tree. see the paper for details.
 */

struct DynamicTreeNode
{
	inline dbool isLeaf()
	{
		return m_Child1 == Null_Node;
	}
	AABB2f m_AABB;
	// storing the ID's rather than the pointers so that even when the capacity is changed, the ID's remain the same.
	dint32 m_Child1, m_Child2;
	union 
	{
		dint32 m_Next;
		dint32 m_Parent;
	};
	
	dint32 m_Depth;
};

class DynamicTree
{
public:
	DynamicTree();
	
	~DynamicTree();
	
	void createProxy(AABB2f& nodeAABB);
	
	void removeProxy(dint32 proxyID );
	
	DynamicTreeNode* getNode(dint32 index)
	{
		return m_Nodes + index;
	}
	
	DynamicTreeNode* getRootNode()
	{
		return m_Nodes + m_RootNode;
	}
	
protected:

	void insertNode(const AABB2f& node, dint32 nodeID);
	
	void removeNode(dint32 nodeID);
	
	dint32 allocateNode();
	
	void deleteNode(dint32 nodeID);
	
	DynamicTreeNode* m_Nodes;
	duint32 m_CurrentNodeCount;
	duint32 m_NodeCapacity;
	
	dint32 m_FreeNode;
	
	dint32 m_RootNode;
};

END_NAMESPACE_DIRAC2D

#endif