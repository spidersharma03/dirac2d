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
		pNode->m_Height = -1;
	}

	DynamicTreeNode* pNode = m_Nodes + m_NodeCapacity - 1;
	pNode->m_Next =  Null_Node;
	pNode->m_Height = -1;
	m_FreeNode = 0;
	
	m_RootNode = Null_Node;
}

dint32 DynamicTree::createProxy( AABB2f& nodeAABB, void* userData )
{
	dint32 newNode = allocateNode();
	m_Nodes[newNode].m_AABB = nodeAABB;
	m_Nodes[newNode].m_UserData = userData;
	
	insertNode(nodeAABB, newNode);
	return newNode;
}

void DynamicTree::removeProxy(dint32 proxyID)
{
	// assert if the node to remove is not a leaf.
	dAssert( m_Nodes[proxyID].isLeaf() );
	// assert if the node to remove is out of limits of nodeCapacity.
	dAssert( proxyID >= 0 && proxyID < m_NodeCapacity );

	removeNode(proxyID);
	deleteNode(proxyID);
}

void DynamicTree::updateProxy(AABB2f& nodeAABB, dint32 proxyID)
{
	removeNode(proxyID);
	m_Nodes[proxyID].m_AABB = nodeAABB;
	insertNode(nodeAABB, proxyID);
}

dint32 DynamicTree::allocateNode()
{
	// If the Node Capacity is reached, allocate new memory and erase old one.
	if( m_FreeNode == Null_Node )
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
		
		for( dint32 i=m_CurrentNodeCount; i<m_NodeCapacity-1; i++ )
		{
			DynamicTreeNode* pNode = m_Nodes + i;
			pNode->m_Next = i+1;
			pNode->m_Height = -1;
		}
		
		DynamicTreeNode* pNode = m_Nodes + m_NodeCapacity - 1;
		pNode->m_Next =  Null_Node;
		pNode->m_Height = -1;
		m_FreeNode = m_CurrentNodeCount;
	}
	
	dint32 node = m_FreeNode;
	m_Nodes[node].m_Child1 = Null_Node;
	m_Nodes[node].m_Child2 = Null_Node;
	m_Nodes[node].m_Height = 0;
	
	m_FreeNode = m_Nodes[node].m_Next;
	m_CurrentNodeCount++;
	return node;
}

void DynamicTree::deleteNode(dint32 nodeID)
{
	dAssert( nodeID < m_NodeCapacity && nodeID >= 0);
	m_Nodes[nodeID].m_Next = m_FreeNode;
	m_Nodes[nodeID].m_Height = -1;
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
	// Height of the new parent
	newParentNode->m_Height = m_Nodes[insertionLocation].m_Height + 1;
	
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
	
	// Reset the Bounding Boxes and Heights of the tree nodes
	dint32 index = m_Nodes[nodeID].m_Parent;
	while ( index != Null_Node ) 
	{
		dint32 child1Index = m_Nodes[index].m_Child1;
		dint32 child2Index = m_Nodes[index].m_Child2;
		m_Nodes[index].m_AABB.combine( m_Nodes[child1Index].m_AABB, m_Nodes[child2Index].m_AABB );
		m_Nodes[index].m_Height = 1 + MAX(m_Nodes[child1Index].m_Height, m_Nodes[child1Index].m_Height);
		index = m_Nodes[index].m_Parent;
	}
}

void DynamicTree::removeNode(dint32 nodeID)
{
	// Return if there is no parent.
	if( m_Nodes[nodeID].m_Parent == Null_Node )
	{
		return;
	}
	
	dint32 parentNodeIndex = m_Nodes[nodeID].m_Parent;
	dint32 grandParentNodeIndex = m_Nodes[parentNodeIndex].m_Parent;
	
	// This is the child which should be moved up in the hierarchy.
	dint32 childNodeIndex;
	if( m_Nodes[parentNodeIndex].m_Child1 == nodeID )
	{
		childNodeIndex = m_Nodes[parentNodeIndex].m_Child2;
	}
	if( m_Nodes[parentNodeIndex].m_Child2 == nodeID ) 
	{
		childNodeIndex = m_Nodes[parentNodeIndex].m_Child1;
	}

	// set the new child as the child of the grandParent.
	if( grandParentNodeIndex != Null_Node )
	{
		if( m_Nodes[grandParentNodeIndex].m_Child2 == parentNodeIndex )
		{
			m_Nodes[grandParentNodeIndex].m_Child2 = childNodeIndex;
		}
		else 
		{
			m_Nodes[grandParentNodeIndex].m_Child1 = childNodeIndex;
		}
		m_Nodes[childNodeIndex].m_Parent = grandParentNodeIndex;
	}
	else 
	{
		m_RootNode = childNodeIndex;
		m_Nodes[childNodeIndex].m_Parent = Null_Node;
	}
	
	// Remove the Parent node.
	deleteNode(parentNodeIndex);
	
	// Reset the Bounding Boxes and Heights of the tree nodes
	dint32 index = grandParentNodeIndex;
	while ( index != Null_Node ) 
	{
		dint32 child1Index = m_Nodes[index].m_Child1;
		dint32 child2Index = m_Nodes[index].m_Child2;
		m_Nodes[index].m_AABB.combine( m_Nodes[child1Index].m_AABB, m_Nodes[child2Index].m_AABB );
		m_Nodes[index].m_Height = 1 + MAX(m_Nodes[child1Index].m_Height, m_Nodes[child1Index].m_Height);
		index = m_Nodes[index].m_Parent;
	}
}

dint32 DynamicTree::calculateHeight(dint32 nodeID)
{
	return 0;
}

// Balance the tree rooted at rootID. see AVL Tree Wiki.
dint32 DynamicTree::balance(dint32 rootID)
{
	dint32 child1Index = m_Nodes[rootID].m_Child1;
	dint32 child2Index = m_Nodes[rootID].m_Child2;
	
	dint32 diffHeight = m_Nodes[child1Index].m_Height - m_Nodes[child2Index].m_Height;
	
	// Pivot node will become the new root.
	dint32 pivotNode = rootID;
	// Left subtree is having larger depth than Right subtree. in this case we need to do a RIGHT ROTATION on this root node.
	if( diffHeight > 1 )
	{
		pivotNode = child1Index;
		m_Nodes[rootID].m_Child1 = m_Nodes[pivotNode].m_Child2;
		m_Nodes[pivotNode].m_Child2 = rootID;
		
		m_Nodes[rootID].m_Parent = m_Nodes[pivotNode].m_Child2;
		m_Nodes[m_Nodes[pivotNode].m_Child2].m_Parent = m_Nodes[rootID].m_Child1;
	}
	// Right subtree is having larger depth than Left subtree. in this case we need to do a LEFT ROTATION on this root node.
	if( diffHeight < -1 )
	{
		pivotNode = child2Index;
		m_Nodes[rootID].m_Child2 = m_Nodes[pivotNode].m_Child1;
		m_Nodes[pivotNode].m_Child1 = rootID;
		
		m_Nodes[rootID].m_Parent = m_Nodes[pivotNode].m_Child1;
		m_Nodes[m_Nodes[pivotNode].m_Child1].m_Parent = m_Nodes[rootID].m_Child2;
	}
	return pivotNode;
}

DynamicTree::~DynamicTree()
{
	free(m_Nodes);
	m_Nodes = 0;
}

END_NAMESPACE_DIRAC2D