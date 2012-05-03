/*
 *  Renderer.h
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "../definitions.h"
#include "../maths/MathUtil.h" 

#ifndef _RENDERER_H_
#define _RENDERER_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionShape;

class RenderingAttributes
{
};

class Renderer
{
public:
	Renderer(PhysicalWorld* world):m_PhysicalWorld(world)
	{
	}
	
	void setRenderingAttributes( RenderingAttributes& renderAtt )
	{
		m_RenderingAttributes = renderAtt;
	}
		
	virtual void drawShape(CollisionShape*) = 0;
	
	virtual void drawCircle( dfloat radius ) = 0;
	
	virtual void drawBox( dfloat width, dfloat height) = 0;
	
	virtual void drawPolygon( Vector2f* vertices, dint32 numVertices ) = 0;
	
	virtual void drawLine( Vector2f& begin, Vector2f& end) = 0;
	
	virtual void drawPoint( Vector2f& point ) = 0;
	
	virtual void setTransform( Matrix3f& xform) = 0;
	
protected:
	
	RenderingAttributes m_RenderingAttributes;
	PhysicalWorld* m_PhysicalWorld;
};


END_NAMESPACE_DIRAC2D

#endif