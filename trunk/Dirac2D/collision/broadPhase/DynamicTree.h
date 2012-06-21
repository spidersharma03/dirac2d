/*
 *  DynamicTree.h
 *  Dirac2D
 *
 *  Created by Prashant on 13/06/12.
 *
 */

#include "../../common.h"
#include "../../definitions.h"
#include "../../maths/MathUtil.h"
#include "../../CallBacks.h"

#include <vector>

using namespace std;

#ifndef _DYNAMIC_TREE_H_
#define _DYNAMIC_TREE_H_

BEGIN_NAMESPACE_DIRAC2D

class QueryCallBack;

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
	
	void* m_UserData;
	dint32 m_Height;
};

class DynamicTree
{
public:
	DynamicTree();
	
	~DynamicTree();
	
	dint32 createProxy(AABB2f& nodeAABB, void* userData);
	
	void removeProxy(dint32 proxyID );
	
	void updateProxy(AABB2f& nodeAABB, dint32 proxyID);
	
	dint32 balance(dint32 rootID);
	
	dint32 calculateHeight(dint32 nodeID);
	
	void validateTree(dint32 index);

	inline void* getUserData( dint32 proxyID ) const
	{
		return m_Nodes[proxyID].m_UserData;
	}
	
	inline DynamicTreeNode* getNode(dint32 index) const
	{
		return m_Nodes + index;
	}
	
	inline DynamicTreeNode* getRootNode() const
	{
		return m_Nodes + m_RootNode;
	}

	// check whether given queryAABB overlaps with any AABB leaf of the tree. for any overlap, the callBack class will be reported.
	inline dbool overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack )
	{	
		dint32 nodeCount = 0;
		nodeVector[nodeCount++] = m_RootNode;
		
		dbool bResult = false;
		
		while ( nodeCount != 0 ) 
		{		
			dint32 nodeID;
			
			nodeCount--;
			nodeID = nodeVector[nodeCount];
			
			if( nodeID == Null_Node )
				continue;
			
			if( queryAABB.intersectAABB(m_Nodes[nodeID].m_AABB) )
			{
				if( m_Nodes[nodeID].isLeaf() )
				{
					bResult = true;
					
					if( callBack )
						callBack->overlapCallBack(nodeID);
				}
				else 
				{
					nodeVector[nodeCount++] = m_Nodes[nodeID].m_Child1;
					nodeVector[nodeCount++] = m_Nodes[nodeID].m_Child2;
				}
			}
		}
		return bResult;
	}
	
	// check whether given queryAABB overlaps with any AABB leaf of the tree. a vector of overlapped aabb is filled here.
	dbool overlapAABB( AABB2f& queryAABB, vector<dint32>& vecOverlappedIDs );
	//
	dbool intersectRay(RayIntersectionCallBackClass* callBack)
	{
		if( callBack )
			callBack->rayIntersectionCallBack(0);
		return false;
	}
	
protected:
	dint32 nodeVector[50];

	void insertNode(const AABB2f& node, dint32 nodeID);
	
	void removeNode(dint32 nodeID);
	
	// Allocates a new node from the pool.
	dint32 allocateNode();
	
	void deleteNode(dint32 nodeID);
	
	DynamicTreeNode* m_Nodes;
	duint32 m_CurrentNodeCount;
	duint32 m_NodeCapacity;
	
	dint32 m_FreeNode;
	
	dint32 m_RootNode;
};


//dbool DynamicTree::intersectAABB( AABB2f& queryAABB, void* callBack )


END_NAMESPACE_DIRAC2D

#endif