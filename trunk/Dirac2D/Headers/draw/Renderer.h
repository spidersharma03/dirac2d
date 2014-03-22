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

enum POLYGON_MODE { EPM_FILL, EPM_LINE, EPM_POINT };

struct RenderingAttributes
{
	POLYGON_MODE m_PolygonMode;	
	
	RenderingAttributes()
	{
		m_PolygonMode = EPM_FILL;
	}
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
	
	virtual void setPointSize(dfloat size) = 0;
	
	virtual void setLineWidth(dfloat width) = 0;

	virtual void setColor( dchar r, dchar g, dchar b, dchar a ) = 0;

	virtual void setColor( dchar r, dchar g, dchar b ) = 0;

	virtual void drawShape(const CollisionShape*) = 0;
	
	virtual void drawCircle( dfloat cx, dfloat cy, dfloat radius ) = 0;
	
	virtual void drawBox( dfloat width, dfloat height) = 0;
	
	virtual void drawPolygon( const Vector2f* vertices, dint32 numVertices ) = 0;
	
	virtual void drawLine( const Vector2f& begin, const Vector2f& end) = 0;
	
	virtual void drawPoint( const Vector2f& point ) = 0;
	
	virtual void drawAABB( const AABB2f& aabb ) = 0;
	
	virtual void drawCapsule( dfloat cx, dfloat cy, dfloat width, dfloat height) = 0;

	virtual void setTransform( const Matrix3f& xform) = 0;
	
protected:
	
	RenderingAttributes m_RenderingAttributes;
	PhysicalWorld* m_PhysicalWorld;
};


END_NAMESPACE_DIRAC2D

#endif