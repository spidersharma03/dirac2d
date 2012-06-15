/*
 *  DynamicTree.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 13/06/12.
 *
 */

#include <stdlib.h>
#include <string>
#include "DynamicTree.h"
#include "../../common.h"

BEGIN_NAMESPACE_DIRAC2D

DynamicTree::DynamicTree()
{
	m_NodeCapacity = 100;
	m_CurrentNodeCount = 0;
	m_Nodes = (DynamicTreeNode*)malloc(m_NodeCapacity * sizeof(DynamicTreeNode));
	
	memset(m_Nodes, 0, m_NodeCapacity * sizeof(DynamicTreeNode) );
	
	for( dint32 i=0; i<m_NodeCapacity-1; i++ )
	{
		DynamicTreeNode* pNode = m_Nodes + i;
		pNode->m_Next = i+1;
		pNode->m_Depth = -1;
	}

	DynamicTreeNode* pNode = m_Nodes + m_NodeCapacity - 1;
	pNode->m_Next =  Null_Node;
	pNode->m_Depth = -1;
	m_FreeNode = 0;
	
	m_RootNode = Null_Node;
}

void DynamicTree::createProxy( AABB2f& nodeAABB )
{
	dint32 newNode = allocateNode();
	m_Nodes[newNode].m_AABB = nodeAABB;
	
	insertNode(nodeAABB, newNode);
}

void DynamicTree::removeProxy(dint32 proxyID)
{
}

dint32 DynamicTree::allocateNode()
{
	// If the Node Capacity is reached, allocate new memory and erase old one.
	if( m_CurrentNodeCount == m_NodeCapacity )
	{
		// increase capacity.
		m_NodeCapacity += m_NodeCapacity;
		// Allocate new memory
		DynamicTreeNode* newNodes = (DynamicTreeNode*)malloc(m_NodeCapacity * sizeof(DynamicTreeNode));
		dAssert( newNodes );
		// Copy old data to new memory.
		memcpy(newNodes, m_Nodes, m_CurrentNodeCount * sizeof(DynamicTreeNode));
		// relaese old memory.
		free(m_Nodes);
		m_Nodes = newNodes;
		
		for( dint32 i=0; i<m_NodeCapacity-1; i++ )
		{
			DynamicTreeNode* pNode = m_Nodes + i;
			pNode->m_Next = i;
			pNode->m_Depth = -1;
		}
		
		DynamicTreeNode* pNode = m_Nodes + m_NodeCapacity - 1;
		pNode->m_Next =  Null_Node;
		pNode->m_Depth = -1;
		m_FreeNode = m_CurrentNodeCount;
	}
	
	dint32 node = m_FreeNode;
	m_Nodes[node].m_Child1 = Null_Node;
	m_Nodes[node].m_Child2 = Null_Node;
	
	m_FreeNode = m_Nodes[node].m_Next;
	m_CurrentNodeCount++;
	return node;
}

void DynamicTree::deleteNode(dint32 nodeID)
{
	dAssert( nodeID < m_CurrentNodeCount && nodeID > 0);
	m_Nodes[nodeID].m_Next = m_FreeNode;
	m_FreeNode = nodeID;
	m_CurrentNodeCount--;
}

// Insert a new Node at the location where the insertion cost is minimal.( see Automatic Creation of Object Hierarchies for Ray Tracing by GoldSmith & Salmon)
void DynamicTree::insertNode(const AABB2f& nodeAABB, dint32 nodeID)
{
	// first node insertion.
	if( m_RootNode == Null_Node )
	{
		m_RootNode = nodeID;
		m_Nodes[m_RootNode].m_AABB = nodeAABB;
		m_Nodes[m_RootNode].m_Parent = Null_Node;
		return;
	}
	// Find the Location where we need to insert the new node.
	DynamicTreeNode* pNode = m_Nodes + m_RootNode;
	dint32 insertionLocation = m_RootNode;
	while ( !pNode->isLeaf() )
	{
		AABB2f aabb = pNode->m_AABB;
		dfloat oldArea = aabb.getPerimeter();
		aabb.combine(nodeAABB);
		dfloat newArea = aabb.getPerimeter();
		// The Cost of Creating a new Parent and adding this node and the new node under the Parent.
		dfloat cost = 2.0f * newArea;
		
		// cost of descending this node further down the hierarchy.
		dfloat descendingCost = 2.0f * ( newArea -  oldArea );
		
		// cost of descending in child1 
		dfloat descendingChild1Cost = descendingCost;
		if( m_Nodes[pNode->m_Child1].isLeaf() )
		{
			AABB2f child1_aabb = m_Nodes[pNode->m_Child1].m_AABB;
			child1_aabb.combine(nodeAABB);
			descendingChild1Cost += 2.0f * child1_aabb.getPerimeter();
		}
		else 
		{
			AABB2f child1_aabb = m_Nodes[pNode->m_Child1].m_AABB;
			oldArea = child1_aabb.getPerimeter();
			child1_aabb.combine(nodeAABB);
			newArea = child1_aabb.getPerimeter();
			descendingChild1Cost += 2.0f * ( newArea - oldArea );
		}

		// cost of descending in child2 
		dfloat descendingChild2Cost = descendingCost;
		if( m_Nodes[pNode->m_Child2].isLeaf() )
		{
			AABB2f child2_aabb = m_Nodes[pNode->m_Child2].m_AABB;
			child2_aabb.combine(nodeAABB);
			descendingChild2Cost += 2.0f * child2_aabb.getPerimeter();
		}
		else 
		{
			AABB2f child2_aabb = m_Nodes[pNode->m_Child2].m_AABB;
			oldArea = child2_aabb.getPerimeter();
			child2_aabb.combine(nodeAABB);
			newArea = child2_aabb.getPerimeter();
			descendingChild2Cost += 2.0f * ( newArea - oldArea );
		}
		
		if( cost < descendingChild1Cost && cost < descendingChild2Cost )
		{
			break;
		}
		
		if( descendingChild1Cost < descendingChild2Cost )
		{
			insertionLocation = pNode->m_Child1;
		}
		else 
		{
			insertionLocation = pNode->m_Child2;
		}

		pNode = m_Nodes + insertionLocation;
	}
	
	// Create a new Parent Node
	dint32 newParentNodeIndex = allocateNode();
	DynamicTreeNode* newParentNode = m_Nodes + newParentNodeIndex;
	newParentNode->m_AABB.combine(nodeAABB, m_Nodes[insertionLocation].m_AABB);
	
	dint32 oldParentIndex = m_Nodes[insertionLocation].m_Parent;
	
	// Old parent was a root node.
	if( m_Nodes[insertionLocation].m_Parent == Null_Node )
	{
		m_RootNode = newParentNodeIndex;
		newParentNode->m_Parent = Null_Node;
	}
	else 
	{
		if( m_Nodes[oldParentIndex].m_Child1 == insertionLocation )
			m_Nodes[oldParentIndex].m_Child1 = newParentNodeIndex;
		if( m_Nodes[oldParentIndex].m_Child2 == insertionLocation )
			m_Nodes[oldParentIndex].m_Child2 = newParentNodeIndex;
		newParentNode->m_Parent = oldParentIndex;
	}

	newParentNode->m_Child1 = insertionLocation;
	newParentNode->m_Child2 = nodeID;
	m_Nodes[insertionLocation].m_Parent = newParentNodeIndex;
	m_Nodes[nodeID].m_Parent = newParentNodeIndex;	
	
	// Now Change the AABB's of the tree up to the parent.
	pNode = newParentNode;
	while ( pNode->m_Parent != -1 ) 
	{
		DynamicTreeNode* parentNode = m_Nodes + pNode->m_Parent;
		parentNode->m_AABB.combine( pNode->m_AABB);
		pNode = m_Nodes + pNode->m_Parent;
	}
}

void DynamicTree::removeNode(dint32 nodeID)
{
}


DynamicTree::~DynamicTree()
{
	free(m_Nodes);
	m_Nodes = 0;
}

END_NAMESPACE_DIRAC2D